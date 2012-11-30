.data
;using to test
;DealerHand DB 1,3, 51, 52
DEALERCARDCOUNT DB 2
total Dword ?
aceCount DWord ?
handType BYTE 'H'


.code


;Gets the dealer hand and puts it into HandBuffer
GetDealerHand PROC USES EAX ESI EDI ECX
	
;	MOVZX ECX, Dealer.CardCount
	;mov ecx, 3
	MOV ESI, OFFSET Dealer.Phand
	;mov esi, offset dealerhand
	MOV EDI, OFFSET HandBuffer
StartCopy:
		MOV EAX, [ESI]
		MOV [EDI], EAX

		INC ECX
		INC ESI
		INC EDI

		CMP ECX, HAND_SIZE
		JNE StartCopy
RET
GetDealerHand ENDP

;Places HandBuffer into the dealer hand
SetDealerHand PROC USES EAX ESI EDI ECX EBX
	MOV ECX, 0
	MOV EBX, 0 
	MOV EDI, OFFSET Dealer.Phand
	MOV ESI, OFFSET HandBuffer
	StartCopy:
	MOV EAX, [ESI]
	MOV [EDI], EAX

	INC ECX
	INC ESI
	INC EDI
	INC EBX
	CMP ECX, HAND_SIZE
	JNE StartCopy
	;MOV Dealer.CardCount, bl
	RET
SetDealerHand ENDP


;pass player number in EAX places that players hand into handbuffer
getPlayerHand PROC USES ECX ESI EDI EAX EBX
	mov ecx, 0
	mov edi, offset handBuffer
	mov esi, 0
	mov ebx, 0
	push eax	
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add ebx, TYPE Playerstructure
		dec eax
	.endw
	lea esi, (Playerstructure PTR playerarray[ebx]).Phand 
	pop eax

	
	getHand:
	push eax

	copyGet:
	MOV al, [ESI]
	MOV [EDI], al

	INC ECX
	INC ESI
	INC EDI
	CMP ECX, HAND_SIZE
	JNE copyGet

	pop eax
		
	RET
getPlayerHand ENDP

;pass player number in EAX places that players hand into handbuffer
getPlayerSplitHand PROC USES ECX ESI EDI EAX EBX
	mov ecx, 0
	mov edi, offset handBuffer
	mov esi, 0
	mov ebx, 0
	push eax	
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add ebx, TYPE Playerstructure
		dec eax
	.endw
	lea esi, (Playerstructure PTR playerarray[ebx]).SplitHand
	pop eax

	
	getSplitHand:
	push eax

	copySplitGet:
	MOV al, [ESI]
	MOV [EDI], al

	INC ECX
	INC ESI
	INC EDI
	CMP ECX, HAND_SIZE
	JNE copySplitGet

	pop eax
		
	RET
getPlayerSplitHand ENDP

;pass player number in EAX places handbuffer into that players hand
setPlayerHand PROC USES ECX EDI ESI EAX EBX
	mov ecx, 0
	mov edi, offset handBuffer
	mov esi, 0
	mov ebx, 0
	push eax	
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add ebx, TYPE Playerstructure
		dec eax
	.endw
	lea esi, (Playerstructure PTR playerarray[ebx]).Phand 
	pop eax

	
	SetHand:
	push eax

	copySet:
	MOV al, [EDI]
	MOV [ESI], al

	INC ECX
	INC ESI
	INC EDI
	CMP ECX, HAND_SIZE
	JNE copySet

	pop eax
		
	RET
setPlayerHand ENDP

;pass player number in EAX places handbuffer into that players hand
setPlayerSplitHand PROC USES ECX EDI ESI EAX EBX
	mov ecx, 0
	mov edi, offset handBuffer
	mov esi, 0
	mov ebx, 0
	push eax	
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add ebx, TYPE Playerstructure
		dec eax
	.endw
	lea esi, (Playerstructure PTR playerarray[ebx]).SplitHand
	pop eax

	
	SetSplitHand:
	push eax

	copySplitSet:
	MOV al, [EDI]
	MOV [ESI], al

	INC ECX
	INC ESI
	INC EDI
	CMP ECX, HAND_SIZE
	JNE copySplitSet

	pop eax
		
	RET
setPlayerSplitHand ENDP

;pass player number in EAX and card to add in EBX
addCardPlayer PROC USES ECX ESI EAX
	call getPlayerHand
	mov esi, offset handbuffer

one:

	mov cl, [esi]
	cmp cl, 52
	JE endadd
	inc esi
	jmp one

endadd:
	mov [esi], bl
	call setPlayerHand

	mov esi, 0
			;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
		
	inc (Playerstructure PTR playerarray[esi]).cardCount


	RET
