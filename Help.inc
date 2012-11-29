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

	dealerCard BYTE ?
	handValueHint BYTE ?

.code

;pass player number in EAX
hint PROC 
	PushAD
	
	push eax
	call getplayerhand
	call gethandvalue
	mov handValueHint, al

	mov eax, 0
	mov esi, offset dealer.phand
	mov al,[esi]
	call getcardvalue
	mov dealercard, al

	pop eax

	call handtypePrompt

	mov esi, 0
			;this will add the type of playerstructure to esi, making it go to the corresponding player in the array	
	.while (eax > 1)
		add esi, TYPE Playerstructure
		dec eax
	.endw
		
	Mov cl, (Playerstructure PTR playerarray[esi]).cardCount
	mov al, (Playerstructure PTR playerarray[esi]).phand
	call getCardvalue
	mov ah, al

	mov eax, 0
	mov al, (Playerstructure PTR playerarray[esi]).phand[1]
	call getcardvalue

	;al is value of card 2
	;ah is value of card 1
	;cl is card count

	.if (cl ==2) && (ah == al)
		JE pair
	.endif

	call gethandvalue
	mov bl, handType

	;al is hand value
	;bl is soft or hard
	
	cmp bl, 'S'
	JE soft
	jmp hard

	soft:
		cmp al, 15
		JBE hit
		
		cmp al, 18
		JBE soft1618
		
		jmp stand

		soft1618:
			cmp dealerCard, 6
			JBE doubleDown
			jmp hit

	hard:
		cmp al, 8
		JBE hit

		cmp al, 9
		JE hard9

		cmp al, 11
		JBE hard1011

		cmp al, 16
		JBE hard1216

		jmp stand

		hard9:
			cmp dealerCard, 6
			JBE doubleDown
			jmp hit

		hard1011:
			cmp al, dealerCard
			JA doubleDown
			jmp hit

		hard1216:
			cmp dealerCard, 6
			JBE stand
			jmp hit

	pair: 
		cmp al, 2
		JE twos

		cmp al, 3
		JE threes

		cmp al, 4
		JE fours

		cmp al, 5
		JE fives

		cmp al, 6
		JE sixes

		cmp al, 7
		JE sevens

		cmp al, 8
		JE Split

		cmp al, 9
		JE nines

		cmp al, 11
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
			cmp dealerCard, 10
			JB doubleDown
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

	PopAD
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

.data
	type1 BYTE "You have a hard ",0
	type2 BYTE "You have a soft ",0
	dealercardprompt BYTE " and the dealer is showing a ", 0

.code
handTypePrompt PROC
	pushad
	.IF handtype == 'H'
		mov edx, offset type1
	.ELSE
		mov edx, offset type2
	.ENDIF
	call writeString
	call getplayerhand
	call gethandvalue
	call writedec
	mov edx, offset dealercardprompt
	call writestring
	mov eax, 0
	mov al, dealerCard
	call writeDec
	call crlf
	
	popad
	ret
handTypePrompt ENDP