/*
* my_sem.c
*
*  Created on: Dec 9, 2013
*      Author: zsims
*/
#include "my_sem.h"
#include "thread_handler.h"
#include "queue.h"
#include <stdio.h>
#include <malloc.h>


sem* mysem_create(int size, char ID){
	sem* newSem = malloc(sizeof(sem) * 1);
	newSem->count = size;
	newSem->waiters = 0;
	newSem->ID = ID;
	return newSem;
}

void mysem_signal(sem* pSem){
	DISABLE_INTERRUPTS();
	pSem->count++;

	int i=0;
	if(pSem->waiters > 0){
		tcb* thread = pSem->waitQueue[0];
		thread->state = READY;
		printf("\t\t\t\t\t\tThread #%u Released\n", thread->tid);
		enqueue(thread);
		pSem->waiters--;
	}

	for(i=1; i <= pSem->waiters; i++){
		pSem->waitQueue[i-1] = pSem->waitQueue[i];
	}

	ENABLE_INTERRUPTS();
}

void mysem_wait(sem* pSem){
	DISABLE_INTERRUPTS();


	if(pSem->count == 0){
		current_running_thread->state = BLOCKED;
		pSem->waitQueue[pSem->waiters] = current_running_thread;
		pSem->waiters++;
		printf("\t\t\t\t\t\tThread #%u blocked\n", current_running_thread->tid);
		ENABLE_INTERRUPTS();
		while(current_running_thread->state == BLOCKED);
	}
	else{
		pSem->count--;
		ENABLE_INTERRUPTS();
	}
}

void mysem_delete(sem* pSem){
	free(pSem->waitQueue);
	free(pSem);
}

int mysem_count(sem* pSem){
	return pSem->count;
}
