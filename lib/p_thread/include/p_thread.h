#pragma once
#include <zephyr.h>

void thread_setup(int priority, struct k_sem semaphore, char* thread_name, struct k_timer *timer, k_timeout_t timeout, k_timeout_t sleeptime);

void thread_counter(int* counter,
                    struct k_sem * semaphore,
                    char* thread_name,
                    struct k_timer *timer,
                    k_timeout_t timeout);

void main_thread(int* counter);

void thread_create_helper(struct k_thread * new_thread, k_thread_stack_t * stack, size_t stack_size, int priority, k_timeout_t wait);