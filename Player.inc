.data
	
	playerCount DWORD 1
	countPrompt BYTE "Enter the number of players: ", 0
	namePrompt BYTE "Enter the name of player ", 0
	betPrompt BYTE "Enter the bet for ", 0
	betPrompt2 BYTE ": ", 0

.code

;displays all players in game
outputPlayers PROC
	mov ecx, playerCount
	mov eax, 1
	outloop:
		call display
		call Testhand
		call crlf
		inc eax
	loop outloop

	RET
outputPlayers ENDP

;Start of game input number of players and names of each
nameInput PROC USES EAX EBX ECX EDX ESI
	mov edx, offset countPrompt
	call WriteString
	call ReadInt
	mov playerCount, eax

	mov ebx, 1

	mov ecx, playercount

	MOV edi, 0

	NameLoop:
			mov edx, offset namePrompt
			call WriteString
			mov eax, ebx
			mov eax, ' '
			call WriteChar

			lea EDX, (Playerstructure PTR playerarray[edi]).pname
		push ecx
		mov ecx, SIZEOF (Playerstructure PTR playerarray[edi]).pname
		call ReadString
		inc ebx
		pop ecx
		add edi, TYPE Playerstructure
	loop NameLoop
			

	RET
nameInput ENDP

;displays player name, bank, and bet | pass eax player number 1-5
display PROC USES EBX ECX EDX ESI EAX EDI
		
		mov esi, 0
			;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
		.while (eax > 1)
			add esi, TYPE Playerstructure
			dec eax
		.endw
		
		lea EDX, (Playerstructure PTR playerarray[esi]).pname
		Mov EAX, (Playerstructure PTR playerarray[esi]).bankacc
		Mov EBX, (Playerstructure PTR playerarray[esi]).bet
			
		printinfo:
			CALL WriteString
			CALL Crlf
			CALL WriteDec
			CALL Crlf
			MOV EAX, EBX
			CALL WriteDec
			CALL Crlf


	RET
display ENDP

;update bank pass eax player number (1-5) and ebx amount to change update bank
deposit PROC USES ESI EAX EBX

	mov esi, 0
		
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
	;add ebx to players bank account
	ADD (Playerstructure PTR playerarray[esi]).bankacc, EBX
	
	RET
deposit ENDP

;update bank pass eax player number and ebx amount to change update bank
withdraw PROC USES ESI EAX EBX

	mov esi, 0
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
	;sub ebx to players bank account
	SUB (Playerstructure PTR playerarray[esi]).bankacc, EBX
	
	RET
withdraw ENDP

;pass player number in EAX(1-5) withdraws money from account and places in bet
PlaceBet PROC USES ECX EDX ESI
	
	push eax
	mov esi, 0
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
	pop eax

	;betting prompt for players
	mov edx, offset betprompt
	call writeString
	lea edx, (Playerstructure PTR playerarray[esi]).Pname
	call writeString
	mov edx, offset betprompt2
	call writestring
	push eax
	call readInt
	mov ebx, eax
	pop eax
	;---------------------------

	;add ebx to players bet
	ADD (Playerstructure PTR playerarray[esi]).bet, EBX
	;withdraw amount from players bank | pops player number from eax so it withdraws from correct player	
	CALL withdraw
	
	RET
PlaceBet ENDP

;for double down send player number in EAX
doubleBet PROC USES EAX ESI EBX
	mov esi, 0
	push eax
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
	;double down doubles your bet 
	MOV EBX, (Playerstructure PTR playerarray[esi]).bet
	ADD (Playerstructure PTR playerarray[esi]).bet, EBX
	;withdraw amount from players bank | pops player number from eax so it withdraws from correct player
	pop eax
	CALL withdraw

	RET
doubleBet ENDP

;for double down send player number in EAX
doubleSplitBet PROC USES EAX ESI EBX
	mov esi, 0
	push eax
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
	;double down doubles your bet 
	MOV EBX, (Playerstructure PTR playerarray[esi]).Splitbet
	ADD (Playerstructure PTR playerarray[esi]).bet, EBX
	;withdraw amount from players bank | pops player number from eax so it withdraws from correct player
	pop eax
	CALL withdraw

	RET
doubleSplitBet ENDP


;send the winning player number in EAX
winBet PROC USES ESI EAX EBX
	
	
	mov esi, 0
	push eax
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
	;sub ebx to players bank account
	MOV EBX, (Playerstructure PTR playerarray[esi]).bet
	;calls deposit twice so it replaces the initial bet and then the amount won
		pop eax
		CALL deposit
		CALL deposit
	;sets bet to 0
	MOV (Playerstructure PTR playerarray[esi]).bet, 0
	RET
winBet ENDP

;send the winning player number in EAX
winSplitBet PROC USES ESI EAX EBX
	
	
	mov esi, 0
	push eax
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
	;sub ebx to players bank account
	MOV EBX, (Playerstructure PTR playerarray[esi]).Splitbet
	;calls deposit twice so it replaces the initial bet and then the amount won
		pop eax
		CALL deposit
		CALL deposit
	;sets bet to 0
	MOV (Playerstructure PTR playerarray[esi]).Splitbet, 0
	RET
winSplitBet ENDP

;send the losing player number in EAX
loseBet PROC USES ESI EAX EBX
	
	mov esi, 0
	push eax
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
	;sets bet to 0
	MOV (Playerstructure PTR playerarray[esi]).bet, 0
	RET
loseBet ENDP

pushBet PROC USES ESI EAX EBX
	mov esi, 0
	push eax
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
	;sub ebx to players bank account
	MOV EBX, (Playerstructure PTR playerarray[esi]).bet
	;calls deposit twice so it replaces the initial bet and then the amount won
		pop eax
		CALL deposit
	;sets bet to 0
	MOV (Playerstructure PTR playerarray[esi]).bet, 0
	RET
pushBet ENDP

pushSplitBet PROC USES ESI EAX EBX
	mov esi, 0
	push eax
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
	;sub ebx to players bank account
	MOV EBX, (Playerstructure PTR playerarray[esi]).Splitbet
	;calls deposit twice so it replaces the initial bet and then the amount won
		pop eax
		CALL deposit
	;sets bet to 0
	MOV (Playerstructure PTR playerarray[esi]).Splitbet, 0
	RET
pushSplitBet ENDP