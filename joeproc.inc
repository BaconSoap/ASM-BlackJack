.data
DBustMsg BYTE "Dealer has busted!",0dh,0ah,0
.code

;does all actions for dealer hits until hand equals at least 17

Dealerturn PROC
pushad

	call getdealerhand
	call gethandvalue
	call writeint
	
	.while (AL < 17)
		Call dealcard
		movzx ebx, al
		call addcarddealer
		call getdealerhand
		call gethandvalue
		call writeint
	.endw
	.if (AL > 21)
		MOV EDX, offset DBustMsg
		call writestring
	.endif
popad
ret
dealerturn ENDP