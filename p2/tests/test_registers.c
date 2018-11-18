#include "testlib.h"
#include "dispatcher.h"
#include <inttypes.h>
#include <stdlib.h>

typedef uint64_t reg_t;

int t1Done = 0;

void t1() {
	register reg_t p1 __asm__ ("rbp");
	reg_t oldP1 = p1;	
	register reg_t p2 __asm__ ("rbx");
	p2 = 42;
	register reg_t p3 __asm__ ("r12");
	p3 = 43;
	register reg_t p4 __asm__ ("r13");
	p4 = 44;
	register reg_t p5 __asm__ ("r14");
	p5 = 45;
	register reg_t p6 __asm__ ("r15");
	p6 = 46;

	yield();

	reg_t newP1 = p1;
	reg_t newP2 = p2;
	reg_t newP3 = p3;
	reg_t newP4 = p4;
	reg_t newP5 = p5;
	reg_t newP6 = p6;

	test_equals_int64(newP1, oldP1, "rbp is restored");
	test_equals_int64(newP2, 42, "rbx is restored");
	test_equals_int64(newP3, 43, "r12 is restored");
	test_equals_int64(newP4, 44, "r13 is restored");
	test_equals_int64(newP5, 45, "r14 is restored");
	test_equals_int64(newP6, 46, "r15 is restored");

	t1Done = 1;

	while (1) 
		yield();
}

int main() {
	test_start("You push all required registers.");
	test_plan(7);
	initThreads();

	startThread(t1);

	for (int i = 0; i < 2; i++) {
		yield();
	}

	test_equals_int(t1Done, 1, "t1 is done");

	return test_end();
}
