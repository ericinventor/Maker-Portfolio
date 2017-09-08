#include <fsl_device_registers.h>
unsigned int counter = 0;


void LEDRoff(){
	PTB->PSOR |= 1<<22;
}
	
void LEDRon(){
	PTB->PCOR |= 1<<22;
}

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

void LEDtoggle(){
	PTB->PDOR ^= 1<<22;
}

int main (void) {
	SIM->SCGC6 = SIM_SCGC6_PIT_MASK;  //Enable the clock to PIT module
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK; //Enable the clock to port B
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK; //Enable the clock to port E
	PORTE->PCR[26] = PORT_PCR_MUX(001); //Set up PTB18 as GPIO
	PORTB->PCR[22] = PORT_PCR_MUX(001); //Set up PTB18 as GPIO
	PORTB->PCR[21] = PORT_PCR_MUX(001); //Set up PTB18 as GPIO
	PTB->PDDR |= 1<<22; //enable PTB22 as an output
	PTB->PDDR |= 0<<21; //enable PTB22 as an output
	PTE->PDDR |= 0<<26;
	
	LEDRoff();
	while(1){
		LEDRon();
    for(int x = 0; x<330000; x++){}
		LEDRoff();
		for(int x = 0; x<3300000; x++){}
		}		
	
}

