#include <fsl_device_registers.h>

/*
     Main program: entry point
*/

//Red LED is PTB22

void LEDRoff(){
	PTB->PSOR |= 1<<22;
}
	
void LEDtoggle(){
	PTB->PTOR |= 1<<22;
	}


/**
	
	Each of these registers has 32 addresses.  You set a particular address(es)
	
	PDOR: 0 = Drives logic level of 0 on pin (LED on), 1 = off
	PSOR: 1 = corresponding bit in PDOR set to 1, 0 does not change things
	PCOR: 1 = corresponding bit in PDOR set to 0, 0 does not change things
	PTOR: 1 = corresponding bit set to inverse of logic state
	
	
	*/
	
	
int main (void)
{
	    /* your code goes here */
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK;  //Enable the clock to PIT module
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //Enable the clock to port B
	PORTB->PCR[22] = PORT_PCR_MUX(001); //Set up PTB22 as GPIO
	PTB->PDDR |= 1<<22; //enable PTB22 as an output

	PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK; // Set load value of zeroth PIT
	PIT->MCR &= 0<<1; // enable clock for timer (set by bit 1 of this register)
	LEDRoff();//LED comes initially on b/c PDOR comes as 0 = On
	
	PIT->CHANNEL[0].TCTRL = 1; // enable timer
	
	
	while(1){		
		if(PIT_TFLG0){			
			LEDtoggle();
			PIT->CHANNEL[0].TCTRL = 0; //disable timer
			PIT->CHANNEL[0].TFLG = 1; //reset flag; writing 1 will clear it
			PIT->CHANNEL[0].TCTRL = 1; // enable timer; Start over again			
		}				
	}
}