#include "testlib.h"
#include "dispatcher.h"
#include <stdlib.h>

int t1Run = 0;
int t2Run = 0;

void t1() {
	for (int i = 0; i < 2; i++) {
		t1Run++;
		test_equals_int(t2Run, i, "t1: t2 ran before");
		yield();
	}
}

void t2() {
	for (int i = 0; i < 2; i++) {
		t2Run++;
		test_equals_int(t1Run, i + 1, "t2: t1 ran before");

		yield();
	}

	exit(test_end());
}

int main() {
	test_start("You schedule two threads the right way.");
	test_plan(6);
	initThreads();

	int tid1 = startThread(t1);
	test_equals_int(tid1, 1, "first thread gets tid 1");

	int tid2 = startThread(t2);
	test_equals_int(tid2, 2, "second thread gets tid 2");

	for (int i = 0; i < 100; i++) {
		yield();
	}

	test_assert(0, "Should never reach this.");

	return test_end();
}
