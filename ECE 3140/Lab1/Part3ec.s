		AREA Myprog, CODE, READONLY
		ENTRY
		EXPORT __main

;constants
;don't change these addresses!
PCR22 	  EQU 0x4004A058 ;PORTB_PCR22  address
SCGC5 	  EQU 0x40048038 ;SIM_SCGC5    address
PDDR 	  EQU 0x400FF054 ;GPIOB_PDDR   address
PCOR 	  EQU 0x400FF048 ;GPIOB_PCOR   address
PSOR      EQU 0x400FF044 ;GPIOB_PSOR   address

ten		  EQU 0x00000400 ; 1 << 10 1024
eight     EQU 0x00000100 ; 1 << 8 256
twentytwo EQU 0x00400000 ; 1 << 22 4194304

__main
	; Your code goes here!
				BL LEDSETUP
				
				MOV R0, #14
				BL fib	
				BL MorseDigit

				B forever 
				
	;------------TEST CASES (pass)---------------------------------
				MOV R0, #6
				BL fib
				BL MorseDigit

				MOV R0, #7
				BL fib
				BL MorseDigit

				MOV R0, #8
				BL fib
				BL MorseDigit

				MOV R0, #9
				BL fib
				BL MorseDigit

				MOV R0, #12
				BL fib
				BL MorseDigit

				MOV R0, #13
				BL fib
				BL MorseDigit

				
MorseDigit
				PUSH {LR}
morseLoop				
				MOV R1, #0  ;initialize counter R1
				MOV R2, #10 ;R2=10 for mod and division purposes
				
				
				;modulo 10
				UDIV R3, R0, R2
				MLS R3, R3, R2, R0 ;R3 stores R0%10
				
				PUSH {R3} ;store modulo 10 into stack (this is a single digit)
				
				ADD R1, #1 ;increment a counter (to count how many digits)
				
				UDIV R0, R0, R2 ;divide R0 by 10 (integer division)
				
				CMP R0, #0
				BNE morseLoop ;loop while R0 > 0
				;so now you have all digits in a stack in the correct order				
lightLoop
				POP{R0} ;let R0 be a digit from the stack
				
				;cases for different digits, as before
				CMP R0, #1
				BNE case2
				
				BL DOT
				BL DASH
				BL DASH
				BL DASH
				BL DASH
				
				BL DELAY3 ;delay between digits
				SUBS R1, R1, #1 ;decrement digit counter
				BNE lightLoop ;loop while R1 > 0 (so until the stack has no more digits)
				CMP R1, #0
				BEQ case9 ;apparently we can't jump to "done" from here because its too big of a jump...lol.  So I chose something close that gets there anyway.	
case2
				CMP R0, #2
				BNE case3
				
				BL DOT
				BL DOT
				BL DASH
				BL DASH
				BL DASH
				
				BL DELAY3
				SUBS R1, R1, #1 
				BNE lightLoop 
				CMP R1, #0
				BEQ case9 
case3
				CMP R0, #3
				BNE case4
				
				BL DOT
				BL DOT
				BL DOT
				BL DASH
				BL DASH
				
				BL DELAY3
				SUBS R1, R1, #1
				BNE lightLoop 
				CMP R1, #0
				BEQ case9
case4
				CMP R0, #4
				BNE case5
				
				BL DOT
				BL DOT
				BL DOT
				BL DOT
				BL DASH
				
				BL DELAY3
				SUBS R1, R1, #1 
				BNE lightLoop 
				CMP R1, #0
				BEQ done
case5
				CMP R0, #5
				BNE case6
				
				BL DOT
				BL DOT
				BL DOT
				BL DOT
				BL DOT
				
				BL DELAY3
				SUBS R1, R1, #1
				BNE lightLoop 
				CMP R1, #0
				BEQ done
case6
				CMP R0, #6
				BNE case7
				
				BL DASH
				BL DOT
				BL DOT
				BL DOT
				BL DOT
				
				BL DELAY3
				SUBS R1, R1, #1 
				BNE lightLoop
				CMP R1, #0
				BEQ done
case7
				CMP R0, #7
				BNE case8
				
				BL DASH
				BL DASH
				BL DOT
				BL DOT
				BL DOT
				
				BL DELAY3
				SUBS R1, R1, #1 
				BNE lightLoop 
				CMP R1, #0
				BEQ done
case8
				CMP R0, #8
				BNE case9
				
				BL DASH
				BL DASH
				BL DASH
				BL DOT
				BL DOT
				
				BL DELAY3
				SUBS R1, R1, #1 
				BNE lightLoop
				CMP R1, #0
				BEQ done
case9
				CMP R0, #9
				BNE default
				
				BL DASH
				BL DASH
				BL DASH
				BL DASH
				BL DOT
				
				BL DELAY3
				SUBS R1, R1, #1 
				BNE lightLoop
				CMP R1, #0
				BEQ done
default				
				CMP R0, #0
				BNE endcheck ;branch to prevent the default case from outputting if R0 != 0
				
				BL DASH
				BL DASH
				BL DASH
				BL DASH
				BL DASH
endcheck											
				BL DELAY3
				SUBS R1, R1, #1 
				BNE lightLoop 
				CMP R1, #0
				BEQ done
done
				POP {LR}
				BX LR
								
fib		
	; Your code goes here!
				CMP R0, #0
				BGT if2
				MOV R0, #0
				BX LR
if2				
				SUB R0, R0, #1 ;n-1
				CMP R0, #0
				BNE recurse
				MOV R0, #1
				BX LR
recurse
				PUSH {LR}
				PUSH {R0} ;store n-1
				BL fib ;compute fib(n-1)
				POP {R1} ;R1 now has old n-1
				PUSH{R0} ;store fib(n-1)
				SUB R0, R1, #1 ;n-2
				BL fib ;compute fib(n-2)
				POP {R1} ;R1 has fib(n-1)
				ADD R0, R0, R1 ;return result in R0
				POP {LR}
				BX LR
				
				
				
	

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
;delay for dots and instances when the LED turns off				
DELAY1
				PUSH {R4}
				LDR R4, =0xFFFFF
loop1
				NOP
				NOP
				NOP
				
				
				SUBS R4, #1
				BNE loop1
				POP{R4}
				BX LR			
				
;delay for dashes
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
				
				SUBS R4, #1
				BNE loop2
				POP{R4}
				BX LR	

DOT
				PUSH {LR}
				BL LEDON
				BL DELAY1
				BL LEDOFF
				BL DELAY1
				POP {LR}
				BX LR

DASH
				PUSH {LR}
				BL LEDON
				BL DELAY3
				BL LEDOFF
				BL DELAY1
				POP {LR}
				BX LR



forever
			B		forever						; wait here forever	
			END
 				
