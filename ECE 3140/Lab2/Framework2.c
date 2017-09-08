#include <fsl_device_registers.h>

/*
     Main program: entry point
*/
void LEDBoff(){
	PTB->PSOR |= 1<<21;
}
	
void LEDBon(){
	PTB->PCOR |= 1<<21;
}
void LEDGoff(){
	PTE->PSOR |= 1<<26;
}
	
void LEDGon(){
	PTE->PCOR |= 1<<26;
}

void LEDB_toggle(){
		PTB->PDOR ^= 1<<21;
}
void LEDG_flash(){
	  LEDGon();
		for(int x = 0; x<330000; x++){}
		LEDGoff();
}
	

int main (void)
{
	  NVIC_EnableIRQ(PIT0_IRQn); /* enable PIT0 Interrupts (for part 3) */

	SIM->SCGC6 = SIM_SCGC6_PIT_MASK;  //Enable the clock to PIT module
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //Enable the clock to port B
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //Enable the clock to port E
	PORTE->PCR[26] = PORT_PCR_MUX(001); //Set up PTB18 as GPIO
	PORTB->PCR[21] = PORT_PCR_MUX(001); //Set up PTB18 as GPIO
	PTB->PDDR |= 1<<21; //enable PTB21 as an output
	PTE->PDDR|= 1<<26; //enable PTB26 as an output
	
	LEDGoff();
	LEDBoff();
	PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK; // Set load value of zeroth PIT
	PIT->MCR &= 0<<1; // enable clock for timer
	PIT->CHANNEL[0].TCTRL |= 0x3; // enable interrupts // enable timer;
	
	while(1){
	LEDB_toggle();
	for(int x = 0; x<3000000; x++);
	}
	
}

/* 
     PIT Interrupt Handler
*/

void PIT0_IRQHandler(void){
	LEDG_flash();
	PIT->CHANNEL[0].TCTRL |= 0x0; //disable timer
	PIT->CHANNEL[0].TFLG = 1; //reset flag
	PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK;
	PIT->CHANNEL[0].TCTRL |= 0x1; // enable timer;
	
}
