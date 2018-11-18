#include "testlib.h"
#include "dispatcher.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

// This does return an address close to the top of the stack.
void *getSp()
{
    int i;
    uintptr_t iPos = (uintptr_t)&i;
    return (void*) iPos;
}

void t1()
{
    printf("Thread 1: Step 1, sp is %p\n", getSp());
    sleep(1);
    yield();
    printf("Thread 1: Step 2, sp is %p\n", getSp());
    sleep(1);
    yield();
    // Thread should terminate.
}

void t2()
{
    for (int i = 1; ; i++) {
        printf("Thread 2: Step %d, sp is %p\n", i, getSp());
        sleep(1);
        yield();
    }
}

int main() {
    test_start("dispatcher.c");
    initThreads();

    int tid1 = startThread(t1);
    printf("Started thread: %d\n", tid1);
    test_equals_int(tid1, 1, "gf");

    int tid2 = startThread(t2);
    printf("Started thread: %d\n", tid2);
    test_equals_int(tid2, 2, "sdf");

    yield();
    for (int i = 1; ; i++) {
        printf("Thread 0: Step %d, sp is %p\n", i, getSp());
        sleep(1);
        yield();
    }

    return test_end();
}
