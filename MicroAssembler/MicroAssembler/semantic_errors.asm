; Created:  21.04.2020 10:56:17
; Authors:	Claudia Polaczek
;			Michal Granda
;			Micha³ Padula
; Info:		Example program with semantic errors
;
.include "m328pdef.inc"

.equ LED = 4
.equ BUTTONS = 18

.cseg
.org 0x0000
	/* Set up stack addresss */
		ldi R16, RAMEND			; Argument out of range 0x00 - 0xFF (8 bit value)
		out SPH, R16 
		ldi R16, 0xFFFF			; Argument out of range 0x00 - 0xFF (8 bit value)
		out SPL, R16
	/* Configure IO lines */
		sbi DDRB, 10			; Argument 2 out of range 0 - 7
		cbi 0xFF, LED			; Argument 1 out of range 
		ldi R0, BUTTONS			; Argument 1 out of range - R16 - R31
		out DDRC, R17			; Clear all bits in DDRC register - in
		ldi R17, 0xFF
		out PORTC, R17			; Enable pull-up resistors on all inputs
	/* main loop */
ET:		ldi R2, 20				; Argument 1 out of range - R16 - R31

.org 0x0100
		breq ET					; Relative branch out of reach