section .text

global _start
global read
global write
global open
global close
global strlen

extern main

_start:

    mov eax, [esp]     ; argc
    mov ebx, esp       ; argv @esp+4
    add ebx, 4

    push ebx
    push eax

    call	main
    mov     ebx,eax
    mov	eax,1
    int 0x80

read:
    enter 0, 0
    push ebx
    push ecx
    push edx

    mov eax, 3         ; read sys call
    mov ebx, [ebp + 8]
    mov ecx, [ebp + 12]
    mov edx, [ebp + 16]

    int 0x80           ; sys call

    pop edx
    pop ecx
    pop ebx
    leave
    ret


write:
    enter 0, 0
    push ebx
    push ecx
    push edx

    mov eax, 4         ; write sys call
    mov ebx, [ebp + 8]
    mov ecx, [ebp + 12]
    mov edx, [ebp + 16]

    int 0x80           ; sys call

    pop edx
    pop ecx
    pop ebx
    leave
    ret

open:
    enter 0, 0

    push ebx           ; save regs
    push ecx           ; save regs

    mov eax, 5h        ; setup system call
    mov ebx, [ebp + 8] ; setup system call
    mov ecx, [ebp + 12] ; setup system call

    int 0x80           ; sys call

    pop ecx            ; restore regs
    pop ebx            ; restore regs

    leave
    ret

close:
    enter 0, 0

    push ebx           ; save regs

    mov eax, 6        ; setup system call
    mov ebx, [ebp + 8]

    int 0x80           ; sys call

    pop ebx            ; restore regs

    leave
    ret                ; clear args from stack and return


strlen:
    enter 0, 0
    push ebx

    mov eax, -1

.comp_len:
    inc eax            ; calc offset
    mov ebx, eax
    add ebx, [ebp+8]   ; take str from stack
    mov bl, BYTE [ebx] ; get byte at adress of ebx
    test bl, bl        ; check if byte is \0

    jne .comp_len      ; if not \0 cont to next byte

    pop ebx
    leave
    ret