#include <stdio.h>

#include "alarm_handler.h"
#include "thread_handler.h"
#include "my_sem.h"

#define NUM_THREADS 8

/* a delay time used to adjust the frequency of printf messages */
#define MAX 100000
#define BUFFER_SIZE 6
sem* full;
sem* empty;
sem* mutex;
char buffer[BUFFER_SIZE];
int Counter = 0, pro=0, con=0;



void insert(char c){
	buffer[Counter] = c;
	Counter++;
	pro++;
}

char removeLast(){
	Counter--;
	char c = buffer[Counter];
	con++;
	return c;
}

void Producer(unsigned int tid, unsigned int numExecutions){
	while(numExecutions > 0){
		int j = 0;
		for(j=0; j<MAX; j++);
	
		mysem_wait(empty);
		mysem_wait(mutex);
		
		insert('X');
		printf("Thread #%d inserted \'X\', Ops left = %d\n", tid, numExecutions);
		
		mysem_signal(mutex);
		mysem_signal(full);
		
		numExecutions--;
	}
}

void Consumer(unsigned int tid, unsigned int numExecutions){
	while(numExecutions > 0){
		int j = 0;
		for(j=0; j<MAX; j++);
		
		mysem_wait(full);
		mysem_wait(mutex);
		
		char c = removeLast();
		printf("\t\t\tThread #%d removed \'%c\', Ops left = %d\n", tid, c, numExecutions);
		
		mysem_signal(mutex);
		mysem_signal(empty);
		
		numExecutions--;
	}
}
    
void prototype_OS()
{
    unsigned int i;
    tcb *thread_pointer;
    
    for (i = 0; i < NUM_THREADS; i++)
    {
		//Even Thread
		if(i % 2 == 0){
			if(i == 0 || i == 2)
				thread_pointer = mythread_create(i, 4096, Consumer, 10);   // 4B * 4096 entries = 16KB
			else
				thread_pointer = mythread_create(i, 4096, Consumer, 15);   // 4B * 4096 entries = 16KB
		}
		//Odd Thread
		else{
			if(i == 1 || i == 3)
				thread_pointer = mythread_create(i, 4096, Producer, 10);   // 4B * 4096 entries = 16KB
			else
				thread_pointer = mythread_create(i, 4096, Producer, 15);   // 4B * 4096 entries = 16KB
		}        
        mythread_start(thread_pointer);
        mythread_join(thread_pointer);
    }
	
	empty = mysem_create(BUFFER_SIZE, 'E');
    full = mysem_create(0, 'F');
	mutex = mysem_create(1, 'M');
	
    if ( start_alarm_succeed() )
        printf ("Start the alarm successfully\n");
    else
        printf ("Unable to start the alarm\n");

    /* an endless while loop */
    while (1)
    {
        printf ("This is the prototype OS for my exciting CSE351 course projects!\nProduced = %d, Consumed = %d\n", pro, con);
        
        /* delay printf for a while */
        for (i = 0; i < MAX; i++);
    }
}

int main()
{
    prototype_OS();
    return 0;
}
