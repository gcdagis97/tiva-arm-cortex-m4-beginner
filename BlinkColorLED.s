;This program will turn the blue led off, delay, turn blue led ON, delay, then repeat... 
;9/14/2017

;	GPIO Pin	B7,B6,B5,B4,B3,B2,B1,B0		Function
;	PF1			0, 0, 0, 0, 0, 0, 1, 0		RED
;	PF2			0, 0, 0, 0, 0, 1, 0, 0		BLUE
;	PF3			0, 0, 0, 0, 1, 0, 0, 0		GREEN

;	MOV		R8, #0x02	R
;	MOV		R8, #0x04	B
;	MOV		R8, #0x08	G

AREA    bli,	CODE,	READONLY 	;Create a memory section for code that is read-only
ENTRY
		
SYSCTL_RCGC2	EQU		0x400FE108	;Address for SYSCTL_RCGC2
GPIOF_DIR 		EQU		0x40025400	;Address for GPIOF_DIR
GPIOF_DEN		EQU		0x4002551C	;Address for GPIOF_DEN
GPIOF_DATA		EQU		0x400253FC	;Address for GPIOF_DATA	
	
EXPORT __main
__main
	
;Initialization of LED begins here
INIT_LED
	
	;Turns on clock for port F
	LDR		R7, =SYSCTL_RCGC2 			;Load content of RCGC2 into R7. Puts 0x400FE108 in R7
	MOV		R8, #0x020					;Store value 0x020 into R8. Turns on bit 5 of this address 0x400FE108
	STR		R8, [R7]					;Store the value of R8 back into RCGC2
	
	LDR		R7, =GPIOF_DIR 				;Load content of GPIOF_DIR into R7
	MOV		R8, #0x0E					;Store value 0x0E into R8	
	STR		R8, [R7]					;Store the value 0x0E into GPIOF_DIR. Set pins 1,2,3 (GPIOF) as output
	
	LDR		R7, =GPIOF_DEN				;Load GPIOF_DEN into R7
	STR		R8, [R7]					;Store 0X0E into R7. Digital enable RGB LEDS
	
	LDR		R7, =GPIOF_DATA				;Load GPIOF_DATA into R7 
	MOV		R8, #0x04					;Store 0x04 into R8. This value turns blue LED on, see line 7 
	MOV		R6, #0						;Turn LED off
	
TURN_OFF	STR		R6,[R7]				;Turn off blue LED 

;DELAY
	LDR		R9, =9000000				;LOAD 9000000
DELAY	SUBS R9, R9, #1					;Decrement R9 by 1
	BNE		DELAY						;Once R9 != 0, jump to DELAY
		
TURN_ON		STR		R8,[R7]				;Turn blue LED on

;DELAY
	LDR		R9, =9000000					;Load 9000000
DELAY2	SUBS R9, R9, #1					;Decrement R9 by 1
	BNE		DELAY2						;Once R9 != 0, jump to DELAY
		
	B	TURN_OFF
	
	END
