section .text
	
global _start
global read
global write
global open
global close
global strlen
global main

global utoa_s
global atou_s
global compare_str
global buffer_load
global comare_txt

section .bss
    int_str: resb 50                    ;assign memory space for input
    cmp_word: resb 50                   ;assign memory space for input (ws flag) argv[2]

section .data
	w_flag: db '-w', 0
    ws_flag: db '-ws', 0
	end_of_line: db 10
	num_words: db 'Number of words: ', 0
    num_words_len: equ  $ - num_words
    
    num_str_1: db 'Number of times the string: "', 0
    num_str_1_len: equ  $ - num_str_1
    
    num_str_2: db '" appeared on the file: ', 0
    num_str_2_len: equ  $ - num_str_2
    
    open_err: db 'Failed to open the file', 10, 0
    open_err_len: equ  $ - open_err

    close_err: db 'Failed to close the file', 10, 0
    close_err_len: equ  $ - close_err

    read_err: db 'Failed to read from file', 10, 0
    read_err_len: equ  $ - read_err

    write_err: db 'Failed to write to file', 10, 0
    write_err_len: equ  $ -  write_err
    
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


%define COUNT dword [ebp-208]               ;W FLAG: count words, WS FLAG: count num of input word
%define FLAG byte [ebp-209]                 ;flag 0: task2a, flag 1: '-w', flag 2: '-ws'
main:
	enter 209, 0                            ;GET 200 bytes, 4 bytes for file, 4 bytes for COUNT, 1 bytes for FLAG
	mov COUNT, 0                            ;counter = 0
	mov FLAG, 0                             ;prepare default flag 
	mov ebx, [ebp+12]                       ;argv
	add ebx, 4                              ;ebx = argv[1]

flag_check:
	push dword [ebx]                        ;prepare str2 for compare func
	push w_flag                             ;prepare str1 for compare func
	
	call compare_str
	
	pop edx                                 ;stack balance
	pop edx                                 ;stack balance
	cmp eax, 0                              ;argv[1] =?'-w'
	je w_flag_func
	
	push dword [ebx]                        ;prepare str2 for compare func
	push ws_flag                            ;prepare str1 for compare func
	
	call compare_str
	
	pop edx                                 ;stack balance
	pop edx                                 ;stack balance
	cmp eax, 0                              ;argv[1] =? '-ws'
	je ws_flag_func
	jmp file_open                           ;argv[1] = file name

w_flag_func:
	mov FLAG, 1                             ;prepare flag 1
	add ebx, 4                              ;file name = argv[2]
	jmp file_open
	
ws_flag_func:
	mov FLAG, 2                             ;prepare flag 2
	add ebx, 4                              ; '-ws' word = argv[2]
	push dword cmp_word                     ;prepare str2 for buffer_load
	push dword [ebx]                        ;prepare str1 for buffer_load
	
	call buffer_load
	
	pop edx                                 ;stack balance
	pop edx                                 ;stack balance
	add ebx, 4                              ;file name = argv[3]
	jmp file_open 

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

	push dword 200              ;prepare the size for our read function
	mov ebx, ebp
	sub ebx, 200
	push ebx                    ;prepare string for our read function
	push dword [ebp-204]        ;move our file for function read
	
	call read
	
	pop ebx                     ;stack balance
	pop ebx                     ;stack balance
	pop ebx                     ;stack balance
	cmp eax, 0                  ;check if buffer is empty
	jg flag_switch
	jmp print_buffer

    
flag_switch:
	mov bl, FLAG                ;get flag number
	cmp bl, 0                   ;flag=? 0
	je print
	cmp bl, 1                   ;flag =? 1
	je pre_num_word
	cmp bl, 2                   ;flag =? 2
	je pre_num_word_str
	
print_buffer:
	push dword [ebp-204]            ;prepare file to close our function
	
	call close
	
	pop ebx                         ;stack balance
	mov bl, FLAG 
	cmp bl, 1                       ;flag =? 1
	je print_num_of_word 
	cmp bl, 2                       ;flag =? 2
	je print_num_of_word_str
	jmp return                      ;flag defult
	
