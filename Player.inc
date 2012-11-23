.data
	
	playerCount DWORD 1
	countPrompt BYTE "Enter the number of players: ", 0
	namePrompt BYTE "Enter the name of player ", 0
	betPrompt BYTE "Enter the bet for player ", 0
	betPrompt2 BYTE ": ", 0

.code


;displays all players in game
outputPlayers PROC
	mov eax, 1
	mov ecx, playerCount
	outloop:
		call display
		inc eax
	loop outloop

	RET
outputPlayers ENDP

;Start of game input number of players and names of each
nameInput PROC USES EAX EBX ECX EDX
	mov edx, offset countPrompt
	call WriteString
	call ReadInt
	mov playerCount, eax

	mov ebx, 1

	mov ecx, playercount
	NameLoop:
			mov edx, offset namePrompt
			call WriteString
			mov eax, ebx
			call WriteDec
			mov eax, ' '
			call WriteChar

		inName1:
			cmp ebx, 1
			JNE inName2
			mov EDX, offset Player1.Pname
			JMP inputName
		inName2:
			cmp ebx, 2
			JNE inName3
			mov EDX, offset Player2.Pname
			JMP inputName
		inName3:
			cmp ebx, 3
			JNE inName4
			mov EDX, offset Player3.Pname
			JMP inputName
		inName4:
			cmp ebx, 4
			JNE inName5
			mov EDX, offset Player4.Pname
			JMP inputName
		inName5:
			cmp ebx, 5
			mov EDX, offset Player5.Pname
			JMP inputName

		inputName:
		push ecx
		mov ecx, SIZEOF Player1.Pname
		call ReadString

		inc ebx
		pop ecx
	loop NameLoop
			

	RET
nameInput ENDP

;displays player and bank pass eax player number 1-5
display PROC USES EBX ECX EDX ESI EAX


		Pstats1:
			cmp eax, 1
			JNE Pstats2
			mov EDX, OFFSET Player1.pname
			mov EAX, Player1.Bankacc
			JMP printinfo
		Pstats2:
			cmp eax, 2
			JNE Pstats3
			mov EDX, OFFSET Player2.pname
			mov EAX, Player2.Bankacc
			JMP printinfo
		Pstats3:
			cmp eax, 3
			JNE Pstats4
			mov EDX, OFFSET Player3.pname
			mov EAX, Player3.Bankacc
			JMP printinfo
		Pstats4:
			cmp eax, 4
			JNE Pstats5
			mov EDX, OFFSET Player4.pname
			mov EAX, Player4.Bankacc
			JMP printinfo
		Pstats5:
			mov EDX, OFFSET Player5.pname
			mov EAX, Player5.Bankacc
			JMP printinfo
			
		printinfo:
			CALL WriteString
			CALL Crlf
			CALL WriteDec
			CALL Crlf


	RET
display ENDP

;update bank pass eax player number and ebx amount to change update bank
deposit PROC

	Pdeposit1:
			cmp eax, 1
			JNE Pdeposit2
			ADD Player1.Bankacc, EBX
			JMP depositend
		Pdeposit2:
			cmp eax, 2
			JNE Pdeposit3
			ADD Player2.Bankacc, EBX
			JMP depositend
		Pdeposit3:
			cmp eax, 3
			JNE Pdeposit4
			ADD Player3.Bankacc, EBX
			JMP depositend
		Pdeposit4:
			cmp eax, 4
			JNE Pdeposit5
			ADD Player4.Bankacc, EBX
			JMP depositend
		Pdeposit5:
			ADD Player5.Bankacc, EBX
		
		depositend:
	
	RET
deposit ENDP

;update bank pass eax player number and ebx amount to change update bank
withdraw PROC

	Pwithdraw1:
			cmp eax, 1
			JNE Pwithdraw2
			SUB Player1.Bankacc, EBX
			JMP withdrawend
		Pwithdraw2:
			cmp eax, 2
			JNE Pwithdraw3
			SUB Player2.Bankacc, EBX
			JMP withdrawend
		Pwithdraw3:
			cmp eax, 3
			JNE Pwithdraw4
			SUB Player3.Bankacc, EBX
			JMP withdrawend
		Pwithdraw4:
			cmp eax, 4
			JNE Pwithdraw5
			SUB Player4.Bankacc, EBX
			JMP withdrawend
		Pwithdraw5:
			SUB Player5.Bankacc, EBX
		
		withdrawend:
	
	RET
withdraw ENDP

;pass player number in EAX withdraws money from account and places in bet
PlaceBet PROC USES ECX EDX ESI
	mov edx, offset betprompt
	call writeString
	call writedec
	mov edx, offset betprompt2
	call writeString
	push eax
	call readInt
	mov ebx, eax
	pop eax

	Pbetting1:
			cmp eax, 1
			JNE Pbetting2
			ADD Player1.Bet, EBX
			JMP bettingend
		Pbetting2:
			cmp eax, 2
			JNE Pbetting3
			ADD Player2.Bet, EBX
			JMP bettingend
		Pbetting3:
			cmp eax, 3
			JNE Pbetting4
			ADD Player3.Bet, EBX
			JMP bettingend
		Pbetting4:
			cmp eax, 4
			JNE Pbetting5
			ADD Player4.Bet, EBX
			JMP bettingend
		Pbetting5:
			ADD Player5.Bet, EBX
		
		bettingend:
		CALL withdraw
	RET
PlaceBet ENDP

;for double down send player number in EAX
doubleBet PROC
	
	Pdouble1:
			cmp eax, 1
			JNE Pdouble2
			ADD Player1.Bet, EBX
			JMP doubleend
		Pdouble2:
			cmp eax, 2
			JNE Pdouble3
			ADD Player2.Bet, EBX
			JMP doubleend
		Pdouble3:
			cmp eax, 3
			JNE Pdouble4
			ADD Player3.Bet, EBX
			JMP doubleend
		Pdouble4:
			cmp eax, 4
			JNE Pdouble5
			ADD Player4.Bet, EBX
			JMP doubleend
		Pdouble5:
			ADD Player5.Bet, EBX
		
		doubleend:
		CALL withdraw
	RET
doubleBet ENDP

;send the winning player number in EAX
winBet PROC
	
	Pwon1:
			cmp eax, 1
			JNE Pwon2
			mov ebx, Player1.bet
			JMP wonend
		Pwon2:
			cmp eax, 2
			JNE Pwon3
			mov ebx, Player2.bet
			JMP wonend
		Pwon3:
			cmp eax, 3
			JNE Pwon4
			mov ebx, Player3.bet
			JMP wonend
		Pwon4:
			cmp eax, 4
			JNE Pwon5
			mov ebx, Player4.bet
			JMP wonend
		Pwon5:
			mov ebx, Player5.bet
		
		wonend:
		CALL deposit
		CALL deposit
	RET
winBet ENDP
