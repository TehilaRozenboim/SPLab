section .text
	global open_file ;make open_file transparent to other modules
	global close_file ;make open_file transparent to other modules

open_file:
	push ebp ;stack maintenance
	push ebx
	mov ebp, esp ;stack maintenance
	
open_sys_call:
	mov eax, 5 ;Sys_open system call number
	mov ebx, [ebp+12] ;retrieves the first function argument
	mov ecx, 0 ;read only
	mov edx, 0777 ;read, write and execute by all
	int 0x80
	mov [fd_in], eax ;save file pointer in the bss
	jmp FINISH
	
close_file:
	push ebp ;stack maintenance
	push ebx
	mov ebp, esp ;stack maintenance

close_file_sys_call:
	mov eax, 6 ;Sys_close_file system call number
	mov ebx, [fd_in] ;retrieves the first function argument
	int 0x80
	jmp FINISH
	
FINISH:
	mov esp, ebp ;stack maintenance
	pop ebx
	pop ebp ;stack maintenance
	ret ;stack maintenance
	
section .bss
	fd_in resb 1
