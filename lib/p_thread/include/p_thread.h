#include <kernel.h>
#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>


void run_analyzer(k_thread_entry_t thread_entry,
                  void *arg,
                  int pri_prio,  k_timeout_t pri_delay,
                  uint64_t *pri_duration,
                  int sec_prio,  k_timeout_t sec_delay,
                  uint64_t *sec_duration,
                  uint64_t *total_duration);

void run_analyzer_split(uint32_t test_duration,
                        k_thread_entry_t pri_thread_entry,
                        void *pri_arg0, void *pri_arg1, void *pri_arg2,
                        int pri_prio,  k_timeout_t pri_delay,
                        uint64_t *pri_duration,

                        k_thread_entry_t sec_thread_entry,
                        void *sec_arg0, void *sec_arg1, void *sec_arg2,
                        int sec_prio,  k_timeout_t sec_delay,
                        uint64_t *sec_duration,

                        uint64_t *total_duration);

void main_thread();

// Thread functions
void thread_create_wrapper(struct k_thread * new_thread, k_thread_stack_t * stack, size_t stack_size, int priority, k_timeout_t wait);
void thread_entry();

// Supervisor functions
void supervisor_create_wrapper(struct k_thread * new_thread, k_thread_stack_t * stack, size_t stack_size);
void supervisor_entry();