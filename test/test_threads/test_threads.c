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
    printk("start priority inversion %s\n", name);
    k_sem_take(sem, K_FOREVER);
    printk("got semaphore %s\n", name);
    for (int i = 0; ; i++);
}

// Activity 1
void test_priority_inversion(void)
{
    printk("Starting prioirty inversion test %s\n", name);
    k_sem_take()
}





int main (void)
{

    UNITY_BEGIN();
    RUN_TEST(test_priority_inversion);
}