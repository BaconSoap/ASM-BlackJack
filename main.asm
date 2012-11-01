TITLE MASM Template						(main.asm)

; Description:
; 
; Revision date:

INCLUDE Irvine32.inc

.data
Instruction DB "Enter a number", 0DH, 0AH, 0
EaxPrint DB "First number is now", 0DH, 0AH, 0
EbxPrint DB "Second number is now", 0DH, 0AH, 0
arrayal DB 50 DUP(?)

.code
main PROC
	call Clrscr
	call ClearRegs
	mov edx, OFFSET Instruction
	CALL WriteString
	CALL ReadInt
	MOV ECX, EAX
	mov edx, offset Instruction
	CALL WriteString
	CALL READINT
	MOV ESI, EAX
	CALL ExchangeNow
	MOV EAX, ECX
	mov edx, offset EaxPrint
	CALL WriteString
	call WriteInt
	call CrLf
	mov eax, ESI
	mov edx, offset EbxPrint
	CALL WriteString
	call WriteInt
	call crlf
	exit
main ENDP

ExchangeNow PROC
	xchg ECX, ESI
	ret
ExchangeNow ENDP

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