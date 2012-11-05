.data
AllHands DB CARD_COUNT DUP(52)
DealerHand DB HAND_SIZE DUP(52)

.code

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