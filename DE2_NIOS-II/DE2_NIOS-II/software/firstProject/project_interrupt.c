#include <stdio.h>
#include "sys/alt_alarm.h"
#include "alt_types.h"

//Defined a macro to calculate the time in seconds
#define ALARMTICKS(x) ((alt_ticks_per_second()*(x))/10)
//MAX value to wait before normal OS operation
#define MAX 100000000

alt_u32 myinterrupt_handler(void *);
void prototype_os();

//Main function to call our
//prototype OS
void main(){
	prototype_os();
	return;	
}
//This function is what happens when
//the timer runs down and an interrupt
//is called
alt_u32 myinterrupt_handler (void * context)
{
	printf("Interrupted by the timer!\n");
			//call this again in 5 seconds
	return ALARMTICKS(10); //Using the defined function above
}

//This function will set up a timer and then
//have a wait loop.  Every time the timer pops
//it will go to the interrupt handler
void prototype_os()
{
	// Set up the timer with its interrupt handler
	static alt_alarm alarm;
	//Set the timer to be 1 second
	alt_alarm_start(&alarm, 1000, myinterrupt_handler, NULL);

	while (1)
	{
		printf ("This is the prototype os for my exciting CSE351 course projects!\n");
		int j=0;
		//MAX is the amount of wait time before the next printf above
		for (j = 0 ; j < MAX; j++);
	}
}