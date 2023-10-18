#pragma once
#include <zephyr.h>


void main_thread();

// Thread functions
void thread_create_wrapper(struct k_thread * new_thread, k_thread_stack_t * stack, size_t stack_size, int priority, k_timeout_t wait);
void thread_entry();

// Supervisor functions
void supervisor_create_wrapper(struct k_thread * new_thread, k_thread_stack_t * stack, size_t stack_size);
void supervisor_entry();