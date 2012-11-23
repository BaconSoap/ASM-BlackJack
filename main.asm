TITLE Blackjack						(main.asm)

; Description:
; 
; Revision date: 11/3/12

HAND_SIZE = 11
CARD_COUNT = 52

INCLUDE IRVINE32.inc
INCLUDE PLAYER.inc
INCLUDE Hand.inc
INCLUDE DECK.INC
include game.inc

.data
Playerstructure STRUCT
		Pname BYTE 30 dup(?)
		Bet DWORD 434
		Bankacc DWORD 300
		phand BYTE HAND_SIZE DUP(52)
		CardCount BYTE 0
Playerstructure ends


HandBuffer DB HAND_SIZE DUP(52) ;HandBuffer will hold a hand that is grabbed through a procedure.
CardBuffer DB 52 ;CardBuffer will hold a single card

Dealer Playerstructure<"Dealer">
Player1 PlayerStructure<>
Player2 PlayerStructure<>
Player3 PlayerStructure<>
Player4 PlayerStructure<>
Player5 PlayerStructure<>

.code 
main PROC
CALL randomize
CALL clearregs

call rungame
;call nameinput
;call outputplayers
;CALL initializedeck
;call shuffledeck
;call testHand
;call winbet
;call placebet

;mov eax, player3.bet
;call writedec



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