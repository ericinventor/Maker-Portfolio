#include "3140_concur.h"
#include <stdlib.h>
#include <fsl_device_registers.h>
#include "realtime.h"
#include "utils.h"
int milsec = DEFAULT_SYSTEM_CLOCK /1000;
process_t *current_process = NULL; 
realtime_t current_time;


int process_deadline_met = 0;
int process_deadline_miss = 0;

struct process_state {
	unsigned int* osp; 
	unsigned int* sp; 
	struct process_state *next; 
	int size; 
	realtime_t *starttime;
	realtime_t *deadlinetime;
	int isreal;
};



struct process_state* start_process = NULL;
struct process_state* start_rt_process = NULL;
struct process_state* ready_rt_process = NULL;

//##############################################################################################################
//##########################################THESE ARE OUR ADD FUNCTIONS#########################################
//##############################################################################################################

void add(struct process_state* elem) { 		//adding a element to the end of the process_queue list
	struct process_state *tmp; 
	if (start_process == NULL) {
		start_process = elem;
	 	(*elem).next = NULL;
	} 
	else {
		tmp = start_process; 
		while (tmp->next != NULL) { 		// while there are more elements in the list
			tmp = tmp->next;
		}									// now tmp is the last element in the list
		tmp->next = elem;
		elem->next = NULL;
	}
}


void addrt(struct process_state* elem) {
	struct process_state *tmp;				// create a temporary struct
	if (start_rt_process == NULL) {			// if queue is empty, then make this head
		start_rt_process = elem;
	 	(*elem).next = NULL;				// there doesnt exist a next process
	} 
	else {									// otherwise, move to last element and append
		tmp = start_rt_process; 
		while (tmp->next != NULL) { 		// while there are more elements in the list
			tmp = tmp->next;
		}									// now tmp is the last element in the list
		tmp->next = elem;
		elem->next = NULL;
	}
}

void addready(struct process_state* elem) {	// basically same suh as before
	struct process_state *tmp; 
	if (ready_rt_process == NULL) { 		// if there is nothing in it right now
		ready_rt_process = elem;			// populate with that is ready!
	 	(*elem).next = NULL;
	} 
	else {									// otherwise go to the end of the list, and then append...
		tmp = ready_rt_process; 
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = elem;
		elem->next = NULL;
	}
}

//##############################################################################################################
//#############################################THESE ARE OUR REMOVE FUNCTIONS###################################
//##############################################################################################################

struct process_state* remove(void){ // removes first element in the queue and returns the removed process_state  
	if (start_process == NULL) {
			struct process_state * elem = NULL;
			return NULL;
	}
	else {
		struct process_state * elem = start_process;
		start_process = start_process->next;
		elem ->next = NULL;
		return elem;
	}
}

void removert(process_t *input){  
	process_t *tmp = start_rt_process;
	process_t *headthing = NULL;

	if (tmp != NULL){						// if start_rt_process has anything in it
		if(input == tmp){
			start_rt_process = start_rt_process->next;
		}
		else{
			while(input != tmp){				// check the ID of each thing in the stack
				headthing = tmp;
				tmp = tmp -> next;				// propagate forward
			}
			headthing->next = tmp->next;		// set the head to the tail of the other, stitch!
			tmp->next = NULL;					// truncate the end of the popped value
		}
	}
}

void removeready(process_t *input){ 
	process_t *tmp = ready_rt_process;
	process_t *headthing = NULL;

	if (tmp != NULL){						// if start_rt_process has anything in it
		if(input == tmp){
			ready_rt_process = ready_rt_process->next;
		}
		else{
			while(input != tmp){				// check the ID of each thing in the stack
				headthing = tmp;
				tmp = tmp -> next;				// propagate forward
			}
			headthing->next = tmp->next;		// set the head to the tail of the other, stitch!
			tmp->next = NULL;					// truncate the end of the popped value
		}
	}
}

//##############################################################################################################
//###################################### PROCESS CREATE FUNCTIONS ##############################################
//##############################################################################################################

