TITLE Blackjack						(main.asm)

; Description:
; 
; Revision date: 11/3/12

HAND_SIZE = 11
CARD_COUNT = 52

INCLUDE IRVINE32.inc
INCLUDE Player.inc
INCLUDE Hand.inc
INCLUDE Deck.inc
INCLUDE Game.inc
.data
Playerstructure STRUCT
		Pname BYTE 30 dup(?)
		Bet DWORD 0
		Bankacc DWORD 300
<<<<<<< HEAD
			phand DB HAND_SIZE DUP(52)
=======
		phand DB HAND_SIZE DUP(52)
>>>>>>> hand value
		CardCount BYTE 0
Playerstructure ends


HandBuffer DB HAND_SIZE DUP(52) ;HandBuffer will hold a hand that is grabbed through a procedure.
CardBuffer DB 52 ;CardBuffer will hold a single card

<<<<<<< HEAD
=======
Dealer Playerstructure<"Dealer",0,0,HAND_SIZE DUP(52),0>
>>>>>>> hand value
PlayerArray PlayerStructure 5 DUP(<>)

Dealer Playerstructure<"Dealer",,,,>


.code 
main PROC
	CALL randomize
	CALL ClearRegs
<<<<<<< HEAD
	CALL Rungame
	
	
	
	
	
	;call initializedeck
	;call shuffledeck

	;call nameinput
	;call outputplayers
	;call dealcard
	;movzx ebx, al
	;call addcarddealer
	;call dealcard
	;movzx ebx, al
	;call addcarddealer
	;mov eax, 1
	;call getplayerhand
=======
	call initializedeck
	call shuffledeck
	;call testDeck
	call crlf
	

	;call nameinput
	;call outputplayers
	call dealcard
	movzx ebx, al
	call addcarddealer
	call dealcard
	movzx ebx, al
	call addcarddealer
	call dealcard
	movzx ebx, al
	call addcarddealer
	call dealcard
	movzx ebx, al
	mov ebx, 2
	call addcarddealer
	
>>>>>>> hand value
	;mov ebx, 5
	;call getplayerhand
	;call addcardplayer
	;mov ebx,43
	;call addcardplayer
	call getdealerhand
	mov ecx, 0
	mov cl, dealer.CardCount
	call outputHand
	;call setplayerhand

	;call getplayerhand
	
	

	CALL CRLF
	
	MOV ESI, OFFSET HandBuffer
	MOV EAX, 0
	MOV [ESI], EAX
	MOV EAX, 13
	MOV [ESI+1], EAX
	MOV EAX, 34
	MOV [ESI+2], EAX
	MOV EAX, 51
	MOV [ESI+3], EAX
	MOV EAX, 52
	MOV [ESI+4], EAX
	MOV EAX, 0
	CALL SetPlayerHand
	CALL PrintHandBuffer
	
	CALL GetHandValueNew
	CALL CRLF
	CALL WriteInt

	CALL CRLF
exit
main endp

ClearRegs PROC
	
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	mov esi, 0
	mov edi, 0

	ret
ClearRegs ENDP

END main