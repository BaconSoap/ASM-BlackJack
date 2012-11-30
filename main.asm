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
INCLUDE Display.inc
INCLUDE help.inc


.data
Playerstructure STRUCT
		Pname BYTE 21 dup(?)
		Bet DWORD 0
		Bankacc DWORD 300
		phand DB HAND_SIZE DUP(52)
		CardCount BYTE 0
		splitHand DB HAND_SIZE DUP(52)
		splitBool DB 0
		splitBet DWORD 0
Playerstructure ends

COORD STRUCT
	x word ?
	y word ?
COORD ENDS

HandBuffer DB HAND_SIZE DUP(52) ;HandBuffer will hold a hand that is grabbed through a procedure.
CardBuffer DB 52 ;CardBuffer will hold a single card
BufferSize COORD <160, 100>
DealerScore DB "Dealer Had: ", 0

PlayerArray PlayerStructure 5 DUP(<>)
Dealer Playerstructure<"Dealer",,,,>
hConsoleOut    DWORD ?


.code 
main PROC

	invoke GetStdHandle, -11
	mov hConsoleOut, eax
	invoke SetConsoleScreenBufferSize, hConsoleOut, BufferSize

	CALL randomize
	CALL ClearRegs

		MOV EDX, offset welcomeMsg
		Call WriteString
		call setDefaultTxtColor
		CALL NameInput
	
		gameloop:
			CALL RunGame
			call getdealerhand
			call gethandvaluenew
			MOV EDX, OFFSET DealerScore
			CALL WriteString
			CALL WriteDec
			CALL CRLF
			call resetphand
			call resetPsplit
			call resetdealer
		loop gameloop
	
	CALL getdealerhand
	MOV ecx, 0
	MOV cl, dealer.CardCount
	
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