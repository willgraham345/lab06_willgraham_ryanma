#include "p_thread.h"


void thread_entry(){
    for (int i = 0; ; i++);
}

void thread_create_wrapper(struct k_thread * new_thread, k_thread_stack_t * stack, size_t stack_size, int priority, k_timeout_t wait){

    k_thread_create(new_thread,
                    stack,
                    stack_size,
                    (k_thread_entry_t) thread_entry,
                    NULL,
                    NULL,
                    NULL,
                    K_PRIO_COOP(priority),
                    0,
                    wait);

}

void supervisor_create_wrapper(struct k_thread * new_thread, k_thread_stack_t * stack, size_t stack_size){
        k_thread_create(new_thread,
                    stack,
                    stack_size,
                    (k_thread_entry_t) supervisor_entry,
                    NULL,
                    NULL,
                    NULL,
                    -CONFIG_NUM_COOP_PRIORITIES,
                    0,
                    K_NO_WAIT);
}



void supervisor_entry(){
    for (int i = 0; ; i++);
}