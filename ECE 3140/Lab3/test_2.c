#include "3140_concur.h"
#include "utils.h"

void p1 (void)
{
	int i;
	for (i=0; i < 100; i++) { //very long process
		delay ();
    LEDRed_On();
	}
}

void p2 (void)
{
	int i;
	for (i=0; i < 8; i++) { //short process
		delay ();
    LEDBlue_Toggle();
	}
}
void p3 (void)
{
	int i;
	for (i=0; i < 8; i++) { //short process
		delay ();
    LEDRed_Toggle();
	}
}

void p4 (void)
{
	int i;
	for (i=0; i < 8; i++) { //short process
		delay ();
    LEDGreen_Toggle();
	}
}
int main (void)
{
 LED_Initialize();
 

if (process_create (p1,20) < 0) {
 	return -1;
 }
	 
 if (process_create (p2,20) < 0) {
 	return -1;
 }

 if (process_create (p3,20) < 0) {
 	return -1;
 }
 
 if (process_create (p4,20) < 0) {
 	return -1;
 }

 process_start ();
 
 LEDGreen_On();
 
 while (1) ;

 return 0;
}
