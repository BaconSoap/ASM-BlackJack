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
INCLUDE joeproc.inc
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


HandBuffer DB HAND_SIZE DUP(52) ;HandBuffer will hold a hand that is grabbed through a procedure.
CardBuffer DB 52 ;CardBuffer will hold a single card


PlayerArray PlayerStructure 5 DUP(<>)
Dealer Playerstructure<"Dealer",,,,>


.code 
main PROC
	CALL randomize
	CALL ClearRegs
		push eax
		mov eax,lightRed
		call SetTextColor
		pop eax
		MOV EDX, offset welcomeMsg
		Call WriteString
		call setDefaultTxtColor
		CALL NameInput
	
		gameloop:
			CALL RunGame
			call resetphand
			call resetPsplit
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