addCardPlayer ENDP

;pass player number in EAX and card to add in EBX
addCardPlayerSplit PROC USES ECX ESI EAX
	call getPlayerSplitHand
	mov esi, offset handbuffer

one:

	mov cl, [esi]
	cmp cl, 52
	JE endadd
	inc esi
	jmp one

endadd:
	mov [esi], bl
	call setPlayerSplitHand

	RET
addCardPlayerSplit ENDP

; card to add in EBX
addCardDealer PROC USES ECX ESI EAX
	call getDealerHand
	mov esi, offset handbuffer

one:
	mov cl, [esi]
	cmp cl, 52
	JE endadd
	inc esi
	jmp one
endadd:
	mov [esi], bl
	call setDealerHand

	inc dealer.cardCount

	RET
addCardDealer ENDP



;Takes handbuffer and returns score for it
;Outputs player's score in EAX
GetHandValueNew PROC USES ESI 
	
	;CALL getPlayerHand
	MOV ESI, OFFSET HandBuffer
	MOV total, 0
	MOV aceCount, 0

	
	StartFirstLoop:
	;call dumpregs
		MOV EAX, 0
		MOV AL, [ESI]
		CMP EAX, 52

		JE StartAceLoop
		CALL getCardNumber

		CMP EAX, 0
		JE IHateAces

		CMP EAX, 10
		JGE ClampToTen
		INC EAX
		JMP AddToTotal
		IHateAces:
			INC aceCount
			MOV EAX, 11
			JMP AddToTotal

		ClampToTen:
			MOV EAX, 10
			JMP AddToTotal

		AddToTotal:
			ADD total, EAX
			INC ESI
			JMP StartFirstLoop

	StartAceLoop:
		CMP aceCount, 0
		JLE EndOfProc
		;IF Total > 21 Subtract 10 from total
		CMP total, 21
		JG SubtractTen
		DEC aceCount
		JMP StartAceLoop
		SubtractTen:
		SUB total, 10
		DEC aceCount
		JMP StartAceLoop
		
	EndOfProc:
	MOV EAX, total

	RET
GetHandValueNew ENDP

showdealerHand PROC
	pushad
	mov edx, offset dealer.pname
	call writestring
	call crlf
	call getdealerhand
	mov esi, offset handBuffer

loopen:
	mov eax, 0
	mov al, [esi]
	cmp al, 52
	JE endstuff
	call getcarddisplay
	mov edx, offset carddisplaystring
	call writestring
	call crlf
	inc esi

	jmp loopen

	endstuff:

	popad
	RET
showdealerHand ENDP




testHand PROC USES ESI EAX EDX
	pushad
	call crlf
	call getplayerhand
	mov esi, offset handBuffer
	call crlf
	call crlf
testit:
	mov eax, 0
	mov al, [esi]
	cmp al, 52
	JE endit
	call getcarddisplay
	mov edx, offset carddisplaystring
	call writestring
	call crlf
	;add esi, 4
	inc esi
	;call dumpregs

	jmp testit

endit:
	call checkSplit

	popad
	RET
testHand ENDP

testSplitHand PROC 
	pushad
	call crlf
	call getplayerSplithand
	mov esi, offset handBuffer
	call crlf
	call crlf
testitsplit:
	mov eax, 0
	mov al, [esi]
	cmp al, 52
	JE enditsplit
	call getcarddisplay
	mov edx, offset carddisplaystring
	call writestring
	call crlf
	;add esi, 4
	inc esi
	;call dumpregs
	jmp testitsplit


enditsplit:

	popad
	RET
testSplitHand ENDP


PrintHandBuffer PROC USES ESI EAX EDX
	MOV ESI, OFFSET HandBuffer
	Top:
		MOV EAX, 0
		MOV AL, [ESI]
		CMP EAX, 52
		JGE EndLoop
		CALL GetCardDisplay
		MOV EDX, OFFSET CardDisplayString
		
		CALL WriteString
		CALL CRLF
		INC ESI
		JMP Top

	EndLoop:
	RET
PrintHandBuffer ENDP


;pass player number in eax split card moves to split array split and phand are dealt another card
SplitHand PROC
	pushad

	mov edx, 0
	mov edi, offset handBuffer
	mov esi, 0
	mov ebx, 0
	push eax	
	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add ebx, TYPE Playerstructure
		dec eax
	.endw

	lea esi, (Playerstructure PTR playerarray[ebx]).bet
	mov al, [esi]
	lea esi, (Playerstructure PTR playerarray[ebx]).splitbet
	mov [esi], al
	lea esi, (Playerstructure PTR playerarray[ebx]).bankacc
	sub [esi], eax
	lea esi, (Playerstructure PTR playerarray[ebx]).phand
	inc esi
	mov al, [esi]
	lea edi, (Playerstructure PTR playerarray[ebx]).splitHand
	mov al, [edi]
	xchg [esi], al
	mov [edi], al
	lea esi, (Playerstructure PTR playerarray[ebx]).splitBool
	mov al, 1
	mov [esi], al


	call dealcard
	movzx ebx, al
	pop eax
	call addcardplayer


	push eax
	call dealcard
	movzx ebx, al
	pop eax
	call addcardplayerSplit

	popad
	RET
