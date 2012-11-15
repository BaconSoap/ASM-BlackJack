.data
	names BYTE ?
	nameLength DWORD 5 DUP(0)
	banks DWORD 5 DUP(300)
	playerCount DWORD 1
	countPrompt BYTE "Enter the number of players: ", 0
	namePrompt BYTE "Enter the name of player ", 0

.code



nameInput PROC USES EAX EBX ECX EDX
	mov edx, offset countPrompt
	call WriteString
	call ReadInt
	mov playerCount, eax

	mov ebx, 1
	
	mov ecx, playerCount
	inputLoop:
		mov edx, offset namePrompt
		call WriteString
		mov eax, ebx
		call WriteInt
		mov eax, ' '
		call WriteChar
		push ecx
		mov edx, offset names
		mov ecx, 100
		call ReadString
		mov [nameLength], eax
		inc ebx
		pop ecx
	loop inputLoop

	RET
nameInput ENDP

;displays player and bank pass eax player 
display PROC 
	
	mov edi, offset names
	mov esi, offset banks

	cmp eax, 1
	je outPlayer

	mov ebx, offset nameLength
	mov ecx, eax
	dec ecx
	mov edx, 0
	nameShift:
		add edx, [ebx]
	loop nameShift 

	add edi, [edx]	;edi name

	mov ecx, eax
	dec ecx
	multiply:
		add eax, 4
	loop multiply

	
	add esi, eax ;esi bank

outPlayer:

	mov edx, edi
	call WriteString
	call crlf
	mov eax, [esi]
	call WriteInt
	call crlf

	RET
display ENDP


;update bank pass eax player number and ebx amount to change updates bank array
deposit PROC
	
	cmp eax, 1
	je addtobank

	mov ecx, eax
	dec ecx
	multiply:
		add eax, 4
	loop multiply


	mov esi, offset banks
	add esi, eax

addtobank:
	mov esi, offset banks
	add [esi], ebx

	RET
deposit ENDP
