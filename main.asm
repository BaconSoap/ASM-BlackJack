TITLE Blackjack						(main.asm)

; Description:
; 
; Revision date: 11/3/12

INCLUDE Irvine32.inc

.data

.code
main PROC
	call Clrscr
	call ClearRegs

	exit
main ENDP

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