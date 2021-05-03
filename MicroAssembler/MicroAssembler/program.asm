; Created:  21.04.2020 10:56:17
; Authors:	Claudia Polaczek
;			Michal Granda
;			Micha³ Padula
; Info:		Program for programming demostration
;
.include "m328pdef.inc"

.equ LED_RED	= 5	; PC5
.equ LED_GREEN	= 4	; PC4
.equ LED_BLUE	= 3	; PC3

.equ BUTTON_1	= 0b10000000	; PD7
.equ BUTTON_2	= 0b01000000	; PD6
.equ BUTTON_3	= 0b00100000	; PD5

.cseg
.org 0x0000
	/* Set up stack addresss */
			ldi R16, HIGH(RAMEND)	; Use of built in HIGH function
			out SPH, R16 
			ldi R16, LOW(RAMEND)	; Use of built in LOW function
			out SPL, R16
	/* Configure IO lines */
			SBI DDRC, LED_RED
			SBI DDRC, LED_GREEN
			SBI DDRC, LED_BLUE

			CBI PORTC, LED_RED
			CBI PORTC, LED_GREEN
			CBI PORTC, LED_BLUE

			LDI R16, 0b00011111
			OUT DDRD, R16
			LDI R16, 0b11111111
			OUT PORTD, R16

	/* main loop */
loop:		IN R17, PIND
			LDI R20, BUTTON_2
			AND R17, R20
			SUB R17, R20
			BREQ LED_RED_OFF
			BRNE LED_RED_ON

cont_green:	IN R17, PIND
			LDI R20, BUTTON_1
			AND R17, R20
			SUB R17, R20
			BREQ LED_GREEN_OFF
			BRNE LED_GREEN_ON

cont_blue:	IN R17, PIND
			LDI R20, BUTTON_3
			AND R17, R20
			SUB R17, R20
			BREQ LED_BLUE_OFF
			BRNE LED_BLUE_ON

LED_RED_ON:		SBI PORTC, LED_RED
				JMP cont_green

LED_RED_OFF:	CBI PORTC, LED_RED
				JMP cont_green

LED_GREEN_ON:	SBI PORTC, LED_GREEN
				JMP cont_blue

LED_GREEN_OFF:	CBI PORTC, LED_GREEN
				JMP cont_blue

LED_BLUE_ON:	SBI PORTC, LED_BLUE
				JMP loop

LED_BLUE_OFF:	CBI PORTC, LED_BLUE
				JMP loop