TITLE Blackjack						(main.asm)

; Description:
; 
; Revision date: 11/3/12

HAND_SIZE = 11
CARD_COUNT = 52

INCLUDE Irvine32.inc
INCLUDE Deck.inc
INCLUDE Hand.inc
INCLUDE Player.inc
INCLUDE Graphics.inc
INCLUDE Game.inc
INCLUDE help.inc

.data

HandBuffer DB HAND_SIZE DUP(52) ;HandBuffer will hold a hand that is grabbed through a procedure.
CardBuffer DB 52 ;CardBuffer will hold a single card
;PlayerCount DB 0
.code

main PROC
	call randomize

	call Clrscr
	call ClearRegs
	
	CALL InitGame
	call shuffledeck
	CALL TestDeck

	CALL GameLoop

	CALL CrLf

	call testDeal
	
	;call testHelp
	;call testPlayer
	;call takebets
	;call outputPlayers
	call crlf

	exit
main ENDP

InitGame PROC
	CALL InitializeDeck
	RET
InitGame ENDP

GameLoop PROC
	RET
GameLoop ENDP

ClearRegs PROC
	
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	mov esi, 0
	mov edi, 0

	ret
ClearRegs ENDP

GetPlayers PROC
	call nameInput
	RET
GetPlayers ENDP

TakeBets PROC USES EAX ECX
	mov eax, 1
	mov ecx, playercount
	betLoop:
		call inputBet
		inc eax
	loop betLoop
	RET
TakeBets ENDP

DealHands PROC
	RET
DealHands ENDP

TakeTurns PROC
	RET
TakeTurns ENDP

HitPlayer PROC
	call dealCard

	RET
HitPlayer ENDP

StandPlayer PROC
	
	RET
StandPlayer ENDP

DrawCard PROC
	
	RET
DrawCard ENDP

END main