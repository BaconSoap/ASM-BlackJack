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
	mov ecx, 10
	dealLoop:
		push eax
		call dealCard
		mov ebx, eax
		pop eax
		call addcardPlayer
	loop dealLoop

	mov esi, offset player1Hand

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


;pass player number in EAX and card in EBX
addCardPlayer PROC USES ECX ESI

	cmp eax, 1
	JE addto1
	cmp eax, 2
	JE addto2
	cmp eax, 3
	JE addto3
	cmp eax, 4
	JE addto4
	cmp eax, 5
	JE addto5

	addto1:
		mov esi, offset player1Hand
		one:
			mov cl, [esi]
			cmp cl, 52
			JE endadd
			inc esi
			jmp one

	addto2:
		mov esi, offset player2Hand
		two:
			mov cl, [esi]
			cmp cl, 52
			JE endadd
			inc esi
			jmp two
	
	addto3:
		mov esi, offset player3Hand
		three:
			mov cl, [esi]
			cmp cl, 52
			JE endadd
			inc esi
			jmp three
		
	addto4:
		mov esi, offset player4Hand
		four:
			mov cl, [esi]
			cmp cl, 52
			JE endadd
			inc esi
			jmp four
		
	addto5:
		mov esi, offset player5Hand
		five:
			mov cl, [esi]
			cmp cl, 52
			JE endadd
			inc esi
			jmp five


endadd:
	mov [esi], ebx

	RET
addCardPlayer ENDP

getHandValue PROC

	RET
getHandValue ENDP