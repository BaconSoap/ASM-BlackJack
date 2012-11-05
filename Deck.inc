;Deck Procedures

.data
Deck DB CARD_COUNT DUP (?)
Discard DB CARD_COUNT DUP(?)
CardDisplayString DB "10 S", 0, 0

;Test various deck procedures
.CODE
TestDeck PROC
	MOV ECX, 0
	MOV EDI, offset Deck
	
	s:
	MOV EAX, 0
	MOV AL, [EDI]
	CALL GetCardDisplay
	MOV EDX, OFFSET CardDisplayString
	CALL WriteString
	CALL CrLf
	INC ECX
	INC EDI

	CMP ECX, CARD_COUNT
	JNE s
	RET
TestDeck ENDP

InitializeDeck PROC USES ECX ESI
	MOV ECX, 0
	MOV ESI, OFFSET Deck

	StartDeckInitLoop:
		MOV [ESI], ECX
		INC ECX
		INC ESI
		CMP ECX, CARD_COUNT
	JNE StartDeckInitLoop
	RET
InitializeDeck ENDP

;Gets the suit of a card
;0-12:		Spades		(0)
;13-25:		Diamonds	(1)
;26-38:		Clubs		(2)
;39-51:		Hearts		(3)
;Input:		Card number in EAX
;Output:	Card suit in EAX
GetSuit PROC
	CMP	EAX, 13
	JL	SetSpades
	CMP	EAX, 26
	JL	SetDiamonds
	CMP EAX, 39
	JL	SetClubs
	JMP	SetHearts

	SetSpades:
	MOV EAX, 0
	RET
	SetDiamonds:
	MOV EAX, 1
	RET
	SetClubs:
	MOV EAX, 2
	RET
	SetHearts:
	MOV EAX, 3
	RET
GetSuit ENDP

;Takes a CardID (0-51) and returns the 0-based card number
;Input:		CardID in EAX
;Output:	Card Number in EAX
GetCardNumber PROC
	CMP EAX, 13
	JL ReturnNumber

	SUB EAX, 13
	CMP EAX, 13
	JL ReturnNumber
	
	SUB EAX, 13
	CMP EAX, 13
	JL ReturnNumber
	
	SUB EAX, 13

	ReturnNumber:
	RET
GetCardNumber ENDP

;Takes a CardID and returns the display form of the card: 1 -> "2 S", 10 -> "J S"
;Input:		CardID in EAX
;Output:	Display string in CardDisplayString
GetCardDisplay PROC USES EBX ESI ECX
	MOV ESI, OFFSET CardDisplayString
	MOV BL, AL
	
	;Format display of the card value
	CALL GetCardNumber
	CMP AL, 0
	JE Ace

	CMP AL, 9
	JL LessThanTen
	CMP AL, 9
	JE Ten
	CMP AL, 10
	JE Jack
	CMP AL, 11
	JE Queen
	CMP AL, 12
	JE King

	LessThanTen:
	ADD AL, 49
	JMP SetSingleCharacter

	Ten:
	MOV AL, "1"
	MOV [ESI], AL
	INC ESI
	MOV AL, "0"
	MOV [ESI], AL
	INC ESI
	JMP SetSuit

	Jack:
	MOV AL, "J"
	JMP SetSingleCharacter

	Queen:
	MOV AL, "Q"
	JMP SetSingleCharacter

	King:
	MOV AL, "K"
	JMP SetSingleCharacter

	Ace:
	MOV AL, "A"
	JMP SetSingleCharacter

	SetSingleCharacter:
	MOV [ESI], AL
	INC ESI


	;Format display of the card suit
	SetSuit:
	MOV AL, 9
	MOV [ESI], AL
	INC ESI

	MOV AL, BL

	CALL GetSuit
	CMP AL, 0
	JE Spades
	CMP AL, 1
	JE Diamonds
	CMP AL, 2
	JE Clubs
	JMP Hearts

	Spades:
	MOV AL, "S"
	JMP SetSuitChar
	Diamonds:
	MOV AL, "D"
	JMP SetSuitChar
	Clubs:
	MOV AL, "C"
	JMP SetSuitChar
	Hearts:
	MOV AL, "H"
	
	SetSuitChar:

	MOV [ESI], AL
	INC ESI

	MOV AL, 0
	MOV [ESI], AL
	RET
GetCardDisplay ENDP