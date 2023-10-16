#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/printk.h>

#include "p_thread.h"

#define STACKSIZE 2000
#define SLEEPTIME 1000
#define DELAY_START K_MSEC(2000)
#define HIGH_PRIORITY 1
#define LOW_PRIORITY 3

struct k_thread coop_thread;
K_THREAD_STACK_DEFINE(coop_stack, STACKSIZE);

struct k_sem semaphore;
// int counter;
// void thread_entry(void)
// {
// 	struct k_timer timer;
// 	k_timer_init(&timer, NULL, NULL);
// }

int main(void)
{
    counter = 0;
    k_sem_init(&semaphore, 1,1); //Initializing the semaphore


    int low_priority_int = 3;
    int high_priority_int = 1;
    // First thread, higher priority, wait 2 seconds to start
    thread_create_helper(&coop_thread, coop_stack, STACKSIZE, high_priority_int, DELAY_START);
    

    // Second thread, lower priority, start immediately, take semaphore
    thread_create_helper(&coop_thread, coop_stack, STACKSIZE, low_priority_int, K_NO_WAIT);


    // Creating the thread, points back to thread_entry

	// while (1) {
    //     thread_counter(&counter, &semaphore, "main",&timer, K_MSEC(SLEEPTIME));
    //     // k_sem_take(&semaphore, K_FOREVER);
    //     // main_thread(&counter);
    //     // k_sem_give(&semaphore);
	// 	// k_timer_start(&timer, K_MSEC(SLEEPTIME), K_NO_WAIT);
	// 	// k_timer_status_sync(&timer);
	// }

	return 0;
}

