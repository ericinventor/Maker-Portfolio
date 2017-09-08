#include <fsl_device_registers.h>

/*
     Main program: entry point
*/

//Blue is PTB21
void LEDBoff(){
	PTB->PSOR |= 1<<21;
}
	
void LEDBon(){
	PTB->PCOR |= 1<<21;
}

void LEDB_toggle(){
		PTB->PTOR |= 1<<21;
}

//Green is PTE26
void LEDGoff(){
	PTE->PSOR |= 1<<26;
}
	
void LEDGon(){
	PTE->PCOR |= 1<<26;
}

int greenOn; //Green LED state flag

int main (void){
	NVIC_EnableIRQ(PIT0_IRQn); /* enable PIT0 Interrupts (for part 3) */

	SIM->SCGC6 = SIM_SCGC6_PIT_MASK;  //Enable the clock to PIT module
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //Enable the clock to port B
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //Enable the clock to port E
	PORTE->PCR[26] = PORT_PCR_MUX(001); //Set up PTE26 as GPIO
	PORTB->PCR[21] = PORT_PCR_MUX(001); //Set up PTB21 as GPIO
	PTB->PDDR |= 1<<21; //enable PTB21 as an output
	PTE->PDDR|= 1<<26; //enable PTB26 as an output
	
	
	PIT->MCR &= 0<<1; // enable clock for timer
	PIT->CHANNEL[0].TCTRL |= 0x3; // enable interrupts // enable timer;
	
		
	LEDGoff();
	LEDBoff();
	
	greenOn= 0; //0 = "not on
	PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK; // Set load value of zeroth PIT
	
	while(1){
	LEDGoff();
	LEDB_toggle();		
	for(int x = 0; x<3500000; x++); //Tested by using BPM calculator.  This net us a BPM of 60
	}
	
}

/* 
     PIT Interrupt Handler
*/

void PIT0_IRQHandler(void){
	
	if(greenOn==0){ //if green light is not on
		//turn on for 0.1 sec
		
		PIT->CHANNEL[0].LDVAL = 0x112A880;
		
		if(PIT_TFLG0){				//PIT->CHANNEL[0].TFLG

			PIT->CHANNEL[0].TCTRL |= 0x0; //disable timer
			PIT->CHANNEL[0].TFLG = 1; //reset flag
			PIT->CHANNEL[0].TCTRL |= 0x1; // enable timer;
			PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; 
			
			LEDGon();
			greenOn = 1;
			
		}
	}
	else{
		//turn off for 0.9 sec
		
		PIT->CHANNEL[0].LDVAL = 0x1E8480;	
		
		if(PIT_TFLG0){			
			
			PIT->CHANNEL[0].TCTRL |= 0x0; //disable timer
			PIT->CHANNEL[0].TFLG = 1; //reset flag
			PIT->CHANNEL[0].TCTRL |= 0x1; // enable timer;
			PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; 
			
			LEDGoff();
			greenOn = 0;
			
		}
	}
	

	
}