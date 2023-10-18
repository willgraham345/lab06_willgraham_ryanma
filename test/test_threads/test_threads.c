#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <unity.h>
#include "helper.c"

void setUp(void) {}

void tearDown(void) {}

void busy_busy(char *name)
{
    for (int i = 0; ; i++);
}

void busy_yield(char *name)
{
    for (int i = 0; ; i++) {
        if (!(i & 0xFF)) {
            k_yield();
        }
    }
}

void busy_sleep(char *name)
{
    printk("start busy_busy %s\n", name);
    k_busy_wait(10000);
    k_sleep(K_MSEC(490));
}


void priority_inversion(char *name, struct k_sem *sem)
{
    // Tests if a lower priority thread will keep a higher priority thread from the semaphore.
    // Think about when to test the runtime
    printk("start priority inversion %s\n", name);
    k_sem_take(sem, K_FOREVER);
    printk("got semaphore %s\n", name);
    for (int i = 0; ; i++);
}

// Activity 1
void test_priority_inversion(void)
{
    uint64_t high_stats, low_stats, elapsed_stats;
    struct k_sem sem;
    k_sem_init(&sem, 1, 1);

    run_analyzer((k_thread_entry_t)priority_inversion, &sem,
                 K_PRIO_PREEMPT(4), K_MSEC(10), &low_stats,
                 K_PRIO_PREEMPT(3), K_MSEC(12), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, high_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, low_stats);
}




int main (void)
{

    UNITY_BEGIN();
    RUN_TEST(test_priority_inversion);
}