.data
DEALERCARDCOUNT DB 2	; dealer has initial 2 cards
total Dword ?			; total of hand value in being looked at
aceCount DWord ?		; holds amount of aces in current hand
handType BYTE 'H'		; hold type of hand
a1 BYTE "player ",0
a2 BYTE "dealer ", 0
winPrompt BYTE "You have won this hand", 0dh, 0ah, 0
losePrompt BYTE "You have lost this hand", 0dh, 0ah, 0
pushPrompt BYTE "You have pushed this hand", 0dh, 0ah, 0
.code
;Output: stores dealer hand in the handbuffer
GetDealerHand PROC USES EAX ESI EDI ECX
	MOV ESI, OFFSET Dealer.Phand
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

;Output: stores handbuffer into dealer hand
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
	RET
SetDealerHand ENDP

;Input: player number in EAX
;Output: stores current player hand into handbuffer
getPlayerHand PROC USES ECX ESI EDI EAX EBX
	mov ecx, 0
	mov edi, offset handBuffer
	mov esi, 0
	mov ebx, 0
	push eax	
	.while (eax > 1)	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
		add ebx, TYPE Playerstructure
		dec eax
	.endw
	lea esi, (Playerstructure PTR playerarray[ebx]).Phand ; had to use lea instead of offset to manipulate hand
	pop eax
	getHand:
	push eax
	copyGet:	;copies player hand to handbuffer
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

;Input: player number in EAX
;Output: players split hand copied into handbuffer
getPlayerSplitHand PROC USES ECX ESI EDI EAX EBX
	mov ecx, 0
	mov edi, offset handBuffer
	mov esi, 0
	mov ebx, 0
	push eax	
	.while (eax > 1)	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
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

;Input: pass player number in EAX
;Output: handbuffer copied into players hand
setPlayerHand PROC USES ECX EDI ESI EAX EBX
	mov ecx, 0
	mov edi, offset handBuffer
	mov esi, 0
	mov ebx, 0
	push eax	
	.while (eax > 1)	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
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

;Input: pass player number in EAX
;Output: handbuffer copied into players split hand
setPlayerSplitHand PROC USES ECX EDI ESI EAX EBX
	mov ecx, 0
	mov edi, offset handBuffer
	mov esi, 0
	mov ebx, 0
	push eax	
	.while (eax > 1)	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
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

;Input: player number in EAX and card to add in EBX
;Output: adds card to players hand
addCardPlayer PROC USES ECX ESI EAX
	call getPlayerHand
	mov esi, offset handbuffer
		one:		; find first empy space in hand
			mov cl, [esi]
			cmp cl, 52
			JE endadd
			inc esi
		jmp one
	endadd:		; add card to hand
	mov [esi], bl
	call setPlayerHand	; set players hand to new hand
	mov esi, 0
		.while (eax > 1);this will add the type of playerstructure to esi, making it go to the corresponding player in the array
			add esi, TYPE Playerstructure
			dec eax
		.endw
	inc (Playerstructure PTR playerarray[esi]).cardCount
RET
addCardPlayer ENDP

;Input: player number in EAX and card to add in EBX
;Output: adds card to a players split hand
addCardPlayerSplit PROC USES ECX ESI EAX
	call getPlayerSplitHand
	mov esi, offset handbuffer
		one:	; find first empty space in split hand 
			mov cl, [esi]
			cmp cl, 52
			JE endadd
			inc esi
		jmp one
	endadd:		; adds card to split hand
	mov [esi], bl
	call setPlayerSplitHand ; set players split hand to new split hand
RET
addCardPlayerSplit ENDP

;Input: card to add in EBX
;Output: adds a card to the dealers hand
addCardDealer PROC USES ECX ESI EAX
	call getDealerHand
	mov esi, offset handbuffer
		one:	; find first empty space in split hand 
			mov cl, [esi]
			cmp cl, 52
		JE endadd
	inc esi
	jmp one
	endadd:		; adds card to dealers hand
	mov [esi], bl
	call setDealerHand
	inc dealer.cardCount
RET
addCardDealer ENDP

