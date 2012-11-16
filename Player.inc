.data
	
	banks DWORD 5 DUP(300)
	bet DWORD 5 DUP(0)
	playerCount DWORD 1
	countPrompt BYTE "Enter the number of players: ", 0
	namePrompt BYTE "Enter the name of player ", 0
	name1 BYTE 30 DUP(0)
	name2 BYTE 30 DUP(0)
	name3 BYTE 30 DUP(0)
	name4 BYTE 30 DUP(0)
	name5 BYTE 30 DUP(0)

.code

;Start of game input number of players and names of each
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
		call WriteDec
		mov eax, ' '
		call WriteChar

		cmp ebx, 1
		JE inName1
		cmp ebx, 2
		JE inName2
		cmp ebx, 3
		JE inName3
		cmp ebx, 4
		JE inName4
		cmp ebx, 5
		JE inName5

		inName1:
			mov edx, offset name1
			jmp inswitchend
		inName2:
			mov edx, offset name2
			jmp inswitchend
		inName3:
			mov edx, offset name3
			jmp inswitchend
		inName4:
			mov edx, offset name4
			jmp inswitchend
		inName5:
			mov edx, offset name5
			jmp inswitchend

		inSwitchEnd:

		push ecx
		mov ecx, SIZEOF name1
		call ReadString

		inc ebx
		pop ecx
	loop inputLoop

	RET
nameInput ENDP

;displays player and bank pass eax player number 1-5
display PROC USES EBX ECX EDX ESI

	cmp eax, 1
	JE inName1
	cmp eax, 2
	JE inName2
	cmp eax, 3
	JE inName3
	cmp eax, 4
	JE inName4
	cmp eax, 5
	JE inName5

	inName1:
		mov edx, offset name1
		mov esi, offset banks
		push eax
		jmp outputPlayer
	inName2:
		mov edx, offset name2
		jmp switchend
	inName3:
		mov edx, offset name3
		jmp switchend
	inName4:
		mov edx, offset name4
		jmp switchend
	inName5:
		mov edx, offset name5
		jmp switchend

switchend:
	mov ebx, 0
	mov ecx, eax
	dec ecx
	push eax

	multiply:
		add ebx, 4
	loop multiply

	mov esi, offset banks
	add esi, ebx

outputPlayer:

	mov eax, [esi]
	call WriteString
	call crlf
	call WriteDec
	call crlf

	pop eax

	RET
display ENDP


;update bank pass eax player number and ebx amount to change updates bank array
deposit PROC
	
	mov esi, offset banks

	cmp eax, 1
	JE addtobank

	mov ecx, eax
	mov edx, 0
	dec ecx
	multiply:
		add edx, 4
	loop multiply

	add esi, edx

addtobank:
	
	add [esi], ebx

	RET
deposit ENDP

;update bank pass eax player number and ebx amount to change updates bank array
withdrawl PROC
	
	mov esi, offset banks

	cmp eax, 1
	JE removefrombank

	mov ecx, eax
	mov edx, 0
	dec ecx
	multiply:
		add edx, 4
	loop multiply

	add esi, edx

removefrombank:
	
	sub [esi], ebx

	RET
withdrawl ENDP


;pass player number in EAX and bet amount in EBX
inputBet PROC
	mov esi, offset bet

	cmp eax, 1
	JE addbet

	mov ecx, eax
	mov edx, 0
	dec ecx
	multiply:
		add edx, 4
	loop multiply

	add esi, edx

addbet:
	mov [esi], ebx

	call withdrawl

	RET
inputBet ENDP

;for double down send player number in EAX
doubleBet PROC
	mov esi, offset bet

	cmp eax, 1
	JE addbet

	mov ecx, eax
	mov edx, 0
	dec ecx
	multiply:
		add edx, 4
	loop multiply

	add esi, edx

addbet:
	mov ebx, [esi]
	add [esi], ebx
	call withdrawl
	RET
doubleBet ENDP

;send the winning player number in EAX
winBet PROC
	mov esi, offset bet

	cmp eax, 1
	JE winningBet

	mov ecx, eax
	mov edx, 0
	dec ecx
	multiply:
		add edx, 4
	loop multiply

	add esi, edx

winningBet:
	mov ebx, [esi]
	call deposit
	call deposit
	RET
winBet ENDP