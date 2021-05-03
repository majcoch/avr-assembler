; Created:  21.04.2020 10:56:17
; Authors:	Claudia Polaczek
;			Michal Granda
;			Micha³ Padula
; Info:		Simple assembly program for testing custom assembler tool
;
.include "m328pdef.inc"

.equ LED = 4
.equ BUTTONS = 0x00

.cseg
.org 0x0000
	/* Set up stack addresss */
		ldi R16, HIGH(RAMEND)	; Use of built in HIGH function
		out SPH, R16
		ldi R16, LOW(RAMEND)	; Use of built in LOW function
		out SPL, R16
	/* Configure IO lines */
		sbi DDRB, LED			; Set bit in DDRB register - out
		cbi PORTB, LED			; diable pull-up resistor
		ldi R17, BUTTONS
		out DDRC, R17			; Clear all bits in DDRC register - in
		ldi R17, 0xFF
		out PORTC, R17			; Enable pull-up resistors on all inputs
	/* main loop */
		ldi R18, 20				; Number of iterations
		ldi R19, 0				; Loop counter
loop:	SUB R18, R19			; Test end condition
		BREQ end				; Branch if equals
		INC R19					; Increment loop counter

		IN R20, PINC			; Read buttons state
		LDI R21, 0b00010000		; Load mask value
		AND R20, R21			; Perform masking
		OUT PORTB, R20			; Copy to output register

		jmp loop

	/* Endless loop */
end:	jmp end