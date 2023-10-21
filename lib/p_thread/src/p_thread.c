#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include "p_thread.h"
#include <unity.h>

#define STACKSIZE 2000
extern k_thread_runtime_stats_t threads_runtime_stats;
K_THREAD_STACK_DEFINE(hi_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(lo_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(super_stack, STACKSIZE);
struct k_thread hi_thread, lo_thread, super_thread;

// Names for threads
char * hi_name = "hi";
char * lo_name = "lo";

void super_entry(struct k_thread *hi, struct k_thread *lo)
{
    printk("super is suspending threads\n");
    k_thread_suspend(hi);
    k_thread_suspend(lo);
}

void thread_create_wrapper(struct k_thread * new_thread,
    k_thread_stack_t * stack,
    size_t stack_size, 
    k_thread_entry_t thread_entry,
    int priority,
    k_timeout_t wait)
{
    // TODO: Refactor this into run_analyzer_split. 
    k_thread_create(new_thread,
                    stack,
                    stack_size,
                    thread_entry,
                    NULL,
                    NULL,
                    NULL,
                    priority,
                    0,
                    wait);

}

void supervisor_create_wrapper(uint32_t test_duration,
    struct k_thread * supervisor_thread, 
    k_thread_stack_t * supervisor_stack, 
    size_t stack_size, 
    k_thread_entry_t supervisor_entry, 
    struct k_thread * hi_prio_thread,
    struct k_thread * lo_prio_thread)
{
        k_thread_create(supervisor_thread,
                    supervisor_stack,
                    stack_size,
                    (k_thread_entry_t) supervisor_entry,
                    hi_prio_thread,
                    lo_prio_thread,
                    NULL,
                    -CONFIG_NUM_COOP_PRIORITIES,
                    0,
                    K_MSEC(test_duration));
}



void run_analyzer(k_thread_entry_t thread_entry,
                  void *arg,
                  int hi_prio, 
                  k_timeout_t hi_delay,
                  uint64_t *pri_duration,
                  int lo_prio, 
                  k_timeout_t lo_delay,
                  uint64_t *sec_duration,
                  uint64_t *total_duration)
{
    run_analyzer_split(5000,
                       thread_entry,
                       hi_name, arg, NULL,
                       hi_prio, hi_delay, pri_duration,
                       thread_entry,
                       lo_name, arg, NULL,
                       lo_prio, lo_delay, sec_duration,
                       total_duration);

}

void run_analyzer_two_entry(k_thread_entry_t thread_entry_hi,
                  k_thread_entry_t thread_entry_lo,
                  void *arg,
                  int hi_prio, 
                  k_timeout_t hi_delay,
                  uint64_t *pri_duration,
                  int lo_prio, 
                  k_timeout_t lo_delay,
                  uint64_t *sec_duration,
                  uint64_t *total_duration)
{
    run_analyzer_split(5000,
                       thread_entry_hi,
                       hi_name, arg, NULL,
                       hi_prio, hi_delay, pri_duration,
                       thread_entry_lo,
                       lo_name, arg, NULL,
                       lo_prio, lo_delay, sec_duration,
                       total_duration);
}

void run_analyzer_split(uint32_t test_duration,
                        k_thread_entry_t hi_thread_entry,
                        void *hi_arg0, void *hi_arg1, void *hi_arg2,
                        int hi_prio,  k_timeout_t hi_delay,
                        uint64_t *hi_duration,
                        k_thread_entry_t lo_thread_entry,
                        void *lo_arg0, void *lo_arg1, void *lo_arg2,
                        int lo_prio,  k_timeout_t lo_delay,
                        uint64_t *lo_duration,
                        uint64_t *total_duration)
{

	uint64_t start, hi, lo, end, elapsed;
    k_thread_runtime_stats_t hi_stats, lo_stats, start_stats, end_stats;
    
    k_thread_runtime_stats_all_get(&start_stats);
    
    supervisor_create_wrapper(test_duration,
        &super_thread, 
        super_stack,
        STACKSIZE,
        (k_thread_entry_t) super_entry,
        &hi_thread,
        &lo_thread);
    printk("Supervisor created, starting hi thread\n");
    thread_create_wrapper(&hi_thread,
        hi_stack,
        STACKSIZE,
        hi_thread_entry,
        hi_prio,
        hi_delay);
    printk("Hi thread created, starting lo thread\n");
    thread_create_wrapper(&lo_thread,
        lo_stack,
        STACKSIZE,
        lo_thread_entry,
        lo_prio,
        lo_delay);
    printk("Lo thread created\n");

    k_thread_join(&super_thread, K_MSEC(5500));
    printk("super joined\n");


    k_thread_runtime_stats_get(&hi_thread, &hi_stats);
    k_thread_runtime_stats_get(&lo_thread, &lo_stats);
    k_thread_runtime_stats_all_get(&end_stats);


    start = timing_cycles_to_ns(start_stats.execution_cycles) / 1000;
    hi = timing_cycles_to_ns(hi_stats.execution_cycles) / 1000;
    lo = timing_cycles_to_ns(lo_stats.execution_cycles) / 1000;
    end = timing_cycles_to_ns(end_stats.execution_cycles) / 1000;
    elapsed = end - start;

    printk("hi %lld lo %lld start %lld end %lld elapsed %lld (us)\n",
           hi, lo, start, end, elapsed);

    *hi_duration = hi;
    *lo_duration = lo;
    *total_duration = elapsed;

    k_thread_abort(&hi_thread);
    k_thread_abort(&lo_thread);
}