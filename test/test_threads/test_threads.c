#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <unity.h>
#include "p_thread.c"

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
    k_sleep(K_MSEC(490)); //Doesn't release semaphore
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

// Activity 0
void test_priority_inversion(void)
{
    uint64_t high_stats, low_stats, elapsed_stats;
    struct k_sem sem;
    k_sem_init(&sem, 1, 1);

    run_analyzer((k_thread_entry_t)priority_inversion, &sem,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &high_stats,
                 K_PRIO_PREEMPT(2), K_MSEC(10), &low_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, high_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, low_stats);
}

// Activity 1
void test_same_priority(void)
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
void test_coop__no_priority__no_yield(void)
{
    uint64_t first_stats, second_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_COOP(3), K_MSEC(10), &first_stats,
                 K_PRIO_COOP(3), K_MSEC(12), &second_stats,
                 &elapsed_stats);
    TEST_ASSERT_EQUAL_UINT64(0, second_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, first_stats);
}

void test_preempt__no_priority__no_yield(void)
{
    uint64_t first_stats, second_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &first_stats,
                 K_PRIO_PREEMPT(3), K_MSEC(12), &second_stats,
                 &elapsed_stats);
    TEST_ASSERT_EQUAL_UINT64(0, second_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, first_stats);
}

void test_coop__no_priority__yield(void)
{
    uint64_t first_stats, second_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_COOP(3), K_MSEC(10), &first_stats,
                 K_PRIO_COOP(3), K_MSEC(10), &second_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 2500000, first_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 2500000, second_stats);
}

void test_preempt__no_priority__yield(void)
{
    uint64_t first_stats, second_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &first_stats,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &second_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 2500000, first_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 2500000, second_stats);
}

void test_coop__priority_low_first__no_yield(void)
{
    uint64_t high_stats, low_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_COOP(3), K_MSEC(10), &low_stats,
                 K_PRIO_COOP(2), K_MSEC(12), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_EQUAL_UINT64(0, high_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, low_stats);
}

void test_coop__priority_high_first__no_yield(void)
{
    uint64_t high_stats, low_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_COOP(3), K_MSEC(12), &low_stats,
                 K_PRIO_COOP(2), K_MSEC(10), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_EQUAL_UINT64(0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}

void test_coop__priority_low_first__yield(void)
{
    uint64_t low_stats, high_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_COOP(3), K_MSEC(10), &low_stats,
                 K_PRIO_COOP(2), K_MSEC(12), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(5000, 0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}

void test_coop__priority_high_first__yield(void)
{
    uint64_t low_stats, high_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_COOP(3), K_MSEC(10), &low_stats,
                 K_PRIO_COOP(2), K_MSEC(12), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(5000, 0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}

void test_preempt__priority__no_yield(void)
{
    uint64_t low_stats, high_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_busy,  NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &low_stats,
                 K_PRIO_PREEMPT(2), K_MSEC(10), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(5000, 0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}

void test_preempt__priority__yield(void)
{
    uint64_t low_stats, high_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &low_stats,
                 K_PRIO_PREEMPT(2), K_MSEC(10), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(5000, 0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}


void test_mix__priority__yield(void)
{

    uint64_t low_stats, high_stats, elapsed_stats;
    run_analyzer((k_thread_entry_t)busy_yield,  NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &low_stats,
                 K_PRIO_COOP(3), K_MSEC(12), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(5000, 0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}



// Activity 2

void test_preempt__busy_and_yield(void)
{
    uint64_t low_stats, high_stats, elapsed_stats;
    // Names kept same as high and low for convenience
    // hi = busy_sleep, lo = busy_yield
    run_analyzer_two_entry((k_thread_entry_t)busy_sleep, (k_thread_entry_t)busy_yield, NULL,
                 K_PRIO_PREEMPT(3), K_MSEC(10), &low_stats,
                 K_PRIO_PREEMPT(2), K_MSEC(10), &high_stats,
                 &elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(5000, 0, low_stats);
    TEST_ASSERT_UINT64_WITHIN(100000, 5000000, high_stats);
}
int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_priority_inversion);
    RUN_TEST(test_same_priority);
    RUN_TEST(test_coop__no_priority__no_yield);
    RUN_TEST(test_preempt__no_priority__no_yield);
    RUN_TEST(test_coop__no_priority__yield);
    RUN_TEST(test_preempt__no_priority__yield);
    RUN_TEST(test_coop__priority_low_first__no_yield);
    RUN_TEST(test_coop__priority_high_first__no_yield);
    RUN_TEST(test_coop__priority_low_first__yield);
    RUN_TEST(test_coop__priority_high_first__yield);
    RUN_TEST(test_preempt__priority__no_yield);
    RUN_TEST(test_preempt__priority__yield);
    RUN_TEST(test_mix__priority__yield);
    
    return UNITY_END();
}