### [Activity 0](https://github.com/uofu-emb/rtos/tree/main/labs/6.scheduling#activity-0)

Induce priority inversion.

1. create two preemptable threads (and a supervisor to manage them).
2. create a semaphore shared between the two.
3. set one to higher priority. set it to delay start.
4. have the lower priority thread acquire the semphore first.
5. verify that the high priority thread will not execute.

You may find it useful to create a helper function to setup and teardown the threads.


### [Activity 1](https://github.com/uofu-emb/rtos/tree/main/labs/6.scheduling#activity-1)

In this activity, you'll create two competing threads that use the following functions

```
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
```

Write tests for two threads running the following scenarios. Try to predict the runtime for each thread.

1. Threads with same priority:
    1. Coop
        1. Both run `busy_busy`.
        2. Both run `busy_yield`
    2. Preempt
        1. Both run `busy_busy`.
        2. Both run `busy_yield`
2. Threads with different priority.
    1. Coop
        1. Both run `busy_busy`.
            1. Higher priority starts first.
            2. Lower priority starts first.
        2. Both run `busy_yield`
            1. Higher priority starts first.
            2. Lower priority starts first.
    2. Preempt
        1. Both run `busy_busy`.
            1. Higher priority starts first.
            2. Lower priority starts first.
        2. Both run `busy_yield`
            1. Higher priority starts first.
            2. Lower priority starts first.

Make sure you are setting priorities according to the priority order presented in the documentation.

### [Activity 2](https://github.com/uofu-emb/rtos/tree/main/labs/6.scheduling#activity-2)

```
void busy_sleep(char *name)
{
    k_busy_wait(10000);
    k_sleep(K_MSEC(490));
}
```

1. Write tests for two threads: one should run `busy_sleep` and the other `busy_yield`
2. Predict the share of the runtime for each of the threads.

### [Activity 3](https://github.com/uofu-emb/rtos/tree/main/labs/6.scheduling#activity-3)

Message queues and other communication channels can become saturated with data if the

1. Create a message queue
2. Create a producer thread and a consumer thread.
3. The producer should produce data faster than the consumer consumes.
    1. Add a busy wait to the consumer to emulate a long computation.
    2. The producer should produce data to the queue. If the queue is full, lose the data.
4. Track the production and consumption rates, the throughput and any data loss.

You'll need to pay attention to the reshedule points in your threads and their priority.

### [Activity 4](https://github.com/uofu-emb/rtos/tree/main/labs/6.scheduling#activity-4)

1. Create a variant on the producer consumer from activity 3, but make it lossless.
