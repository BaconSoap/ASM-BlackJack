.data
AllHands DB CARD_COUNT DUP(52)
DealerHand DB HAND_SIZE DUP(52)
player1Hand DB HAND_SIZE DUP(52)
player2Hand DB HAND_SIZE DUP(52)
player3Hand DB HAND_SIZE DUP(52)
Player4Hand DB HAND_SIZE DUP(52)
player5Hand DB HAND_SIZE DUP(52)

.code

testHand PROC
	mov eax, 1
	mov ecx, 2
	dealLoop:
		push eax
		call dealCard
		mov ebx, eax
		pop eax
		call addcardPlayer
	loop dealLoop

	call getPlayerHand
	mov esi, offset handBuffer

testit:
	mov eax, 0
	mov al, [esi]
	cmp al, 52
	JE endit
	call getcarddisplay
	mov edx, offset carddisplaystring
	call writestring
	call crlf
	add esi, 4
	jmp testit

	endit:


	RET
testHand ENDP

;Gets the dealer hand and puts it into HandBuffer
GetDealerHand PROC USES EAX ESI EDI ECX
	MOV ECX, 0
	MOV ESI, OFFSET DealerHand
	MOV EDI, OFFSET HandBuffer
	StartCopy:
	MOV EAX, [ESI]
	MOV [EDI], EAX

	INC ECX
	INC ESI
	INC EDI
	CMP ECX, HAND_SIZE
	JE StartCopy
	RET
GetDealerHand ENDP

;Places HandBuffer into the dealer hand
SetDealerHand PROC USES EAX ESI EDI ECX
	MOV ECX, 0
	MOV EDI, OFFSET DealerHand
	MOV ESI, OFFSET HandBuffer
	StartCopy:
	MOV EAX, [ESI]
	MOV [EDI], EAX

	INC ECX
	INC ESI
	INC EDI
	CMP ECX, HAND_SIZE
	JE StartCopy
	RET
SetDealerHand ENDP

;pass player number in EAX places that players hand into handbuffer
getPlayerHand PROC USES ECX ESI EDI
	mov ecx, 0
	mov edi, offset handBuffer

	cmp eax, 1
	JE get1
	cmp eax, 2
	JE get2
	cmp eax, 3
	JE get3
	cmp eax, 4
	JE get4
	cmp eax, 5
	JE get5

	get1:
		mov esi, offset player1Hand
		jmp getHand
		
	get2:
		mov esi, offset player2Hand
		jmp getHand
		
	get3:
		mov esi, offset player3Hand
		jmp getHand
		
	get4:
		mov esi, offset player4Hand
		jmp getHand
		
	get5:
		mov esi, offset player5Hand
		jmp getHand

	getHand:
	push eax

	copyGet:
	MOV al, [ESI]
	MOV [EDI], al

	INC ECX
	INC ESI
	INC EDI
	CMP ECX, HAND_SIZE
	JE copyGet

	pop eax
		
	RET
getPlayerHand ENDP

;pass player number in EAX places handbuffer into that players hand
setPlayerHand PROC USES ECX EDI ESI
	mov ecx, 0
	mov edi, offset handBuffer

	cmp eax, 1
	JE set1
	cmp eax, 2
	JE set2
	cmp eax, 3
	JE set3
	cmp eax, 4
	JE set4
	cmp eax, 5
	JE set5

	set1:
		mov esi, offset player1Hand
		jmp setHand
		
	set2:
		mov esi, offset player2Hand
		jmp setHand
		
	set3:
		mov esi, offset player3Hand
		jmp setHand
		
	set4:
		mov esi, offset player4Hand
		jmp setHand
		
	set5:
		mov esi, offset player5Hand
		jmp setHand

	setHand:
	push eax

	copySet:
	MOV al, [EDI]
	MOV [ESI], al

	INC ECX
	INC ESI
	INC EDI
	CMP ECX, HAND_SIZE
	JE copySet

	pop eax

	RET
setPlayerHand ENDP


;pass player number in EAX and card to add in EBX
addCardPlayer PROC USES ECX ESI
	call getPlayerHand
	mov esi, offset handbuffer

one:
	mov cl, [esi]
	cmp cl, 52
	JE endadd
	inc esi
	jmp one

endadd:
	mov [esi], ebx
	call setPlayerHand

	RET
addCardPlayer ENDP


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