print_num_of_word:
	push dword num_words_len            ;prepare int size for write
    push dword num_words                ;prepare char *buf for write
    push dword 1                        ;prepare int fd for write 
    
    call write

    pop ebx                             ;stack balance
    pop ebx                             ;stack balance
    pop ebx                             ;stack balance
	mov ebx, COUNT 
	mov eax, int_str
	push eax                            ;prepare int for utoa_s
	push ebx                            ;prepare char *buf for utoa_s
	
	call utoa_s
	
	pop ebx                             ;stack balance
	pop ebx                             ;stack balance
	push dword 50                       ;prepare int size for write
	push dword int_str                  ;prepare char *buf for  write
	push dword 1                        ;sprepareet int fd for write 
	
	call write
	
	pop ebx                             ;stack balance
	pop ebx                             ;stack balance
	pop ebx                             ;stack balance
	push dword 1                        ;prepare int size for write
    push dword end_of_line              ;prepare char *buf for write
    push dword 1                        ;prepare int fd for write
    
    call write

    pop ebx                             ;stack balance
    pop ebx                             ;stack balance
    pop ebx                             ;stack balance
	jmp return
	
print_num_of_word_str:
	push dword num_str_1_len              ;prepare int size for write
    push dword num_str_1                  ;prepare char *buf for write
    push dword 1                        ;prepare int fd for write 
    
    call write

    pop ebx                             ;stack balance
    pop ebx                             ;stack balance
    pop ebx                             ;stack balance
    push dword 50                       ;prepare int size for write
    push dword cmp_word                 ;prepare char *buf for write
    push dword 1                        ;prepare int fd for write
    
    call write

    pop ebx                             ;stack balance
    pop ebx                             ;stack balance
    pop ebx                             ;stack balance
    push dword num_str_2_len            ;prepare int size for write
    push dword num_str_2                ;prepare char *buf for write
    push dword 1                        ;prepare int fd for write 
    
    call write

    pop ebx                             ;stack balance
    pop ebx                             ;stack balance
    pop ebx                             ;stack balance
	mov ebx, COUNT
	mov eax, int_str
	push eax                            ;prepare int number for function utoa_s
	push ebx                            ;prepare char *buf for function utoa_s
	
	call utoa_s
	
	pop ebx 							;stack balance
	pop ebx 							;stack balance
	push dword 50 						;prepare int size forwrite
	push dword int_str 					;prepare char *buf for write
	push dword 1						;prepare int fd for write (stdout)
	
	call write
	
	pop ebx 							;stack balance
	pop ebx 							;stack balance
	pop ebx 							;stack balance
	push dword 1 						;prepare int size for write
    push dword end_of_line 				;prepare char *buf for write
    push dword 1 						;prepare int fd for write
    
    call write

    pop ebx 							;stack balance
    pop ebx 							;stack balance
    pop ebx 							;stack balance
	jmp return

print:
	push eax 							;prepare int size for write
	mov ebx, ebp
	sub ebx, 200
	push ebx 							;prepare char *buf for write
	push dword 1 						;prepare int fd for write

	call write

	pop ebx 							;stack balance
	pop ebx 							;stack balance
	pop ebx 							;stack balance

	cmp eax, 0 							;check if we got an error
	jle get_error
	jmp file_read


pre_num_word:
	push eax 							;prepare size
	mov ebx, ebp
	sub ebx, 200 						;point to start of buffer
	push ebx
	mov ebx, ebp
	sub ebx, 208
	push ebx 							;prepare counter
	
	call count_words
	
	pop ebx 							;stack balance
	pop ebx 							;stack balance
	pop ebx 							;stack balance
	jmp file_read	

count_words:
	enter 0, 0
	mov ecx, dword [ebp+16] 			;ecx:size
	mov ebx, dword [ebp+12] 			;ebx: start of buffer 
	sub ebx, 1 							;restore before we start the loop
	add ecx, 1 							;restore before we start the loop
	
char_L2:
	call get_char
	
    inc ebx 							;go to our next char
    loop char_L2 						;loop until finish (size 0)
    jmp end_of_count

get_char:
	inc ebx
	dec ecx
	cmp ecx, 0
	je end_of_count 				;finish read
	cmp [ebx], byte 9 					;check if tab
    je get_char
	cmp [ebx], byte 10 					;check if is new line
    je get_char
    cmp [ebx], byte 32 					;check if is space
    je get_char
    jmp found_word 						;start of new word (next char not space)

found_word:
	mov eax, [ebp+8] 					;our counter
    inc dword [eax] 					;add to counter
    jmp go_to_word
	
go_to_word:
	inc ebx
	dec ecx
	cmp ecx, 0
	je end_of_count 						;finish read
	cmp [ebx], byte 9 							;check if is tab
    je get_char
	cmp [ebx], byte 10 							;check if is new line
	je get_char
	cmp [ebx], byte 32 							;check if is space
	je get_char
	jmp go_to_word 								;start of new word (next char not space)
	