int process_rt_create(void (*f)(void), int n, realtime_t *start, realtime_t *deadline){
	process_t *new_process = (process_t*) malloc(sizeof(process_t));	// creating an instance of process state
	if(new_process == NULL) {return -1;}  								// check to make sure there is space for it
	
	new_process -> isreal = 1;
	new_process -> starttime = start;									// include the start time in the struct
	new_process -> deadlinetime = deadline;								// include the deadline in the struct
	
	unsigned int * process_startsp = process_stack_init( *f, n);	// Allocate and initialize a stack for a process
	
	new_process->sp = process_startsp;  						    // assigning stack pointer 
	(*new_process).osp = (*new_process).sp;							// set osp to sp
	new_process -> size = n; 										// saved size of the initial process 
	addrt(new_process);												// add this process to the RT queue
	
	if (process_startsp == NULL) {  //return if valid
		return -1;
	}
	else {
		return 0;
	}
}

		
int process_create(void (*f) (void), int n) {
	process_t *new_process = (process_t*) malloc(sizeof(process_t));	// creating an instance of process state
	if(new_process == NULL) {return -1;} 
	
	new_process->isreal = 0;
	new_process ->starttime= NULL;
	new_process ->deadlinetime = NULL;
	
	unsigned int * process_startsp = process_stack_init( *f, n);	// Allocate and initialize a stack for a process
	
	new_process->sp = process_startsp;  							// Assigning stack pointer
	(*new_process).osp = (*new_process).sp;  
	new_process -> size = n; 										//saved size of the initial process 
	add(new_process);
	
	if (process_startsp == NULL) {  //return if valid
		return -1;
	}
	else{return 0;}
}; 

// ########################################################################################################3
// ############################################# PROCESS  START ###########################################3
// ########################################################################################################3
void process_start (void){
	current_time.sec = 0;
	current_time.msec= 0;
	
	NVIC_EnableIRQ(PIT0_IRQn);
	NVIC_EnableIRQ(PIT1_IRQn);
	SIM -> SCGC6 = SIM_SCGC6_PIT_MASK; // enable clock to PIT module
	PIT_MCR = 0x00; 
	PIT->CHANNEL[1].TCTRL = PIT_TCTRL_TEN_MASK;
	PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK; // enable interrupt
	PIT->CHANNEL[0].LDVAL = DEFAULT_SYSTEM_CLOCK/10; //set load value of PIT to whatever
	PIT->CHANNEL[1].LDVAL = milsec; 	// this will be our timer for scheduling, 120M divided by 1000

	NVIC_SetPriority(SVCall_IRQn, 1);
	NVIC_SetPriority(PIT0_IRQn, 2);
	NVIC_SetPriority(PIT1_IRQn, 0); 
	
	process_begin(); //finishes by calling this process begin function
}; 
	

//###########################################################################################################
//################################### PIT1 TIMER ITERRUPT SERVICE ROUTINE ###################################
//##################### this will tally up the time and populate the ready queue ############################

void PIT1_IRQHandler(void){ 
	if(current_time.msec == 1000){				// increment the second counter 
		current_time.msec = 0;
		current_time.sec++;
		//LEDGreen_Toggle();
	}
	else{current_time.msec++;}					// increment the millisecond counter
	
	PIT->CHANNEL[1].LDVAL = milsec ; 				// Set load value back to 1ms
	if(PIT->CHANNEL[1].TFLG){
		PIT->CHANNEL[1].TFLG = 1;   				// clear flag			
		PIT->CHANNEL[1].TCTRL = 0;  				// disable clock
		PIT->CHANNEL[1].TCTRL = 1;  				// reenable clock
		PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_TIE_MASK;// literally no idea what this is, but it works
	}
}


//#######################################################################################################
//########################################### MINIMUM DEADLINE FINDING FUNCTION #########################
//########################## cycles through ready queue and picks out the one with closest deadline #####

struct process_state* findminimum(void){ // returns the element in the ready_queue with the closest deadline
	if(ready_rt_process == NULL){
		return NULL;
	}
	else{
		process_t *rtmp = ready_rt_process;
		int min = (*rtmp->deadlinetime).sec*1000 + (*rtmp->deadlinetime).msec + (*rtmp->starttime).sec*1000 + (*rtmp->starttime).msec;
		process_t *minstruct = rtmp;
		while(rtmp!=NULL){ 				//shuffle through the rtmp queue to find minimum deadline time.
			int dtcurr = (*rtmp->deadlinetime).sec*1000 + (*rtmp->deadlinetime).msec + (*rtmp->starttime).sec*1000 + (*rtmp->starttime).msec;
			if(dtcurr < min){
				min = dtcurr;
				minstruct = rtmp;
			}
			rtmp = rtmp->next;
		}
		return minstruct;
	}
}

