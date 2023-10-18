#include "p_thread.h"


void thread_entry_helper(int *counter, struct k_sem *semaphore, char* thread_name, 
                        struct k_timer *timer, k_timeout_t timeout, k_timeout_t sleeptime){


        
        k_timer_init(timer, NULL, NULL);

        while (1) {
            thread_counter(counter, semaphore, thread_name, timer, timeout, sleeptime);
        }
    }

void thread_counter(int* counter,
                    struct k_sem *semaphore,
                    char* thread_name,
                    struct k_timer *timer,
                    k_timeout_t timeout,
                    k_timeout_t sleep_time){
if (k_sem_take(semaphore, timeout) == 1){
        *counter = *counter + 1;
        printk("hello world from %s! Count %d\n", thread_name, *counter);
        k_sem_give(semaphore);
        return;
    }
    else{
        k_sem_give(semaphore);
        k_timer_start(timer, K_MSEC(100), K_NO_WAIT);
        k_timer_status_sync(timer);
        return;
    }
    
}




void thread_create_helper(struct k_thread * new_thread, k_thread_stack_t * stack, 
                            size_t stack_size, int priority, k_timeout_t wait){

    k_thread_create(new_thread,
                    stack,
                    stack_size,
                    (k_thread_entry_t) thread_entry_helper,
                    NULL,
                    NULL,
                    NULL,
                    K_PRIO_COOP(priority),
                    0,
                    wait);

}