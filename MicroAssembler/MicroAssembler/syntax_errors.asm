; Created:  21.04.2020 10:56:17
; Authors:	Claudia Polaczek
;			Michal Granda
;			Micha³ Padula
; Info:		Example program with syntax errors
;
.include "m328pdef.inc"

.equ LED = 4				; Invalid label - missing "="
.equ BUTTONS = 1		; Label cannot be assigned to label

.cseg
.org 0x0000
	/* Set up stack addresss */
		ldi R16, HIGH(RAMEND)
		out SPH, R16 
		ldi R16, LOW(RAMEND)
		out SPL, 0x23			; Invalid operand - expecting working register R0 - R32
	/* Configure IO lines */
		sbi R10, LED			; Invalind operand - expecting IO register
		cbi PORTB, LED
		ldi R17, BUTTONS
		out DDRC				; Instruction requires two operands
		ldi R17,				; Missing operand - value or label
		out PORTC, R17
	/* main loop */
		ldi R18, 20
		jmp R14					; Invalid operand - address is required