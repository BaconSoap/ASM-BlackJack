.data


.code

RunGame PROC
		CALL InitializeDeck
		
		CALL NameInput
		
		CALL ShuffleDeck
		;call testdeal
		call dealcard
		mov eax,1 ; loop will run through first for player one
		MOV ECX, PlayerCount

		;gets all players bets before dealing cards
		BettingLoop:
			CALL placebet
			inc EAX
		Loop BettingLoop

;initial deal for all players
		mov eax,1 ; 
		MOV ECX, PlayerCount
	dealLoop:


	Loop dealLoop

	call outputplayers


ret
RunGame ENDP