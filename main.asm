TITLE Blackjack						(main.asm)

; Description:
; 
; Revision date: 11/3/12

INCLUDE Irvine32.inc
INCLUDE Deck.inc

.code
main PROC
	call Clrscr
	call ClearRegs
	CALL InitializeDeck
	
	CALL TestDeck

	CALL CrLf
	exit
main ENDP

ClearRegs PROC
	
	mov eax, 0
	mov ebx, 0
	mov ecx, 0
	mov edx, 0
	mov esi, 0
	mov edi, 0

	ret
ClearRegs ENDP

GetNamesOfPlayers PROC
	RET
GetNamesOfPlayers ENDP

GetNumberOfPlayers PROC
	RET
GetNumberOfPlayers ENDP

GiveStartingMoney PROC
	RET
GiveStartingMoney ENDP

ShuffleDeck PROC
	RET
ShuffleDeck ENDP

TakeBets PROC
	RET
TakeBets ENDP

DealHands PROC
	RET
DealHands ENDP

TakeTurns PROC
	RET
TakeTurns ENDP

HitPlayer PROC
	RET
HitPlayer ENDP

StandPlayer PROC
	RET
StandPlayer ENDP

DrawCard PROC
	RET
DrawCard ENDP

END main
