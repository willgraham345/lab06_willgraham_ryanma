#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <unity.h>
#include "p_thread.h"




void thread_create_wrapper(struct k_thread * new_thread,
    k_thread_stack_t * stack,
    size_t stack_size, 
    int priority,
    k_timeout_t wait)
{
    // TODO: Refactor this into run_analyzer_split. 
    k_thread_create(new_thread,
                    stack,
                    stack_size,
                    (k_thread_entry_t) thread_entry,
                    NULL,
                    NULL,
                    NULL,
                    priority,
                    0,
                    wait);

}

void supervisor_create_wrapper(struct k_thread * supervisor_thread, 
    k_thread_stack_t * supervisor_stack, 
    size_t stack_size, 
    k_thread_entry_t supervisor_entry, 
    struct k_thread * hi_prio_thread,
    struct k_thread * low_prio_thread)
{
        k_thread_create(supervisor_thread,
                    stacksupervisor_stack,
                    stack_size,
                    (k_thread_entry_t) supervisor_entry,
                    hi_prio_thread,
                    lo_prio_thread,
                    NULL,
                    -CONFIG_NUM_COOP_PRIORITIES,
                    0,
                    K_NO_WAIT);
}



void run_analyzer(k_thread_entry_t thread_entry,
                  void *arg,
                  int pri_prio,  k_timeout_t pri_delay,
                  uint64_t *pri_duration,
                  int sec_prio,  k_timeout_t sec_delay,
                  uint64_t *sec_duration,
                  uint64_t *total_duration)
{
    run_analyzer_split(5000,
                       thread_entry,
                       primary_name, arg, NULL,
                       pri_prio, pri_delay, pri_duration,
                       thread_entry,
                       secondary_name, arg, NULL,
                       sec_prio, sec_delay, sec_duration,
                       total_duration);

}



void run_analyzer_split(uint32_t test_duration,
                        k_thread_entry_t pri_thread_entry,
                        void *pri_arg0, void *pri_arg1, void *pri_arg2,
                        int pri_prio,  k_timeout_t pri_delay,
                        uint64_t *pri_duration,

                        k_thread_entry_t sec_thread_entry,
                        void *sec_arg0, void *sec_arg1, void *sec_arg2,
                        int sec_prio,  k_timeout_t sec_delay,
                        uint64_t *sec_duration,

                        uint64_t *total_duration)
{

	uint64_t start, primary, secondary, end, elapsed;
    k_thread_runtime_stats_t pri_stats, sec_stats, start_stats, end_stats;;

    k_thread_runtime_stats_all_get(&start_stats);
    supervisor_create_wrapper(&super_thread, 
                            super_stack,
                            STACKSIZE,
                            (k_thread_entry_t) super_entry,
                            &hi_prio_thread,
                            &low_prio_thread,
                            NULL);

    //TODO: Add two preemtive thread calls
    //TODO: Add joining threads
    //TODO: Add runtime states
    //TODO: Add printing everything (use printk)

    //TODO: Add aborting threads.

    // thread_create_wrapper(&primary_)


}