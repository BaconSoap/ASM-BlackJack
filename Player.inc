.data
	names BYTE 5 DUP(?)
	banks DWORD 5 DUP(300)
	playerCount DWORD 1
	countPrompt BYTE "Enter the number of player: ", 0
	namePrompt BYTE "Enter the name of player ", 0

.code



nameInput PROC USES EAX ECX EDX
	mov eax, 1
	mov edx, offset namePrompt
	mov ecx, playerCount
	inputLoop:
		call WriteString
		call WriteInt
		call ReadString
		inc eax
	loop inputLoop

	RET
nameInput ENDP