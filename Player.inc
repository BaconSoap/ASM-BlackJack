.data
	
	playerCount DWORD 1
	countPrompt BYTE "Enter the number of players: ", 0
	namePrompt BYTE "Enter the name of player ", 0
	betPrompt BYTE "Enter the bet for ", 0
	betPrompt2 BYTE ": ", 0

	tooFewPeople BYTE "Too few players (1 min).",0Ah,0
	tooManyPeople BYTE "Too many players (5 max).",0Ah,0
	NameTooLong BYTE "Name is too long, try again: ",0
	NameTooShort BYTE "You must enter a name: ",0
	TenDollarMinimum BYTE "There is $10 bet minimum. Try again: ",0
	ProspectiveBet DWORD 0 ; http://support.microsoft.com/kb/74666
	NotEnoughInBank BYTE "Not enough money in the bank. Bet less: ",0
	CantBet BYTE "Can't bet, not enough money in the bank.",0Ah,0

.code

setPlayerColor PROC USES eax

	RET
setPlayerColor ENDP

setDefaultTxtColor PROC USES eax

	RET
setDefaultTxtColor ENDP

;displays all players in game
outputPlayers PROC
	mov ecx, playerCount
	mov eax, 1
	outloop:
		call setPlayerColor
		call display
		call Testhand
		call crlf
		inc eax
		call setDefaultTxtColor
	loop outloop

	RET
outputPlayers ENDP

;Start of game input number of players and names of each
nameInput PROC USES EAX EBX ECX EDX ESI
	getHeadCount:
	mov edx, offset countPrompt
	call WriteString
	call ReadInt
	mov playerCount, eax
	.IF SDWORD PTR eax < 1
		mov edx,OFFSET tooFewPeople
		call WriteString
		jmp getHeadCount
	.ELSEIF eax > 5
		mov edx,OFFSET tooManyPeople
		call WriteString
		jmp getHeadCount
	.ENDIF

	mov ebx, 1

	mov ecx, playercount

	MOV edi, 0

	NameLoop:
		call setPlayerColor
			mov edx, offset namePrompt
			call WriteString
			mov eax, ebx
			mov eax, ' '
			call WriteChar
		push ecx
		lea EDX, (Playerstructure PTR playerarray[edi]).pname
		mov ecx, SIZEOF (Playerstructure PTR playerarray[edi]).pname
		tryNameAgain:
		call ReadString
		.IF eax > 21
			push edx
			push ecx
			mov edx,OFFSET NameTooShort
			call WriteString
			pop ecx
			pop edx
			jmp tryNameAgain
		.ELSEIF SDWORD PTR eax < 1
			push edx
			push ecx
			mov edx,OFFSET NameTooShort
			call WriteString
			pop ecx
			pop edx
			jmp tryNameAgain
		.ENDIF
		inc ebx
		call setDefaultTxtColor
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
	TryBetAgain:
	mov eax,0
	call readInt
	mov ebx,eax
	add eax,((PlayerStructure PTR PlayerArray[ESI]).bet)
	mov ProspectiveBet,eax
	mov eax,0
	mov eax,((PlayerStructure PTR PlayerArray[ESI]).BankAcc)
	.IF ProspectiveBet > eax && SDWORD PTR ebx < 10
		mov edx,OFFSET CantBet
		call WriteString
		;; FIXME go back to menu
		ret
	.ELSEIF SDWORD PTR ebx < 10
		mov edx,OFFSET TenDollarMinimum
		call WriteString
		jmp TryBetAgain
	.ELSEIF ProspectiveBet > eax ; if current bet + prospective bet is greater than bank, try again.
		mov edx,OFFSET NotEnoughInBank
		call WriteString
		jmp TryBetAgain
	.ENDIF
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