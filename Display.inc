.data
	CurrentPlayerColor	DB	2
	OtherPlayerColor	DB	15
	CursorX				DB	0
	CursorY				DB	0
	BaseX				DB	0
	BetDisplay			DB	"Bet: ", 0
	BankDisplay			DB	"Balance: ", 0

.code
SetCurrentColor PROC USES EAX
	MOVZX EAX, CurrentPlayerColor
	CALL SetTextColor
	RET
SetCurrentColor ENDP

SetOtherColor PROC USES EAX
	MOVZX EAX, OtherPlayerColor
	CALL SetTextColor
	RET
SetOtherColor ENDP

DisplayAll PROC USES EAX ECX ESI EDX
	CALL SetOtherColor
	CALL ClrScr
	;CURRENTPLAYER
	MOV ECX, 0
	MOV ESI, 0
	
	MOV CursorX, 0
	MOV CursorY, 0
	MOV BaseX, 0
	CALL MoveCursor

	PlayerLoop:
		CALL SetOtherColor
		MOVZX EAX, CurrentPlayer
		CMP ECX, EAX
		JNE ShowPlayer
		CALL SetCurrentColor
		JMP ShowPlayer

		ShowPlayer:
		MOV AL, CursorX
		MOV BaseX, AL
		;Show Name
			LEA EDX, (PlayerStructure PTR PlayerArray[ESI]).PName ;Load the address of PNAME to display
			CALL WriteString
		
			CALL SetToBaseX
		;Show Bank
			MOV EDX, OFFSET BankDisplay
			CALL WriteString
			MOV EAX, (PlayerStructure PTR PlayerArray[ESI]).BankAcc
			CALL WriteDec

			CALL SetToBaseX
		;Show Bet
			MOV EDX, OFFSET BetDisplay
			CALL WriteString
			MOV EAX, (PlayerStructure PTR PlayerArray[ESI]).Bet
			CALL WriteDec
			
		;ShowCards
			CALL SetToBaseX
			CALL ShowPlayerCards
		;Tab over and reset Y value
			ADD BaseX, 23
			MOV AL, BaseX
			MOV CursorX, AL
			MOV CursorY, 0
			CALL MoveCursor
		;Move to next player
		ADD ESI, TYPE PlayerStructure
		INC ECX
		CMP ECX, PlayerCount
	JL PlayerLoop
	MOV CursorX, 0
	MOV CursorY, 15
	CALL MoveCursor
	CALL SetOtherColor
	RET
DisplayAll ENDP

ShowPlayerCards PROC USES EAX ECX ESI EDX
	MOV EAX, ECX
	INC EAX
	
	call getplayerhand
	mov esi, offset handBuffer

	testit:
	mov eax, 0
	mov al, [esi]
	cmp al, 52
	JE endit
	call getcarddisplay
	mov edx, offset carddisplaystring
	call writestring
	call SetToBaseX
	;add esi, 4
	inc esi
	;call dumpregs

	jmp testit

endit:
	call checkSplit

	RET
ShowPlayerCards ENDP

ShowDealerCards PROC

ShowDealerCards ENDP

SetToBaseX PROC USES EAX
	INC CursorY
	MOV AL, BaseX
	MOV CursorX, AL
	CALL MoveCursor
	RET
SetToBaseX ENDP

MoveCursor PROC USES EDX
	MOV DH, CursorY
	MOV DL, CursorX
	CALL Gotoxy
	RET
MoveCursor ENDP