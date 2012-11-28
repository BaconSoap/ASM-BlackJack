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
INCLUDE joeproc.inc
INCLUDE help.inc

.data
Playerstructure STRUCT
		Pname BYTE 30 dup(?)
		Bet DWORD 0
		Bankacc DWORD 300
		phand DB HAND_SIZE DUP(52)
		CardCount BYTE 0
		splitHand DB HAND_SIZE DUP(52)
		splitBool DB 0
		splitBet DWORD 0
Playerstructure ends


HandBuffer DB HAND_SIZE DUP(52) ;HandBuffer will hold a hand that is grabbed through a procedure.
CardBuffer DB 52 ;CardBuffer will hold a single card


PlayerArray PlayerStructure 5 DUP(<>)
Dealer Playerstructure<"Dealer",,,,>



.code 
main PROC
	CALL randomize
	CALL ClearRegs
	CALL RunGame

	;mov ebx, 5
	;call getplayerhand
	;call addcardplayer
	;mov ebx,43
	;call addcardplayer
	call getdealerhand
	mov ecx, 0
	mov cl, dealer.CardCount
	
	;call setplayerhand

	;call getplayerhand
	
	

;	CALL CRLF
	
;	MOV ESI, OFFSET HandBuffer
;	MOV EAX, 0
;	MOV [ESI], EAX
;	MOV EAX, 13
;	MOV [ESI+1], EAX
;	MOV EAX, 34
;	MOV [ESI+2], EAX
;	MOV EAX, 51
;	MOV [ESI+3], EAX
;	MOV EAX, 52
;	MOV [ESI+4], EAX
;	MOV EAX, 0
;	CALL SetPlayerHand
;	CALL PrintHandBuffer
	
;	CALL GetHandValueNew
;	CALL CRLF
;	CALL WriteInt

;	CALL CRLF
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