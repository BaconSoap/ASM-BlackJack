.data
;using to test
DealerHand DB 1,3, 51, 52
DEALERCARDCOUNT DB 2
total DD ?
aceCount DD ?

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
	MOV Dealer.CardCount, bl
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

	
	getHand:
	push eax

	copyGet:
	MOV al, [EDI]
	MOV [ESI], al

	INC ECX
	INC ESI
	INC EDI
	CMP ECX, HAND_SIZE
	JNE copyGet

	pop eax
		
	RET
setPlayerHand ENDP



;pass player number in EAX and card to add in EBX
addCardPlayer PROC USES ECX ESI EAX
	call getPlayerHand
	mov esi, offset handbuffer

one:
;call dumpregs
	mov cl, [esi]
	cmp cl, 52
	JE endadd
	inc esi
	jmp one
	;call dumpregs
endadd:
	mov [esi], bl
	call setPlayerHand

	RET
addCardPlayer ENDP

; card to add in EBX
addCardDealer PROC USES ECX ESI EAX
	call getDealerHand
	mov esi, offset handbuffer

one:
call dumpregs
	mov cl, [esi]
	cmp cl, 52
	JE endadd
	inc esi
	jmp one
	call dumpregs
endadd:
	mov [esi], bl
	call setDealerHand

	RET
addCardDealer ENDP



getHandValue PROC
	call getPlayerHand

	mov esi, offset handbuffer
	mov ebx, 0
	mov eax, [esi]

	sumHand:
	call getCardNumber
	add ebx, eax
	inc esi
	mov eax, [esi]
	cmp eax, 52
	JE J1

	J1:
	mov esi, offset handbuffer
	J2:
	cmp ebx, 21
	JBE endit

	checkAce:
		mov edx, [esi]
		cmp edx, 11
		JE changeto1
		cmp edx, 52
		JE endit
		inc esi
		jmp checkace

		changeto1:
		sub ebx, 10
		jmp J2


	endit:


	RET
getHandValue ENDP

;Takes in EAX player #
;Outputs player's score in EAX
GetHandValueNew PROC USES ESI 
	
	CALL getPlayerHand
	MOV ESI, OFFSET HandBuffer
	MOV total, 0
	MOV aceCount, 0

	
	StartFirstLoop:
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
	;call writeint
	cmp al, 52
	JE endstuff
	call getcarddisplay
	mov edx, offset carddisplaystring
	call writestring
	call crlf
	;add esi, 4
	inc esi
	;call dumpregs

	jmp loopen

	endstuff:

	popad
	RET
showdealerHand ENDP


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
		JMP StartAceLoop
	EndOfProc:
	MOV EAX, total
	
	RET
GetHandValueNew ENDP


testHand PROC USES ESI EAX EDX
	pushad
	call writeint
	call crlf
	call getplayerhand
	mov esi, offset handBuffer
	call crlf
	call crlf
testit:
	mov eax, 0
	mov al, [esi]
	;call writeint
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

	popad
	RET
testHand ENDP


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