end_of_count:
	leave 										;return saved bytes
	ret
	
pre_num_word_str:
	push eax 									;prepare size
	mov ebx, ebp
	sub ebx, 200 								;prepare our pointer to the start of the buffer
	push ebx
	mov ebx, ebp
	sub ebx, 208
	push ebx 									;prepare our counter
	
	call num_of_str_word
	
	pop ebx 									;stack balance
	pop ebx 									;stack balance
	pop ebx 									;stack balance
	jmp file_read	

num_of_str_word:
	enter 0, 0	
	mov ecx, dword [ebp+16] 					;get the size
	mov ebx, dword [ebp+12] 					;get the pointer of the start of buffer
	
str_L3:
	call check_if_we_get_our_word
    inc ebx 									;go to our next char
    loop str_L3 								;go to loop (until size 0)
    jmp end_of_count_string

check_if_we_get_our_word:
	push ebx 									;ebx:str2 for comp
	push dword cmp_word 						;str1 for comp
	
	call comare_txt
	
	pop edx 									;stack balance
	pop edx 									;stack balance
    
	cmp eax, 0 									;check if we found argv[2]
	je find_str
    ret

find_str:
	mov eax, [ebp+8] 							;take our counter
    inc dword [eax] 							;add to counter
	ret
	
end_of_count_string:
	leave 										;return stored bytes
	ret

get_error:
	mov eax, 1 
	leave 										;return stored bytes
	ret

return:
	mov eax, 0
	leave 										;return stored bytes
	ret

read:
	push ebp
	push ebx
	mov	ebp, esp
	mov eax, 3 									; read sys call
	mov ebx, [ebp+12] 							;get first argument
	mov ecx, [ebp+16] 							;get second argument
	mov edx, [ebp+20] 							;get third argument
	int 0x80
	cmp eax, -1
    jle fail_to_read
	jmp FINISH


fail_to_read:
	push dword read_err_len 					;prepare int size for write
    push dword read_err 						;prepare char *buf for write
    push 1 										;prepare int fd for write
    
    call write

    pop ebx 									;stack balance
    pop ebx 									;stack balance
    pop ebx 									;stack balance
    jmp get_error

write:
	push ebp
	push ebx
	mov	ebp, esp
	mov eax, 4 									;write sys call
	mov ebx, [ebp+12] 							;get first argument
	mov ecx, [ebp+16] 							;get second argument
	mov edx, [ebp+20] 							;get third argument
	int 0x80
	cmp eax, -1
    jle fail_to_write
	jmp FINISH
	
fail_to_write:
	push dword write_err_len 					;prepare int size for write
    push dword write_err 						;prepare char *buf for write
    push 1 										;prepare int fd for write 
    
    call write

    pop ebx 									;stack balance
    pop ebx 									;stack balance
    pop ebx 									;stack balance
    jmp get_error

open:
	push ebp
	push ebx
	mov	ebp, esp
	mov eax, 5 									;sys call open
	mov ebx, [ebp+12] 							;get first argument
	mov ecx, [ebp+16] 							;get second argument
	mov edx, 0777 								;carry out all procedures (read, write)
	int 0x80
	cmp eax, -1
    jle fail_to_open
	jmp FINISH
	
fail_to_open:
	push dword open_err_len 					;prepare int size for write
    push dword open_err 						;prepare char *buf for write
    push 1 										;prepare int fd for write 
    
    call write

    pop ebx 									;stack balance
    pop ebx 									;stack balance
    pop ebx 									;stack balance
    jmp get_error

close:
	push ebp
	push ebx
	mov	ebp, esp
	mov eax, 6 									;sys call (close) 
	mov ebx, [ebp+12] 							;get first argument
	int 0x80
	cmp eax, -1
    jle fail_to_close
	jmp FINISH
	
fail_to_close:
	push dword close_err_len 					;prepare int size for write
    push dword close_err 						;prepare char *buf for write
    push 1 										;prepare int fd for write
    
    call write

    pop ebx 									;stack balance
    pop ebx 									;stack balance
    pop ebx 									;stack balance
    jmp get_error

strlen:
	push ebp
	push ebx
	mov	ebp, esp
	mov	eax,-1
	jmp .comp_L

