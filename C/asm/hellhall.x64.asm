;;
;; Loader
;;
[BITS 64]

;;
;; tell the compiler to access
;; symbols relative to RIP
;;
DEFAULT REL

section .data
    dwSSN       dd 0h            ; SYSCALL SSN (32-bit)
    qAddr       dq 0h            ; `syscall` INSTRUCTION ADDRESS (64-bit)

section .text
    global SetConfig
SetConfig:
    mov [dwSSN], ecx            ; Move the value of ecx into dwSSN
    mov [qAddr], rdx            ; Move the value of rdx into qAddr
    ret

    global HellHall
HellHall:
    mov r10, rcx                ; Move the value of rcx into r10
    mov eax, [dwSSN]            ; Move the value of dwSSN into eax
    jmp qword [qAddr]           ; Jump to the address stored in qAddr (which points to `syscall`)
    ret

section .end
