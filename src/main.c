#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/printk.h>

#include "p_thread.h"

#define STACKSIZE 1000
#define SLEEPTIME 1000
#define DELAY_START K_MSEC(2000)
#define HIGH_PRIORITY 1
#define LOW_PRIORITY 3

struct k_thread high_priority_thread;
struct k_thread low_priority_thread;
struct k_thread supervisor_thread;
K_THREAD_STACK_DEFINE(high_priority_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(low_priority_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(supervisor_stack, STACKSIZE);

struct k_sem semaphore;
// int counter;
// void thread_entry(void)
// {
// 	struct k_timer timer;
// 	k_timer_init(&timer, NULL, NULL);
// }

int main(void)
{
    k_sem_init(&semaphore, 1,1); //Initializing the semaphore
    // supervisor_create_thread(&coop_thread, coop_stack, STACKSIZE, )

    // int high_priority_int = 1;
    // int low_priority_int = 3;

    // supervisor_create_wrapper(5000, &supervisor_thread, supervisor_stack, STACKSIZE, super_entry, &high_priority_thread, &low_priority_thread);
    // // First thread, higher priority, wait 2 seconds to start
    // thread_create_wrapper(&high_priority_thread, high_priority_stack, STACKSIZE, high_priority_int, DELAY_START);
    // // Second thread, lower priority, start immediately, take semaphore
    // thread_create_wrapper(&low_priority_thread, high_priority_stack, STACKSIZE, low_priority_int, K_NO_WAIT);

	return 0;
}

