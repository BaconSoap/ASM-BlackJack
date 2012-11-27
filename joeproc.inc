.data
DBustMsg BYTE "Dealer has busted!",0dh,0ah,0
;if dealer busted set to 1
DealerBust BYTE  0
DealerHandVal BYTE 0
.code

;does all actions for dealer hits until hand equals at least 17

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