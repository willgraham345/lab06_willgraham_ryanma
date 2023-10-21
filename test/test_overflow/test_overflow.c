#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <unity.h>
#include "p_thread.h"




void setUp(void) {}

void tearDown(void) {}

void producer_thread_entry(struct k_msgq *queue, uint64_t *out_count, uint64_t *lost_count)
// void producer_thread_entry(struct k_msgq *my_msgq, uint64_t *lost_ctr, uint64_t *produce_ctr)
{
//     // Send a continuous message across my_msgq
//     printk("Starting the producer thread entry\n");
//     char msg = 'A';
//     while (1) {
//         (*produce_ctr)++;
//         if (k_msgq_put(my_msgq, &msg, K_NO_WAIT)) {
//             (*lost_ctr)++;
//         }
//     }
// }
    printk("Starting produce\n");
    while(1) {
        // Send the 95 printable ASCII characters
        for (char c = ' '; c < 0x7f; c++) {
            uint32_t u = (uint32_t)c;
            (*out_count)++;
            if (k_msgq_put(queue, &u, K_NO_WAIT)) {
                (*lost_count)++;
            }
        }
    }
}

void consumer_thread_entry(struct k_msgq *my_msgq, uint64_t *consume_ctr)
{
    // Receive a continuous message across my_msgq
    printk("Starting the consumer thread entry\n");
    uint32_t msg;
    while (1) {
        k_msgq_get(my_msgq, &msg, K_FOREVER);
        k_busy_wait(1000);
        (*consume_ctr)++;
    }
}

K_MSGQ_DEFINE(my_msgq, sizeof(uint32_t), 1000, 4);





void test_msgq_overflow(void)
{
    k_msgq_purge(&my_msgq);
    uint64_t consume_stats, produce_stats, elapsed_stats;
    uint64_t produce_ctr, lost_ctr, consume_ctr;
    produce_ctr = lost_ctr = consume_ctr = 0;
    run_analyzer_two_entry_with_args(100,
                       (k_thread_entry_t)producer_thread_entry, &my_msgq, &lost_ctr, &produce_ctr,
                       K_PRIO_PREEMPT(4), K_MSEC(10), &produce_stats,
                       (k_thread_entry_t)consumer_thread_entry, &my_msgq, &consume_ctr,
                       K_PRIO_PREEMPT(3), K_MSEC(12), &consume_stats,
                       &elapsed_stats);
    printk("produce_stats: %lld\n", produce_stats);
    printk("consume_stats: %lld\n", consume_stats);
    printk("elapsed_stats: %lld\n", elapsed_stats);
    printk("produce_ctr: %lld\n", produce_ctr);
    printk("lost_ctr: %lld\n", lost_ctr);
    printk("consume_ctr: %lld\n", consume_ctr);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, produce_stats);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, consume_stats);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, elapsed_stats);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, produce_ctr);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, lost_ctr);
    TEST_ASSERT_UINT64_WITHIN(1000, 0, consume_ctr);
}


int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_msgq_overflow);
    
    return UNITY_END();
}