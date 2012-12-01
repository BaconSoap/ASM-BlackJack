.data
welcomeMsg BYTE "Welcome to the wonderful world of BlackJack!",0dh,0ah
			BYTE "Each player starts with $300", 0dh, 0ah, 0
SelectionMsg Byte "Please select one of the following",0dh,0ah
			 BYTE "1. HIT ",0dh,0ah
			 BYTE "2. STAND ",0dh,0ah
			 BYTE "3. DOUBLE DOWN ",0dh,0ah
			 BYTE "4. SPLIT ",0dh,0ah
			 BYTE "5. RULES ", 0dh, 0ah
			 BYTE "6. HINT ", 0dh, 0ah
			 BYTE "7. EXIT ", 0dh, 0ah, 0
Bustedmsg BYTE "Sorry, you busted",0dh,0ah,0
DBustMsg BYTE "Dealer has busted!",0dh,0ah,0
nosplitmsg BYTE "You cannot split with this hand!",0dh,0ah,0

DealerBust BYTE  0		;set to '1' if he busts.
DealerHandVal BYTE 0	;stores dealer hand overall value
PlayerChoice BYTE 0		;holds (1-7) which represents current player's selection choice
CurrentPlayer BYTE 0	;holds (1-5) which represents current player's turn
IsSplitTurn		DB	0	;set to '1' if playing split hand turn
DealerShowAll	DB	0	;set to '1' to show all hand at end of game, otherwise will show one card
NumberOfBust	DD	0	;keeps number of players that busted
ShouldShowDoubleError	DD	0 ; set to '1' if unable to double down
ActivePlayers	DD 0	;holds number of players still in game 
FCardVal BYTE 0			;stores first two card values to check for splitting
SCardVal BYTE 0			;-^-