//##############################################################################################################
//################################### PROCESS SELECT FUNCTION###################################################
//################## returns sp of the next process that we want to execute ####################################


unsigned int *process_select (unsigned int* cursp){
	//UPDATE THE READY QUEUE
	struct process_state *tmp = start_rt_process;// initialize the ready queue temp struct, and times
	int timenow = (current_time.sec*1000) + current_time.msec;
	int timeproc = ((*tmp->starttime).sec *1000) + (*tmp->starttime).msec;
	
	while(tmp!=NULL){
		timenow = (current_time.sec*1000) + current_time.msec;
		timeproc = ((*tmp->starttime).sec *1000) + (*tmp->starttime).msec;
		if(timeproc <= timenow){				// if the process start time is less than current time, add to queue
			addready(tmp);						// add to ready queue
			removert(tmp);						// remove from normal RT queue
		}										// otherwise dont do anything, skip over that dood
		tmp = tmp->next;						// advance the place of temp queue
	}

	// IF THERE IS A REAL TIME PROCESS THAT IS READY
	if(ready_rt_process !=NULL){
		
		process_t * minstruct = findminimum();

		if(cursp == NULL && current_process == NULL){	//	no process running, initiate first process
			current_process = minstruct; 				// set the first process to be the shortest deadline we found in the readyqueue
			if(current_process == NULL) {return NULL;}
			return current_process -> sp; 
		}

		if(cursp == NULL && current_process != NULL){ 	//this process is done, but there are still processes in queue
			struct process_state* tmp = current_process;
			int deadtime = (*tmp->deadlinetime).sec*1000 + (*tmp->deadlinetime).msec +  (*tmp->starttime).sec*1000 + (*tmp->starttime).msec;
			int curtime = current_time.sec*1000 + current_time.msec;
			if(deadtime <= curtime){
				process_deadline_miss++;				// if the deadline is greater than the total time, we missed
			}
			else{
				process_deadline_met++;					// if the deadline is less than the total time, we met! woo
			}

			removeready(current_process); 							// remove the old current process from the ready queue
			minstruct = findminimum();					// find the new minimum deadline process

			if(minstruct != NULL){ 						// if there is nothing left in the ready queue
				current_process = minstruct;			// reset the current process to the one with the closest deadline
				process_stack_free(tmp->osp, tmp-> size);	// free this old stack plz
				free(tmp);
				return current_process -> sp; 				// return the new process' stack pointer
			}
			else{
				current_process = start_process; 
				process_stack_free(tmp->osp, tmp-> size);	// free this old stack plz
				free(tmp);
				if(current_process == NULL) {return NULL;}
				return current_process -> sp;							// we need to execute normal processes...
			}			
		}

		else{ // this will switch you from your normal process to the higher priority one
			(*current_process).sp = cursp;
			minstruct = findminimum();
			current_process = minstruct;
			return current_process->sp;			
		}
	}

	if(ready_rt_process == NULL && start_process !=NULL){	// if the RT ready queue is empty, but the normal queue is filled
		if(cursp == NULL && current_process == NULL){		// no process running, initiate first process
			current_process = start_process; 
			return current_process -> sp; 
		}

		if(cursp == NULL && current_process != NULL){ 		// this process is done, but there are still processes in queue
			struct process_state* tmp = remove();
			current_process = start_process; 
			process_stack_free(tmp->osp, tmp-> size);	
			free(tmp); 
			return current_process -> sp; 
		}

		else{ 												// the current process is not done, and the process queue is not empty} 
			(*current_process).sp = cursp; 
			struct process_state* tmp = remove(); 
			add(tmp); 
			current_process = start_process; 
			return (current_process-> sp); 
		}
	}
															// if ready and process queue are empty, but your RT queue is not...
	if(ready_rt_process == NULL && start_process == NULL && start_rt_process != NULL){ // sort of a busy wait kinda thing...
		__enable_irq();
		while(ready_rt_process == NULL);
		__disable_irq();
		current_process = ready_rt_process;
		return current_process -> sp;						// return the head of the newly populated ready queue
	}

	else{						// there's nothing left for us Charlie, this world has been ravaged beyond all processes
		current_process = NULL;
		return NULL;
	}
}