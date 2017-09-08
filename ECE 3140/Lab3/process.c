#include "3140_concur.h"
//#include <stdlib.h>
#include <fsl_device_registers.h>
#include "shared_structs.h"


process_t *current_process = NULL;
process_t *nil = NULL; //next in line

struct process_state {
	unsigned int *sp;
	unsigned int *sp_;
	struct process_state *next;
	int process_size;
};

int process_create (void (*f)(void), int n) {
	SIM -> SCGC6 |= SIM_SCGC6_PIT_MASK;
	unsigned int *tmp_sp = process_stack_init (f, n);
	if (tmp_sp == NULL) return -1;
	
	process_t *tmp_process = malloc(sizeof(process_t));
	if (tmp_process == NULL) return -1;
	
	tmp_process -> sp = tmp_sp;
	tmp_process -> sp_ = tmp_sp;
	tmp_process -> process_size = n;
	if (nil == NULL) {
		nil = tmp_process;
		tmp_process -> next = NULL;
	}
	else {
		process_t *start = nil;
		while (start -> next != NULL) {
			start = start -> next;
		}
		start -> next = tmp_process;
		tmp_process -> next = NULL;
	}
	
	return 0;
}

void process_start (void){
	NVIC_EnableIRQ(PIT0_IRQn);
	SIM -> SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT -> MCR &= 0xfffffffc;
	PIT_LDVAL0 = 10000000; //running at 80 Hz
	
	process_begin();
}

unsigned int * process_select(unsigned int * cursp) {	
	if(cursp == NULL) {
		if(nil == NULL) return NULL;
		else {
			if(current_process != NULL) {
				process_stack_free(current_process -> sp_, current_process -> process_size);
				free(current_process);
			}
			current_process = nil;
			nil = nil -> next;
			current_process -> next = NULL;
			
			return current_process->sp;
		}
	}
	
	else {
		current_process -> sp = cursp;
		if(nil == NULL) return current_process -> sp;
		else {
			process_t *tmp_s = nil;
			while(tmp_s -> next != NULL) {
				tmp_s = tmp_s -> next;
			}
			tmp_s -> next = current_process;
			current_process -> next = NULL;
			current_process = nil;
			nil = nil -> next;
			current_process -> next = NULL;
			
			return current_process -> sp;
		}
	}
}
	
