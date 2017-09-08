		AREA Myprog, CODE, READONLY
		ENTRY
		EXPORT __main
			
;don't change these addresses!
PCR22 	  EQU 0x4004A058 ;PORTB_PCR22  address
SCGC5 	  EQU 0x40048038 ;SIM_SCGC5    address
PDDR 	  EQU 0x400FF054 ;GPIOB_PDDR   address
PCOR 	  EQU 0x400FF048 ;GPIOB_PCOR   address
PSOR      EQU 0x400FF044 ;GPIOB_PSOR   address

ten		  EQU 0x00000400 ; 1 << 10
eight     EQU 0x00000100 ; 1 << 8
twentytwo EQU 0x00400000 ; 1 << 22

__main
	; Your code goes here!
		
				BL    LEDSETUP
				MOV   R0, #1 ;load inpute number here
		
case1			
				CMP R0, #1
				BNE case2
				
				BL DOT
				BL DASH
				BL DASH
				BL DASH
				BL DASH
				
				BL forever
				
case2			
				CMP R0, #2
				BNE case3
				
				BL DOT
				BL DOT
				BL DASH
				BL DASH
				BL DASH
				
				BL forever		

case3			
				CMP R0, #3
				BNE case4
				
				BL DOT
				BL DOT
				BL DOT
				BL DASH
				BL DASH
				
				BL forever	
case4			
				CMP R0, #4
				BNE case5
				
				BL DOT
				BL DOT
				BL DOT
				BL DOT
				BL DASH
				
				BL forever	
				
case5			
				CMP R0, #5
				BNE case6
				
				BL DOT
				BL DOT
				BL DOT
				BL DOT
				BL DOT
				
				BL forever	
				
case6			
				CMP R0, #6
				BNE case7
				
				BL DASH
				BL DOT
				BL DOT
				BL DOT
				BL DOT
				
				BL forever	
				
case7			
				CMP R0, #7
				BNE case8
				
				BL DASH
				BL DASH
				BL DOT
				BL DOT
				BL DOT
				
				BL forever	
				
case8			
				CMP R0, #8
				BNE case9
				
				BL DASH
				BL DASH
				BL DASH
				BL DOT
				BL DOT
				
				BL forever	
				
case9			
				CMP R0, #9
				BNE case0
				
				BL DASH
				BL DASH
				BL DASH
				BL DASH
				BL DOT
				
				BL forever	

case0			
				CMP R0, #0
				
				BL DASH
				BL DASH
				BL DASH
				BL DASH
				BL DASH
				
				BL forever	
fib		
	; Your code goes here!

; Call this function first to set up the LED
LEDSETUP
				PUSH  {R4, R5} ; To preserve R4 and R5
				LDR   R4, =ten ; Load the value 1 << 10
				LDR		R5, =SCGC5
				STR		R4, [R5]
				
				LDR   R4, =eight
				LDR   R5, =PCR22
				STR   R4, [R5]
				
				LDR   R4, =twentytwo
				LDR   R5, =PDDR
				STR   R4, [R5]
				POP   {R4, R5}
				BX    LR

; The functions below are for you to use freely   


LED1
				BL LEDON
				
				
DELAY1
				PUSH {R4}
				LDR R4, =0xFFFFF
loop1			
				NOP
				NOP
				NOP
				
				SUBS R4, #1
				BNE loop1
				POP {R4}
				BX LR
				
				
DELAY3
				PUSH {R4}
				LDR R4, =0xFFFFF
loop2			
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				NOP
				
				SUBS R4, #1
				BNE loop2
				POP {R4}
				BX LR
LEDON				
				PUSH  {R4, R5}
				LDR   R4, =twentytwo
				LDR   R5, =PCOR
				STR   R4, [R5]
				POP   {R4, R5}
				BX    LR
LEDOFF				
				PUSH  {R4, R5}
				LDR   R4, =twentytwo
				LDR   R5, =PSOR
				STR   R4, [R5]
				POP   {R4, R5}
				BX    LR
DOT 			
				PUSH {LR}
				BL LEDON
				BL DELAY1
				BL LEDOFF
				BL DELAY1
				POP{LR}
				BX LR
				
DASH			
				PUSH {LR}
				BL LEDON
				BL DELAY3
				BL LEDOFF
				BL DELAY1
				POP{LR}
				BX LR

				
forever
			B		forever						; wait here forever	
			END
 				
