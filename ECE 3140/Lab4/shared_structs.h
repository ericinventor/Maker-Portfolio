#ifndef __SHARED_STRUCTS_H__
#define __SHARED_STRUCTS_H__

/** Implement your structs here */

/**
 * This structure holds the process structure information
 */
 
struct process_state {
	unsigned int *sp;
	unsigned int *orig_sp;
	int n;
	struct process_state *next;
	int status; // blocked(1) unblocked (0)
	
};

/**
 * This defines the lock structure
 */
typedef struct lock_state {
	int acquired;
	struct process_state *queue_blocked;
} lock_t;


static void push_tail_process(struct process_state *proc) {
	if (!process_queue) {
		process_queue = proc;
	}
	if (process_tail) {
		process_tail->next = proc;
	}
	process_tail = proc;
	proc->next = NULL;
}
/**
 * This defines the conditional variable structure
 */
//typedef struct cond_var {

//} cond_t;

#endif