;Takes handbuffer and returns score for it
;Output: player's hand value in EAX
GetHandValueNew PROC USES ESI EDX
	mov al, 'H'
	mov handType, al
	MOV ESI, OFFSET HandBuffer
	MOV total, 0	; total hand value
	MOV aceCount, 0	; how many aces in hand
	MOV EDX, 0
		StartFirstLoop:
			inc edx
			MOV EAX, 0
			MOV AL, [ESI]
			CMP EAX, 52
			JE StartAceLoop	; if end of cards in hand go to ace loop
			CALL getCardNumber
				.if (EDX == 1);moves values to fcardval and scardval in order to see if you can split
					mov FCardVal, AL
				.elseif (EDX == 2)
					mov SCardVal, AL
				.endif
			CMP EAX, 0	; if current card is an ace
			JE IHateAces
			CMP EAX, 10	; if current card is a 10 J Q K
			JGE ClampToTen
			INC EAX
			JMP AddToTotal
			IHateAces:	; adds 11, high ace value
				INC aceCount
				MOV EAX, 11
				JMP AddToTotal
			ClampToTen:	; adds 10, 10 or face card 
				MOV EAX, 10
				JMP AddToTotal
			AddToTotal:	; adds value to total and loops for hand
				ADD total, EAX
				INC ESI
		JMP StartFirstLoop
		StartAceLoop:	; run this loop for number of aces handles ace value depending on if you would bust
			CMP aceCount, 0
			JLE EndOfProc
			CMP total, 21	;IF Total > 21 Subtract 10 from total
			JG SubtractTen
			JMP EndOfProc
			SubtractTen:
			SUB total, 10
			DEC aceCount
		JMP StartAceLoop
	EndOfProc:
	MOV EAX, total
		.IF(aceCount > 0)
			mov handType, 'S'
		.ENDIF
RET
GetHandValueNew ENDP

;Displays the dealers cards
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

;Input: player number in EAX
;Output: displays the players cards
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
		inc esi
	jmp testit
	endit:
	call checkSplit
popad
RET
testHand ENDP

;Input: player number in EAX
;Output: displays the players split hand
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
		inc esi
	jmp testitsplit
	enditsplit:
popad
RET
testSplitHand ENDP

;displays the handbuffer
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

;Input: player number in eax 
;Output: Moves second card into the players split hand and deals a new card to the hand and split hand
SplitHand PROC
pushad
	mov edx, 0
	mov edi, offset handBuffer
	mov esi, 0
	mov ebx, 0
	push eax	
		.while (eax > 1)	;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
			add ebx, TYPE Playerstructure
			dec eax
		.endw
	lea esi, (Playerstructure PTR playerarray[ebx]).bet	; takes inital bet and moves it into the splitbet and takes it out of bank account, sets split hand and sets split bool
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
	call dealcard	; deals new card to player after split
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

;Input: player number in EAX
;Output: player wins hand if hand is better than dealer's pushes if equal to and loses if less
compareHand PROC
pushad
	push eax
	call getplayerhand
	call gethandvaluenew
	mov bl, al
	call getdealerhand
	call gethandvaluenew
		.IF(al<bl && bl<=21) ; hand value beats dealer
			pop eax
			call playerWin
		.ELSEIF(al==bl)		; equal to dealer
			pop eax
			call playerPush
		.ELSE				; anything else, player loses
			pop eax
			call playerLose
		.ENDIF
popad
RET
compareHand ENDP

;Input: player number in EAX
;Output: player wins split hand if hand is better than dealer's pushes if equal to and loses if less
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
			call playerSplitLose
		.ENDIF
popad
RET
compareSplitHand ENDP

;Input:  winning player number in EAX
;Output: players bank adds double the bet
playerWin PROC USES EDX
	MOV CurrentPlayerMessage, 2
	CALL SetPlayerMessage
	call winbet
	mov edx, offset winPrompt
	call writestring
RET
playerWin ENDP

;--------------------------------------------------------------------
;Input: losing player number in EAX
;Output: bet is reset to 0
playerLose PROC USES EDX
	MOV CurrentPlayerMessage, 3
	CALL SetPlayerMessage
	call losebet
	mov edx, offset losePrompt
	call writeString

	RET
playerLose ENDP

;--------------------------------------------------------------------
;Input: pushing player number in EAX
;Output: players bank add bet back
playerPush PROC USES EDX
	call pushBet
	mov edx, offset pushPrompt
	call writeString

	ret
playerPush ENDP

;--------------------------------------------------------------------
;Input:  winning player number in EAX
;Output: players bank adds double the bet
playerSplitWin PROC USES EDX
	call winSplitbet
	mov edx, offset winPrompt
	call writestring

	RET
playerSplitWin ENDP

;--------------------------------------------------------------------
;Input: losing player number in EAX
;Output: bet is reset to 0
playerSplitLose PROC USES EDX
	call loseSplitbet
	mov edx, offset losePrompt
	call writeString

	RET
playerSplitLose ENDP

;--------------------------------------------------------------------
;Input: pushing player number in EAX
;Output: players bank add bet back
playerSplitPush PROC USES EDX
	call pushSplitBet
	mov edx, offset pushPrompt
	call writeString

	ret
playerSplitPush ENDP

;--------------------------------------------------------------------
;Input: Player number in EAX
;Output: bl=0 if player has not split and bl=1 if the player split
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

;--------------------------------------------------------------------
;Input: player number in EAX
;Output: blank player hand
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
		MOV (Playerstructure PTR playerarray[ebx]).Message, 0
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

;--------------------------------------------------------------------
;Input: player number in EAX
;Output: blank player split hand
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

;--------------------------------------------------------------------
;Output: blank dealer hand
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