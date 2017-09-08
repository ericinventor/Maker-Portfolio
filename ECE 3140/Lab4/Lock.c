#include "lock.h"


void l_init(lock_t* l){
/**
 * Initialises the lock structure
 *
 * @param l pointer to lock to be initialised
 */
	l->acquired = 0;
	l->queue_blocked = NULL;
}

void l_lock(lock_t* l){
/**
 * Grab the lock or block the process until lock is available
 *
 * @param l pointer to lock to be grabbed
 */
	PIT->CHANNEL[0].TCTRL = 0x1; // disable interrupts
	
	if (l->acquired == 1){ // lock is not available
		current_process->status = 1; //blocked
		if(l->queue_blocked == NULL) //add process to blocked queue if empty
			l->queue_blocked = current_process;
		else{ 							// add process to blocked queue if not empty
			struct process_state *findend = l->queue_blocked;
			while(findend->next != NULL){ // iterate through blocked queue until end is found
				findend = findend->next;
			}
			findend->next = current_process; // add process to end of queue
		}
		process_blocked();
		
	}
	   			//lock is available
		l->acquired = 1;
	
	PIT->CHANNEL[0].TCTRL = 0x3; // enable interrupts
}
void l_unlock(lock_t* l){
/**
 * Release the lock along with the first process that may be waiting on
 * the lock. This ensures fairness wrt lock acquisition.
 *
 * @param l pointer to lock to be unlocked
 */
	PIT->CHANNEL[0].TCTRL = 0x1; // disable interrupts
	

	if(l->queue_blocked != NULL){
		struct process_state *released = l->queue_blocked; //copy blocked queue
		l->queue_blocked = l->queue_blocked->next; //advance block queue to elimnate released process
		released->next = NULL;			//snip first process from copied blocked queue
		released->status=0;
		push_tail_process(released);
	} else{
		l->acquired = 0; // release lock
	}
PIT->CHANNEL[0].TCTRL = 0x3; // enable interrupts
}