.code
RunGame PROC		; main block that holds critical CALLs and functions that run the game
	CALL Crlf			
	CALL InitializeDeck	; creates Deck array
	CALL ShuffleDeck	; shuffles Deck array elements	
	MOV DealerBust, 0 ; resets dealerbust for each run of the game
	; following code is used for betting
	MOV EAX, 1 			; makes bettingloop start with player one		
	MOV ECX, PlayerCount
		BettingLoop:
			PUSH EAX
			MOV ESI, 0
				.WHILE (EAX > 1)	; goes through structure array to manipulate correct player
					ADD ESI, TYPE Playerstructure
					DEC EAX
				.ENDW
			MOV EAX, (Playerstructure PTR playerarray[esi]).bankacc ; manipulate players bank
				.IF EAX >= 10			; bet must be above 10
					POP EAX
					CALL setPlayerColor	; gets unique color
					CALL PlaceBet		; places bet
				.ELSE
					POP EAX
				.ENDIF
			INC EAX
		Loop BettingLoop
	;-----------------------------------------------------------
	; initial deal loop runs twice, dealing a single card for the deal and each player
	MOV ECX, 2
		DealLoop:
			CALL DealCard ; returns top card from deck
			MOVZX EBX, AL
			CALL AddCardDealer ; adds card to dealer
			MOV EAX, 1 
			PUSH ECX
			MOV ECX, PlayerCount
				InitialDeal:
					PUSH EAX
					CALL dealcard ;returns top card from deck
					MOVZX EBX, AL
					POP EAX
					CALL AddCardPlayer ; adds card to player
					INC EAX
				Loop InitialDeal
			POP ECX
		Loop DealLoop
		;--------------------------------------------------------------------------------------
	CALL DisplayAll ; show all players and dealer
	; take turn loop gives each player the choice to select an option
	MOV EAX, 1
	MOV ECX, PlayerCount
	MOV NumberOfBust, 0	; resets bust count to 0 for each new run
		TakeTurn:		
			PUSH EAX
			MOV ESI, PlayerCount
			MOV ActivePlayers, ESI
			MOV ESI, 0
				.WHILE (EAX > 1)
					ADD ESI, TYPE Playerstructure
					DEC EAX
				.ENDW
			MOV EAX, (Playerstructure PTR playerarray[esi]).bankacc
			MOV EBX, (Playerstructure PTR playerarray[esi]).bet
				.IF EAX < 10 && EBX < 10	; ensures sufficient funds are in bank for betting
					DEC ActivePlayers
					POP EAX
				.else
					POP EAX
					MOV DealerShowAll, 0	; dealer will show only one card
					CALL DisplayAll			
					MOV CurrentPlayer, AL
					CALL PlayerTurn			; current player can take his turn		
				.ENDIF
					.IF ActivePlayers <= 0	; ends game if no more players can make bets
						EXIT
					.ENDIF
			INC EAX	; next player will run through this loop
		LOOP TakeTurn
	;-----------------------------------------------------------------------------------------------
	CALL DisplayAll
	MOV EDX, PlayerCount ;if all players are bust skips dealer's turn
	CMP NumberOfBust, EDX
	JE NoDealerTurn			
	CALL DealerTurn			; dealer takes turn
	NoDealerTurn:			; skips dealer turn
	MOV DealerShowAll, 1	; dealer will show whole hand
	CALL DisplayAll
	CMP DealerBust, 1		; if dealer busted jump to dealerlose
	JE DealerLose			; -^-
	MOV EAX, 1
	MOV ECX, PlayerCount
	;compare each player hand value and player split hand value to dealer hand value if dealer busted this is skipped
		Compare:
			MOV CurrentPlayer, AL
			CALL CompareHand
			CALL CheckSplit	; if split, compare to dealer
			.IF(BL==1)
				CALL CompareSplitHand
			.ENDIF
			INC EAX
		LOOP Compare
	;----------------------------------------------------------------------------------------
	JMP EndRound
	DealerLose:
	MOV EAX, 1
	MOV ECX, PlayerCount
	; checkplayer loop looks at each players hand value and chooses correct procedure
		CheckPlayer:
			MOV CurrentPlayer, AL
			PUSH EAX
			CALL GetPlayerHand
			CALL GetHandValueNew
				.IF (AL<=21)	; if hand value is 21 or less
					POP EAX
					CALL playerWin
				.ElSE			; if over 21 
					POP EAX
					CALL playerLose
				.ENDIF
			CALL checkSplit		; looks for split hand
				.IF(bl==1)
					CALL compareSplitHand
					MOV currentplayer, AL
					PUSH EAX
					CALL GetPlayerSplitHand
					CALL gethandvalueNew
					.IF (al<=21) ; if hand value is 21 or less
						pop eax
						CALL playerSplitWin
					.ElSE		; if over 21
						pop eax
						CALL playerLose
				.ENDIF
			.ENDIF
			inc eax
		loop checkPlayer
	;--------------------------------------------------------------------------------
	endround:
	CALL DisplayAll
ret
RunGame ENDP



