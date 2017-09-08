#include <fsl_device_registers.h>

/*
     Main program: entry point
*/

void LEDRoff(){
	PTB->PSOR |= 1<<22;
}
	
void LEDRon(){
	PTB->PCOR |= 1<<22;
}	
void LEDtoggle(){
	PTB->PDOR ^= 1<<22;
	}

int main (void)
{
	    /* your code goes here */
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK;  //Enable the clock to PIT module
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //Enable the clock to port B
	PORTB->PCR[22] = PORT_PCR_MUX(001); //Set up PTB18 as GPIO
	PTB->PDDR |= 1<<22; //enable PTB22 as an output

	PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK; // Set load value of zeroth PIT
	PIT->MCR &= 0<<1; // enable clock for timer
	LEDRoff();
	
	while(1){
		PIT->CHANNEL[0].TCTRL = 1; // enable timer
		while(!PIT_TFLG0);
		LEDtoggle();
		PIT->CHANNEL[0].TFLG = 1; //reset flag
		PIT->CHANNEL[0].TCTRL = 0; //disable timer
		;
	}

}
