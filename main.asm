TITLE Blackjack						(main.asm)

; Description:
; 
; Revision date: 11/3/12

HAND_SIZE = 11	; hand size is always 11
CARD_COUNT = 52	; 52 cards are in a deck
INCLUDE IRVINE32.inc
INCLUDE Player.inc
INCLUDE Hand.inc
INCLUDE Deck.inc
INCLUDE Game.inc
INCLUDE Display.inc
INCLUDE help.inc
.data
Playerstructure STRUCT			; structure for player, contains everything a player in blackjack needs
	Pname BYTE 21 dup(?)		; holds player name
	Bet DWORD 0					; current player bet for hand
	Bankacc DWORD 300			; bank account, initializes at 300 points
	phand DB HAND_SIZE DUP(52)	; initial player hand is empy
	CardCount BYTE 0			; holds amount of cards player currently has
	splitHand DB HAND_SIZE DUP(52); hold split hand, initializes empty
	splitBool DB 0				; sets '1' if player has a split hand
	splitBet DWORD 0			; players bet for split hand
	ACTIVE	DD	1				; if player is active, able to make bets and play
	Message	DB	0				; sets to number which represents type of message to be displayed
Playerstructure ends
COORD STRUCT					; used for screen x and y coordinates
	x word ?
	y word ?
COORD ENDS
HandBuffer DB HAND_SIZE DUP(52) ; HandBuffer will hold a hand that is grabbed through a procedure, for manipulation
CardBuffer DB 52				; CardBuffer will hold a single card, for manipulation
BufferSize COORD <160, 100>		; size of playing window
DealerScore DB "Dealer Had: ", 0; string for displaying hand
PlayerArray PlayerStructure 5 DUP(<>); creates an array of 5 players with defualt initialization
Dealer Playerstructure<"Dealer",10, 300>; dealer structure
hConsoleOut    DWORD ?
.code 
main PROC ; main procedure of the game
	invoke GetStdHandle, -11
	mov hConsoleOut, eax
	invoke SetConsoleScreenBufferSize, hConsoleOut, BufferSize
	CALL randomize	; seeds random generator
	CALL ClearRegs	; clears regsiters
	MOV EDX, offset welcomeMsg	; welcome message
	Call WriteString
	CALL NameInput	; asks for amount of players and names of players
	PUSH ECX
	MOV ECX, PlayerCount
	MOV ActivePlayers, ECX
	POP ECX
		gameloop:	; main game loop, game exits once this is not jumped to anymore
			CALL RunGame	; run game procedure that has most functionality of the game
			call getdealerhand
			call gethandvaluenew
			MOV EDX, OFFSET DealerScore	; shows dealer score
			CALL WriteString
			CALL WriteDec
			CALL CRLF
			call resetphand	; resets all players hands, bets, split bets
			call resetPsplit; resets all players split hands
			call resetdealer; resets dealers hand
		jmp gameloop
	CALL getdealerhand
	MOV ecx, 0
	MOV cl, dealer.CardCount
exit
main endp

;Sets all registers to 0
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