;Input: player number in EAX
;Output: input menu for player 
PlayerTurn PROC 
PUSHAD

	turnloop:	
	MOV IsSplitTurn, 0	; checks if player is making actions for split hand
	PUSH EAX
	MOVzx eax, currentplayer
	CALL DisplayAll
	pop eax
	choosemenu:			; can't double down if restrictions aren't met
		.if ShouldShowDoubleError == 1
			MOV EDX, OFFSET CantDoubleDown
			CALL WriteString
			MOV ShouldShowDoubleError, 0
		.endif
	MOV EDX, offset selectionMsg	; displays selction menu
	CALL WriteString
	CALL Crlf
	PUSH EAX
	CALL GetPlayerHand
	CALL GetHandValueNew
	CMP EAX, 21
	JE ItBlackJack	 
	JMP NotABlackJack
	ItBlackJack: ; if black jack, turn will end
	POP EAX		
	JMP EndTurn
	NotABlackJack:
	POP EAX
	PUSH eax
	CALL readint
	MOV playerchoice, al		; stores player input
		.if playerchoice == 1	; deals a card to players hand
			CALL DealCard
			MOVzx ebx, al
			pop eax
			CALL addcardplayer
		.elseif playerchoice == 2 ; stand, ends player's turn
			pop eax
			JMP endturn
		.elseif playerchoice == 3 ; double down, doubles bet and hits only once
			pop eax
			CALL doublebet
				.if ShouldShowDoubleError == 0	
					PUSH eax
					CALL dealcard
					MOVzx ebx, al
					pop eax
					CALL addcardplayer
					MOVzx eax, currentplayer
					CALL testhand
					jmp endturn
				.endif
		.elseif playerchoice == 4 ; splits a players hand
			pop eax
			PUSHad
			MOV ebx, 0
			MOV bl, FCardVal
				.if (bl == SCardval); if both cards are equal, split
					popad
					CALL splithand
				.else				; else don't split and choose menu option again
					popad
					CALL DisplayAll
					MOV edx, offset nosplitmsg
					CALL writestring 
					JMP choosemenu
				.endif
		.elseif playerchoice == 5  ; shows rules
			pop eax
			CALL rules
		.elseif playerchoice == 6  ; shows hit for hand
			pop eax
			CALL hint
		.elseif playerchoice == 7  ; exit game
			pop eax
			exit
		.else						; invalid input, look at 
			pop eax
			JMP turnloop
		.endif	
	PUSH eax
	CALL getplayerhand
	CALL gethandvaluenew
	cmp eax, 21	; if player hand is 21 or less send back to continue turn otherwise endturn because of bust
	JG Busted
	pop eax
	JMP turnloop
		Busted:	; player busted, shows message and increments number of busted for this hand
			pop eax
		MOV CurrentPlayerMessage, C_PLAYERBUST
		CALL SetPlayerMessage
		INC NumberOfBust
		
		endturn:
	CALL checkSplit  ; takes turn for split hand if there exists one for player, identical functionality to normal player turn
		.IF (bl == 1)
			turnloopSplit:
			MOV IsSplitTurn, 1
			PUSH EAX
			MOVzx eax, currentplayer
			CALL DisplayAll
			MOV EDX, offset selectionMsg
			CALL WriteString
			CALL Crlf
			CALL readint
			MOV playerchoice, al
				.if playerchoice == 1 ; hit
					CALL DealCard
					MOVzx ebx, al
					pop eax
					CALL addcardplayerSplit	
				.elseif playerchoice == 2 ; stand
					pop eax
					JMP endturnSplit
				.elseif playerchoice == 3	; double down, makes sure you can
					pop eax
					CALL doubleSplitbet
					PUSH eax
					CALL dealcard
					MOVzx ebx, al
					pop eax
					CALL addcardplayerSplit
					MOVzx eax, currentplayer
					CALL testSplithand
					jmp endturnSplit
				.elseif playerchoice == 4	; split, makes sure you can
					PUSH edx
					MOV edx, offset nosplitmsg
					CALL writestring
					pop edx
					jmp turnloopsplit
				.elseif playerchoice == 5  ;rules
					pop eax
					CALL rules
				.elseif playerchoice == 6  ;hint
					pop eax
					CALL hint
				.elseif playerchoice == 7  ; exit game
					pop eax
					exit
				.else						; invalid input, loop back
					pop eax
					JMP turnloopsplit
				.endif	
			PUSH eax
			CALL getplayerSplithand
			CALL gethandvaluenew
			cmp eax, 21
			JG BustedSplit	
			pop eax
			JMP turnloopSplit
			BustedSplit:
			MOV EDX, OFFSET Bustedmsg
			CALL writestring
			pop eax
			endturnSplit:
		.ENDIF
popad
ret
PlayerTurn ENDP

;Output: deal hits if under 17 and stands if 17 or greater
Dealerturn PROC
PUSHAD
	CALL getdealerhand
	CALL gethandvaluenew
	MOV DealerHandVal, al
		.while (DealerHandVal < 17) ; if under 17, user must hit
			CALL dealcard
			MOVzx ebx, al
			CALL addcarddealer
			CALL getdealerhand
			CALL gethandvaluenew
			MOV DealerHandVal, al
			CALL writeint
		.endw
		.if (AL > 21)				; checks if dealer busted
			MOV EDX, offset DBustMsg
			CALL writestring
			MOV DealerBust, 1
		.endif
popad
ret
dealerturn ENDP
