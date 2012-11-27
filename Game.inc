.data
welcomeMsg BYTE "Welcome to the wonderful world of BlackJack!",0dh,0ah,0
SelectionMsg Byte "Please select one of the following",0dh,0ah
			 BYTE "1. HIT ",0dh,0ah
			 BYTE "2. STAY ",0dh,0ah
			 BYTE "3. DOUBLE DOWN ",0dh,0ah
			 BYTE "4. SPLIT ",0dh,0ah,0
Bustedmsg BYTE "Sorry, you busted",0dh,0ah,0
playerChoice BYTE 0
CURRENTPLAYER BYTE 0
.code

RunGame PROC
	;creates deck, shuffles deck asks for names
		MOV EDX, offset welcomeMsg
		Call WriteString
		CALL Crlf
		CALL InitializeDeck
		CALL ShuffleDeck		
		CALL NameInput
	;-------------------------------------------
	
	;Ask each player for an initial bet, loops as many times as necessary
		MOV EAX, 1 
		MOV ECX, PlayerCount
		;BettingLoop:
		;	CALL placebet
		;	INC EAX
		;Loop BettingLoop

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

	call outputplayers
	call showdealerhand
	;

	;LOOP THROUGH AND GIVE EACH PLAYER A chance to select an option
		mov EAX, 1
		mov ecx, Playercount
		taketurn:
			mov currentplayer, al
			call Playerturn
			inc eax
		loop taketurn
	
	call outputplayers
	call dealerturn
	call showdealerhand
		
ret
RunGame ENDP




PlayerTurn PROC
pushad

turnloop:
	push EAX
	movzx eax, currentplayer
	call testhand

	MOV EDX, offset selectionMsg
	CALL WriteString
	CALL Crlf
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
		;splitting goes here

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
popad
ret
PlayerTurn ENDP