SplitHand ENDP

.data
	a1 BYTE "player ",0
	a2 BYTE "dealer ", 0

.code
;pass the player number in EAX to compare to dealer hand
compareHand PROC
	pushad
	push eax
	call getplayerhand
	call gethandvaluenew
	mov bl, al
	call getdealerhand
	call gethandvaluenew
	.IF(al<bl && bl<=21)
		pop eax
		call playerWin
	.ELSEIF(al==bl)
		pop eax
		call playerPush
	.ELSE
		pop eax
		call playerLose
	.ENDIF


	popad
	RET
compareHand ENDP

;pass the player number in EAX to compare to dealer hand
compareSplitHand PROC
	pushad
	push eax
	call getplayerSplithand
	call gethandvaluenew
	mov bl, al
	call getdealerhand
	call gethandvaluenew
	.IF(al<bl && bl<=21)
		pop eax
		call playerSplitWin
	.ELSEIF(al==bl)
		pop eax
		call playerSplitPush
	.ELSE
		pop eax
		call playerLose
	.ENDIF

	popad
	RET
compareSplitHand ENDP

.data
	winPrompt BYTE "You have won this hand", 0dh, 0ah, 0
	losePrompt BYTE "You have lost this hand", 0dh, 0ah, 0
	pushPrompt BYTE "You have pushed this hand", 0dh, 0ah, 0

.code
;pass winning player number in EAX
playerWin PROC USES EDX
	call winbet
	mov edx, offset winPrompt
	call writestring

	RET
playerWin ENDP

playerLose PROC USES EDX
	mov edx, offset losePrompt
	call writeString

	RET
playerLose ENDP

playerPush PROC USES EDX
	call pushBet
	mov edx, offset pushPrompt
	call writeString

	ret
playerPush ENDP

;pass winning player number in EAX
playerSplitWin PROC USES EDX
	call winSplitbet
	mov edx, offset winPrompt
	call writestring

	RET
playerSplitWin ENDP

playerSplitPush PROC USES EDX
	call pushSplitBet
	mov edx, offset pushPrompt
	call writeString

	ret
playerSplitPush ENDP

checkSplit PROC USES EAX ESI
	
	mov eax, 0
	mov al, currentplayer

	mov esi, 0
		;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
	mov bl, (Playerstructure PTR playerarray[esi]).splitBool 
		
	RET
checkSplit ENDP

resetPhand PROC
pushad
	
	mov eax, 1
	mov ecx, playercount
	mov edi, 52

	resethand:
	mov ebx, 0
	push eax
		.while (eax > 1)
			add ebx, TYPE Playerstructure
			dec eax
		.endw
		lea esi, (Playerstructure PTR playerarray[ebx]).Phand 
		mov (Playerstructure PTR playerarray[ebx]).bet, 0
		pop eax

	mov ebx, 0
	resetting:

	MOV [ESI], edi

	INC ebx
	INC ESI
	CMP EBX, HAND_SIZE
	JNE resetting
	inc eax
	loop resethand

popad
ret
resetPhand ENDP


resetPsplit PROC
pushad
	
	mov eax, 1
	mov ecx, playercount
	mov edi, 52

	resetsplit:
	mov ebx, 0
	push eax
		.while (eax > 1)
			add ebx, TYPE Playerstructure
			dec eax
		.endw
		lea esi, (Playerstructure PTR playerarray[ebx]).splitHand
		mov  (Playerstructure PTR playerarray[ebx]).splitbet, 0
		mov  (Playerstructure PTR playerarray[ebx]).splitbool, 0
		pop eax

	mov ebx, 0
	splitreset:

	MOV [ESI], edi

	INC ebx
	INC ESI
	CMP EBX, HAND_SIZE
	JNE splitreset
	inc eax
	loop resetsplit

popad
ret
resetPsplit ENDP


resetdealer PROC
pushad
	mov edx, 52
	mov esi, offset dealer.phand

	mov ebx, 0
	resettingdealer:

	MOV [ESI], edx

	INC ebx
	INC ESI
	CMP EBX, HAND_SIZE
	JNE resettingdealer

popad
ret
resetdealer ENDP