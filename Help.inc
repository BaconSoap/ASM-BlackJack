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


	splitPrompt BYTE "Your odds are best if you split this hand", 0

	hitPrompt BYTE "Your odds are greatest if you hit on this hand", 0

	standPrompt BYTE "Your odds are best if you stand", 0 

	doubleDownPrompt BYTE "Your odds are best if you double down", 0

	help BYTE "Help box", 0

	card1Prompt BYTE "enter card 1:  ", 0
	card2Prompt BYTE "enter card 2: ", 0
	dealerPrompt BYTE "enter dealer card:  ", 0

	card1 DWORD 2
	card2 DWORD 2
	dealerCard DWORD 3
	total DWORD 0

.code

hint PROC USES EAX EBX
	mov eax, card1
	mov ebx, card2
	mov total, eax
	add total, ebx

	cmp eax, ebx
	JE pair

	cmp card1, 11
	JE soft
	cmp card2, 11
	JE soft
	jmp hard

	soft:
		cmp total, 15
		JBE hit
		
		cmp total, 18
		JBE soft1618
		
		jmp stand

		soft1618:
			cmp dealerCard, 6
			JBE doubleDown
			jmp hit

	hard:
		cmp total, 8
		JBE hit

		cmp total, 9
		JE hard9

		cmp total, 11
		JBE hard1011

		cmp total, 16
		JBE hard1216

		jmp stand

		hard9:
			cmp dealerCard, 6
			JBE doubleDown
			jmp hit

		hard1011:
			mov eax, total
			cmp eax, dealerCard
			JA doubleDown
			jmp hit

		hard1216:
			cmp dealerCard, 6
			JBE stand
			jmp hit

	pair: 
		cmp eax, 2
		JE twos

		cmp eax, 3
		JE threes

		cmp eax, 4
		JE fours

		cmp eax, 5
		JE fives

		cmp eax, 6
		JE sixes

		cmp eax, 7
		JE sevens

		cmp eax, 8
		JE Split

		cmp eax, 9
		JE nines

		cmp eax, 11
		JE Split

		jmp stand

		Split:
			call splitHint
			jmp endit

		twos:
			cmp dealerCard, 8
			JAE hit
			jmp split

		threes:
			cmp dealerCard, 9
			JAE hit
			jmp split

		fours:
			cmp dealerCard, 2
			JBE hit
			cmp dealerCard, 7
			JAE hit
			jmp split

		fives:
			mov eax, total
			cmp eax, dealerCard
			JA doubleDown
			jmp hit

		sixes:
			cmp dealerCard, 8
			JAE hit
			jmp split

		sevens:
			cmp dealerCard, 7
			JAE hit
			jmp split

		nines:
			cmp dealerCard, 7
			JE stand
			cmp dealerCard, 10
			JE stand
			cmp dealerCard, 11
			JE stand
			jmp split

	hit:
		call hitHint
		jmp endit

	stand:
		call standHint
		jmp endit

	doubleDown:
		call doubleDownHint
		jmp endit
	
	endit:


	RET
hint ENDP

rules PROC USES EBX EDX
	mov ebx, offset help
	mov edx, offset rulesPrompt
	call MsgBox
	
	RET
rules ENDP

splitHint PROC USES EBX EDX
	mov ebx, offset help
	mov edx, offset splitPrompt
	call MsgBox

	RET
splitHint ENDP

hitHint PROC USES EBX EDX
	mov ebx, offset help
	mov edx, offset hitPrompt
	call MsgBox

	RET
hitHint ENDP

standHint PROC USES EBX EDX
	mov ebx, offset help
	mov edx, offset standPrompt
	call MsgBox

	RET
standHint ENDP

doubleDownHint PROC USES EBX EDX
	mov ebx, offset help
	mov edx, offset doubleDownPrompt
	call MsgBox

	RET
doubleDownHint ENDP