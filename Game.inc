.data
welcomeMsg BYTE "Welcome to the wonderful world of BlackJack!",0dh,0ah,0
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
;if dealer busted set to 1
DealerBust BYTE  0
DealerHandVal BYTE 0
playerChoice BYTE 0
CURRENTPLAYER BYTE 0
IsSplitTurn		DB	0
;stores first two card values to check for splitting
FCardVal BYTE 0
SCardVal BYTE 0
.code

RunGame PROC
	;creates deck, shuffles deck asks for names

		CALL Crlf
		CALL InitializeDeck
		CALL ShuffleDeck		
	
		MOV DealerBust, 0
	;-------------------------------------------
	
	;Ask each player for an initial bet, loops as many times as necessary
		MOV EAX, 1 
		MOV ECX, PlayerCount
		BettingLoop:
			call setPlayerColor
			CALL placebet
			INC EAX
			call setDefaultTxtColor
		Loop BettingLoop

	;initial deal for dealer
		MOV ECX, 2
		DEALLOOP:
			CALL DealCard
			MOVZX EBX, AL
			CALL AddCardDealer
	;---------------------------
		;make initial deals for players
			MOV EAX, 1 
			PUSH ECX
			MOV ECX, PlayerCount
			InitialDeal:
				push eax
				call dealcard
				movzx ebx, al
				pop eax
				call addcardplayer
				inc eax
			Loop InitialDeal
			POP ECX
		Loop DEALLOOP
		;--------------------------------

	call DisplayAll
	;call showdealerhand
	;

	;LOOP THROUGH AND GIVE EACH PLAYER A chance to select an option
		mov EAX, 1
		mov ecx, Playercount
		taketurn:
			CALL DisplayAll
			mov currentplayer, al
			call Playerturn
			inc eax
		loop taketurn

	call DisplayAll
	call dealerturn
	;call showdealerhand
			
	;compare each player hand value to dealer hand value if dealer busted this is skipped
	CMP DealerBust, 1
	JE dealerlose
		mov EAX, 1
		mov ecx, Playercount
		compare:
			call setPlayerColor
			mov currentplayer, al
			call compareHand
			call checkSplit
			.IF(bl==1)
				call compareSplitHand
			.ENDIF
			inc eax
			call setDefaultTxtColor
		loop compare
		jmp endround

	dealerlose:
		mov EAX, 1
		mov ecx, Playercount
		checkPlayer:
			mov currentplayer, al
			push eax
			call getplayerhand
			call gethandvaluenew
			.IF (al<=21)
				pop eax
				call playerWin
			.ElSE 
				pop eax
				call playerLose
			.ENDIF

			call checkSplit
			.IF(bl==1)
				call compareSplitHand
				mov currentplayer, al
				push eax
				call getplayerSplithand
				call gethandvaluenew
				.IF (al<=21)
					pop eax
					call playerSplitWin
				.ElSE 
					pop eax
					call playerLose
				.ENDIF

			.ENDIF
			inc eax
		loop checkPlayer

	endround:
	call DisplayAll

ret
RunGame ENDP




PlayerTurn PROC USES EAX EDX ESI ECX
pushad

turnloop:
	MOV IsSplitTurn, 0
	push EAX
	movzx eax, currentplayer
	call DisplayAll
	
	pop eax
choosemenu:
	MOV EDX, offset selectionMsg
	CALL WriteString
	CALL Crlf
	push eax
	call readint

	MOV playerchoice, al
	
	.if playerchoice == 1
		CALL DealCard
		movzx ebx, al
		pop eax
		call addcardplayer
		;checkhand value, jump to end turn if over 21


	.elseif playerchoice == 2
		pop eax
		JMP endturn

		;double down, only allows you to hit once
	.elseif playerchoice == 3
		pop eax
		call doublebet
		push eax
		call dealcard
		movzx ebx, al
		pop eax
		call addcardplayer
		movzx eax, currentplayer
		call testhand
		jmp endturn
	.elseif playerchoice == 4
	pop eax
		pushad
		mov ebx, 0
			mov bl, FCardVal
			.if (bl == SCardval)
				popad
				call dumpregs
				Call splithand
			.else
				popad
				CALL DisplayAll
				mov edx, offset nosplitmsg
				call writestring 
				
				JMP choosemenu
			.endif
			

	.elseif playerchoice == 5  ;rules
		pop eax
		call rules
	.elseif playerchoice == 6  ;hint
		pop eax
		call hint
	.elseif playerchoice == 7  ;exit
		pop eax
		exit
	.else
		call dumpregs
		JMP choosemenu

	.endif	

		push eax
		call getplayerhand
		call gethandvaluenew
		cmp eax, 21
		JG Busted
		pop eax
		JMP turnloop
	Busted:
		MOV EDX, OFFSET Bustedmsg
		call writestring
		pop eax
	endturn:


	call checkSplit  

	.IF (bl == 1)
	turnloopSplit:
	MOV IsSplitTurn, 1
	push EAX
	movzx eax, currentplayer
	call DisplayAll

	MOV EDX, offset selectionMsg
	CALL WriteString
	CALL Crlf
	call readint

	MOV playerchoice, al

	.if playerchoice == 1
		CALL DealCard
		movzx ebx, al
		pop eax
		call addcardplayerSplit
		;checkhand value, jump to end turn if over 21


	.elseif playerchoice == 2
		pop eax
		JMP endturnSplit

		;double down, only allows you to hit once
	.elseif playerchoice == 3
		pop eax
		call doubleSplitbet
		push eax
		call dealcard
		movzx ebx, al
		pop eax
		call addcardplayerSplit
		movzx eax, currentplayer
		call testSplithand
		jmp endturnSplit
	.elseif playerchoice == 4
		push edx
		mov edx, offset nosplitmsg
		call writestring
		pop edx
		jmp turnloopsplit
	.elseif playerchoice == 5  ;rules
		pop eax
		call rules
	.elseif playerchoice == 6  ;hint
		pop eax
		call hint
	.endif	

		push eax
		call getplayerSplithand
		call gethandvaluenew
		cmp eax, 21
		JG BustedSplit
		pop eax
		JMP turnloopSplit
	BustedSplit:
		MOV EDX, OFFSET Bustedmsg
		call writestring
		pop eax
	endturnSplit:
	
	.ENDIF

popad
ret
PlayerTurn ENDP

;dealers moves
Dealerturn PROC
pushad

	call getdealerhand
	call gethandvaluenew
		mov DealerHandVal, al
;	call writeint
	
	.while (DealerHandVal < 17)
		Call dealcard
		movzx ebx, al
		call addcarddealer
		call getdealerhand
		call gethandvaluenew
		mov DealerHandVal, al
		call writeint
	.endw
	.if (AL > 21)
		MOV EDX, offset DBustMsg
		call writestring
		MOV DealerBust, 1
	.endif
popad
ret
dealerturn ENDP
