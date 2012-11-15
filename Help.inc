COMMENT !

Window prompt overtop of table for number of players 1-5
Window prompt overtop of table prompt for player names 
Each player starts with $300
	Either in text or with chips
	Chips values are 
	Red $5
	Blue $10
	Green $25
	Orange $50
Betting goes around the table from player 1 to the last player there is a $10 minimum bet and maximum of the players total amount of money.  Window prompt for bet amount.  

One card is dealt to each the players face up starting from the bottom right of the screen to the bottom left one at a time.  One face up card is dealt to the dealer.  
A second face up card is dealt to the players in the same order followed by one face down card to the dealer.  

Each player gets the choice to stand, hit, split, double down.  
Stand means that the player does not take anymore cards and his decisions are done.  
Hit is to take another card and then the player has the choice to stand, hit, or double down after that.  
Split is a choice if the players original two cards have the same value, the player can choose to split and make two hands being dealt a second card for each of 
the originals and betting an equal amount as the original bet for the new second hand.  The player then plays both hands separately. 
Double down means that the player doubles his bet, takes one more card and stands.

The dealer plays his cards by always standing if the value is 17 or greater and always hitting if the value is 16 and under. 

The Goal of the game is to get a card value of 21 or to have a better hand than the dealer.  If the cards total to more than 21 that is a bust and that player loses his bet.  
For the players that have values under 21 the hand is then compared to the dealers hand.  If the player has a lower value than the dealer they lose their bet.  
If the values are the same it is a push and the player receives their bet back.  If the value is greater than the dealer they win the hand and receives twice their bet back.  
If the dealer busts all players that did not bust win and receive twice their bet back.

After each round the players banks are updated based on their winning or losing the player then has the choice to leave the table or place a bet.  

Once bets are placed a full 52 card deck is shuffled and dealt to the remaining players.  


!

.data
	rulesPrompt BYTE "Each player starts with $300", 0dh, 0ah

		BYTE "Betting goes around the table from player 1 to the last player there is a $10 minimum bet and maximum of the players total amount of money.", 0dh, 0ah
		BYTE "Window prompt for bet amount.", 0dh, 0ah

		BYTE "One card is dealt to each the players face up starting from the bottom right of the screen to the bottom left one at a time.", 0dh, 0ah 
		BYTE "One face up card is dealt to the dealer.", 0dh, 0ah  
		BYTE "A second face up card is dealt to the players in the same order followed by one face down card to the dealer.", 0dh, 0ah  

		BYTE "Each player gets the choice to stand, hit, split, double down.", 0dh, 0ah 
		BYTE "Stand means that the player does not take anymore cards and his decisions are done.", 0dh, 0ah  
		BYTE "Hit is to take another card and then the player has the choice to stand, hit, or double down after that.", 0dh, 0ah  
		BYTE "Split is a choice if the players original two cards have the same value, the player can choose to split and make two hands being", 0dh, 0ah 
		BYTE "dealt a second card for each of the originals and betting an equal amount as the original bet for the new second hand.", 0dh, 0ah
		BYTE "The player then plays both hands separately.", 0dh, 0ah
		BYTE "Double down means that the player doubles his bet, takes one more card and stands.", 0dh, 0ah

		BYTE "The dealer plays his cards by always standing if the value is 17 or greater and always hitting if the value is 16 and under.", 0dh, 0ah 

		BYTE "The Goal of the game is to get a card value of 21 or to have a better hand than the dealer.", 0dh, 0ah  
		BYTE "If the cards total to more than 21 that is a bust and that player loses his bet.", 0dh, 0ah  
		BYTE "For the players that have values under 21 the hand is then compared to the dealers hand.", 0dh, 0ah  
		BYTE "If the player has a lower value than the dealer they lose their bet.", 0dh, 0ah  
		BYTE "If the values are the same it is a push and the player receives their bet back.", 0dh, 0ah  
		BYTE "If the value is greater than the dealer they win the hand and receives twice their bet back.", 0dh, 0ah  
		BYTE "If the dealer busts all players that did not bust win and receive twice their bet back.", 0


	splitPrompt BYTE "Split is a choice if the players original two cards have the same value,", 0dh, 0ah
		BYTE "the player can choose to split and make two hands being dealt a second card for each of ", 0dh, 0ah
		BYTE "the originals and betting an equal amount as the original bet for the new second hand.  The player then plays both hands separately.", 0

	hitPrompt BYTE "Hit is to take another card and then the player has the choice to stand, hit, or double down after that.", 0

	standPrompt BYTE "Stand means that the player does not take anymore cards and his decisions are done.", 0 

	doubleDownPrompt BYTE "Double down means that the player doubles his bet, takes one more card and stands.", 0

	help BYTE "Help box", 0

	card1 DWORD 10
	card2 DWORD 8

.code

Help PROC USES EAX EBX
	mov eax, card1
	mov ebx, card2
	cmp eax, ebx
	JE split

	add eax, ebx
	cmp eax, 17
	JAE stand
	JB hit

	jmp endit

	split: 
		call splitHelp
		jmp endit

	hit:
		call hitHelp
		jmp endit

	stand:
		call standHelp
		jmp endit

	doubleDown:
		call doubleDownHelp
		jmp endit
	
	endit:


	RET
Help ENDP

rules PROC USES EBX EDX
	mov ebx, offset help
	mov edx, offset rulesPrompt
	call MsgBox
	
	RET
rules ENDP

splitHelp PROC USES EBX EDX
	mov ebx, offset help
	mov edx, offset splitPrompt
	call MsgBox

	RET
splitHelp ENDP

hitHelp PROC USES EBX EDX
	mov ebx, offset help
	mov edx, offset hitPrompt
	call MsgBox

	RET
hitHelp ENP

standPrompt PROC USES EBX EDX
	mov ebx, offset help
	mov edx, offset standPrompt
	call MsgBox

	RET
standPrompt ENDP

doubleDownPrompt PROC USES EBX EDX
	mov ebx, offset help
	mov edx, offset doubleDownPrompt
	call MsgBox

	RET
doubleDownPrompt ENDP