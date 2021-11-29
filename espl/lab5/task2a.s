section .text

global main
	
global _start
global read
global write
global open
global close
global strlen

global utoa_s
global atou_s

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

main:
	enter 204, 0                ;store buffer of 200 bytes 
	mov ebx, [ebp+12]           ;get argv
	add ebx, 4                  ;ebx: argv[1]

file_open:
	mov ebx, [ebx]              ;get data from argv[1]
	push dword 0                ;flag for open file
	push ebx                    
	
	call open                   ;open our file
	
	pop ebx                     
	pop ebx                     ;clean 

	mov [ebp-204], eax          ;save our file on stack
	cmp eax, 0                  ;check if we get an error
	jle get_error

file_read:

	push dword 200              ;set the size for our read function
	mov ebx, ebp
	sub ebx, 200
	push ebx                    ;set string for our read function
	push dword [ebp-204]        ;move our file for function read
	
	call read
	
	pop ebx                     ;stack balance
	pop ebx                     ;stack balance
	pop ebx                     ;stack balance
	cmp eax, -1                 ;check if we got an error
	jle get_error
	cmp eax, 0                  ;check if buffer is empty
	jg print_buffer

end_read_file:
	push dword [ebp-204]        ;set file to close our function
	
	call close
	
	pop ebx                     ;stack balance
	jmp return

print_buffer:
	push eax                    ;num of bytes to write to buffer 
	mov ebx, ebp
	sub ebx, 200
	push ebx                    ;point to buffer
	push dword 1                ;stdout

	call write

	pop ebx                     ;stack balance
	pop ebx                     ;stack balance
	pop ebx                     ;stack balance

	cmp eax, 0                  ;check if we got an error 
	jle get_error
	jmp file_read

return:
	mov eax, 0
	leave                       ;clear bytes
	ret

get_error:
	mov eax, 1 
	leave                       ;clear bytes
	ret


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

utoa_s:

    push ebp
	push ebx

	mov	ebp, esp
    mov [ebp-4], dword 10           ;save our divisor
    mov eax, [ebp+12]               ;get number to convert
    mov ebx, [ebp+16]               ;get buffer
    mov [ebp-8], ebx                ;stores buffer address
    mov ebx, [ebp-8]                ;move buffer pointer back

.divisor:

    mov edx, 0                      ;store remainder in edx
    div dword [ebp-4]               ;divide our number by the divisor, eax: stores quotient, edx: store remainder
    mov cl, dl                      ;give us the remainder

    or edx, eax                     ;check if edx = eax

    jne .write_char                 ;if are not equal write our char
    jmp .end_of_string              ;if equal write and finish

.write_char:

    add cl, '0'                     ;convert to ascii
    mov [ebx], cl                   ;put remainder in buffer before divide

    inc ebx                         ;move pointer to the next buffer cell

    jmp .divisor                    ;go to divisor

.end_of_string:

    mov [ebx], byte 0               ;put /0 in the end of the str 
    dec ebx                         ;ebx: point to one cell before \0
    mov ecx, [ebp-8]                ;ecx: point to first of buffer

.swap_bytes:

    cmp ecx, ebx                    ;check if ecx pointer => ebx pointer 

    jge FINISH                      ;if yes, finish our program

    mov al, [ebx]                   ;swap bytes between ebx and ecx to reverse the output

    xchg al, [ecx]
    mov [ebx], al                   ;move pointers
    dec ebx                         ;decrease ebx
    inc ecx                         ;increase ecx
    jmp .swap_bytes

atou_s:

	push ebp
	push ebx

	mov	ebp, esp
	mov edx, [ebp+12]               ;get our string
	mov eax, 0                      ;eax: will store our result
	
.L1:
	movzx ecx, byte [edx]           ;move to next char

	inc edx                         ;increment edx: to read next char

	cmp ecx, '0'                    ;check if there is a next char that is bigger than 0
	jb FINISH
	cmp ecx, '9'                    ;check if there is a next char that is smaller than 9
	ja FINISH

	sub ecx, '0'                    ;convert char to int
	imul eax, 10                    ;multiply our int by 10
	add eax, ecx                    ;add our digit to the number we have until now
	jmp .L1                         

FINISH:

	mov esp, ebp
	pop ebx
	pop ebp
	ret
