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
			phand DB HAND_SIZE DUP(52)
		CardCount BYTE 0
Playerstructure ends


HandBuffer DB HAND_SIZE DUP(52) ;HandBuffer will hold a hand that is grabbed through a procedure.
CardBuffer DB 52 ;CardBuffer will hold a single card

PlayerArray PlayerStructure 5 DUP(<>)

Dealer Playerstructure<"Dealer",,,,>


.code 
main PROC
	CALL randomize
	CALL ClearRegs
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
	;mov ebx, 5
	;call getplayerhand
	;call addcardplayer
	;mov ebx,43
	;call addcardplayer
	;call getdealerhand
	;call setplayerhand

	;call getplayerhand
	;call testhand

ClearRegs PROC
	
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	mov esi, 0
	mov edi, 0

	ret
ClearRegs ENDP

exit
main endp
END main