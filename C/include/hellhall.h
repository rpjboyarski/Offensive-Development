//
// Created by ronan on 11/19/24.
//

#ifndef OFFENSIVE_DEV_HELLHALL_H
#define OFFENSIVE_DEV_HELLHALL_H
#include <common.h>

BOOL InitializeModule(PMODULE Module);
BOOL InitializeSyscall(IN ULONG SyscallHash);
VOID GetCurrentSyscallStruct(OUT PSyscall psF);
extern VOID SetConfig(WORD wSystemCall, PVOID pSyscallInst);
extern NTSTATUS HellHall();

#define PREP_SYSCALL(SyscallFunction)(SetConfig(SyscallFunction.SSN, SyscallFunction.Instruction))

#define INITIALIZE_SYSCALL(SyscallHash)                                  \
    switch (InitializeSyscall(SyscallHash)) {                 \
        case TRUE:                                                      \
            GetCurrentSyscallStruct(&Gate.Win32.S##SyscallHash);            \
            break;                                                      \
        case FALSE:                                                     \
            DEBUG_PRINT("[*] Failed to initialize " #SyscallHash " syscall structure"); \
            return FALSE;                                               \
    }


BOOL InitializeSyscalls();

#endif //OFFENSIVE_DEV_HELLHALL_H