.comp_L:
	add eax, 1 						;add 1 to eax
	mov ebx, eax 					;prepare ebx : start of str input, before reading
	add ebx, [ebp+12] 				;get out next char of str input 
	movzx ebx, BYTE [ebx] 			;zero extends the value of ebx
	test bl,bl 						; bl =? zero char
	jne .comp_L 					;loop (until get zero char)
	jmp FINISH

utoa_s:

    push ebp
	push ebx

	mov	ebp, esp
    mov [ebp-4], dword 10           ;save our divisor
    mov eax, [ebp+12]               ;get number to convert
    mov ebx, [ebp+16]               ;get buffer
    mov [ebp-8], ebx                ;stores buffer address
    mov ebx, [ebp-8]                ;move buffer pointer back


.handele_defult_flag:

	mov edx, 0 						
    div dword [ebp-4] 				;divide our number by 10, eax: quotient, edx: remainder
    mov cl, dl 						;get remainder
    or edx, eax 					
    je .defult_flag
    jmp .write_char
    
.defult_flag:

	add cl, '0'

	mov [ebx], cl 					;put the remainder in buffer
    inc ebx 						;change buffer pointer to next cell
    jmp .end_of_string

.divisor:

    mov edx, 0                      ;store remainder in edx
    div dword [ebp-4]               ;divide our number by the divisor, eax: stores quotient, edx: store remainder
    mov cl, dl                      ;give us the remainder

    or edx, eax                     ;check if edx = eax

    jne .write_char                 ;if are not equal write our char
    jmp .end_of_string             ;if equal write and finish

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
		
compare_str:
	push ebp
	push ebx
	mov	ebp, esp
	mov	eax,-1 						;eax:index of start read

.L4:
	add eax, 1 						;increase eax
	mov ebx, eax 					; ebx : start of str1 before read
	mov ecx, eax 					; ebx :start of str2 before read

	add ebx, [ebp+12] 				;take next in string1
	add ecx, [ebp+16] 				;take next in string2

	movzx ebx, BYTE [ebx] 			;zero extends the value of ebx
	movzx ecx, BYTE [ecx] 			;zero extends the value of ecx

	test bl,bl 						;bl =? zero char
	je .compare 					;get the end of str1
	test cl,cl 						;cl =? zero char
	je .compare 					;get the end of str2
	cmp ebx, ecx 					; char1 =? char2 
	jne .compare 					;chars are different 
	jmp .L4 						;loop (no find diff)
	
.compare:

	mov eax, ebx 					;eax: is curr char where we finish the loop
	sub eax, ecx 					;compare lasts char
	jmp FINISH 						
	
buffer_load:
    push ebp
	push ebx
	mov	ebp, esp
    mov eax, [ebp+12] 				;ste arg
    mov ebx, [ebp+16] 				;buffer arg
    mov [ebp-8], ebx 				;buffer address
    mov ebx, [ebp-8] 				;move buffer pointer to the start

.char_load:
	movzx ecx, byte [eax]
    mov [ebx], cl 					;write to buffer our char
    inc eax 						;change the str pointer to next (cell)
    inc ebx 						;change the buffer pointer to next (cell)
    test cl, cl 					;end of string
    je .the_str_end
    jmp .char_load

.the_str_end:
    mov [ebx], byte 0 					;close str (\0) 
    jmp FINISH
    
comare_txt:
	push ebp
	push ebx
	mov	ebp, esp
	mov	eax,-1 							;eax: index of start reading


.L5:
	add eax, 1 						;increase eax
	mov ebx, eax 					; ebx : start of str1 before read
	mov ecx, eax 					; ebx :start of str2 before read

	add ebx, [ebp+12] 				;take next in string1
	add ecx, [ebp+16] 				;take next in string2

	movzx ebx, BYTE [ebx] 			;zero extends the value of ebx
	movzx ecx, BYTE [ecx] 			;zero extends the value of ecx

	test bl,bl 						;bl =? zero char
	je .word_in_text 			;get the end of str1
	test cl,cl 						;cl =? zero char
	je .word_not_in_text	;get the end of str2
	cmp ebx, ecx 					; char1 =? char2 
	jne .word_not_in_text 	;chars are different 
	jmp .L5 						;loop (no find diff)

.word_in_text:
	mov eax, dword 0 ;string was found
	jmp FINISH ;jump to the end of the program

.word_not_in_text:
	mov eax, dword -1 ;string wasn't found
	jmp FINISH ;jump to the end of the program

FINISH:
	mov esp, ebp
	pop ebx
	pop ebp
	ret
