#include "utils.h"
#include "3140_concur.h"
#include "realtime.h"

/*pNRT_1 or pNRT_2 will run initally. Then 2 blue run, then 1 red, then 1 blue. Then non-realtime. They finish running. 
Busy wait. 2 blue*/


/*stack space for processes*/

#define NRT_STACK 80
#define RT_STACK  80


/*process start times*/

realtime_t rt1_start = {2,0};
realtime_t rt2_start = {4,0};
realtime_t rt3_start = {20,0};


/*process deadlines*/
realtime_t deadline1 = {5,0};
realtime_t deadline2 = {2,0};
realtime_t deadline3 = {4,0};

/*------------------*/
/* Helper functions */
/*------------------*/
void shortDelay2(){delay();}
void mediumDelay2() {delay(); delay();}



void pNRT_1(void) {
	int i;
	for (i=0; i<5;i++){
	LEDGreen_On();
	shortDelay2();
	LEDGreen_Toggle();
	shortDelay2();
	}
	
}

void pNRT2_1 (void) {
	int i; 
	for (i=0;i<10; i++){
		LEDRed_On();
		shortDelay2();
		LEDRed_Toggle();
		shortDelay2();
	}
	
}

/*-------------------
 * Real-time process
 *-------------------*/

void pRT1_1(void) {
	int i;
	for (i=0; i<3;i++){
	LEDBlue_On();
	mediumDelay2();
	LEDBlue_Toggle();
	mediumDelay2();
	}
}

void pRT2_1 (void) {
	int i;
	for (i=0;i<1;i++){
		LEDRed_On();
		mediumDelay2();
		LEDRed_Toggle();
		mediumDelay2();
	}
}

void pRT3_1 (void) {
	int i;
	for (i=0;i<2;i++){
		LEDBlue_On();
		mediumDelay2();
		LEDBlue_Toggle();
		mediumDelay2();
	}
}

int main(void) {

	LED_Initialize();
	
	if (process_create(pNRT_1,NRT_STACK) < 0) {return -1;}
	if (process_create(pNRT2_1,NRT_STACK) < 0){return -1;}
	
	if (process_rt_create(pRT1_1,RT_STACK, &rt1_start, &deadline1) < 0) {return -1;}
	if (process_rt_create(pRT2_1, RT_STACK, &rt2_start, &deadline2) < 0) {return -1;}
	if (process_rt_create(pRT3_1, RT_STACK, &rt3_start, &deadline3) < 0) {return -1;}
	
	process_start();
	
	LED_Off();
  while(process_deadline_miss>0) {
		LEDGreen_On();
		LEDBlue_On();
		LEDRed_On();
		shortDelay2();
		LED_Off();
		shortDelay2();
		process_deadline_miss--;
	}
	
	
	while(1);
	return 0;
	
}