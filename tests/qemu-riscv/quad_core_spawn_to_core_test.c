#include <bmk.h>
#include <stdint.h>
#include "libfemto/include/stdlib.h"

extern char _bss_end, _memory_end;

bmk_atomic_t		lock;

// Only configure core0
void bmk_level0_main(void) {
	_malloc_addblock(&_bss_end, &_memory_end - &_bss_end);

	{
		uint8_t *stk = (uint8_t *)malloc(16384);
		bmk_info_low("bmk_level0_main");

		bmk_init_core(0, stk, 16384);

		stk = (uint8_t *)malloc(16384);
		bmk_init_core(1, stk, 16384);
	}

    bmk_atomics_init(&lock);
}

typedef struct msg_s {
	bmk_mutex_t		send_mutex;
	bmk_cond_t		send_cond;
	uint32_t		send_ready;

	bmk_mutex_t		recv_mutex;
	bmk_cond_t		recv_cond;
	uint32_t		recv_ready;
} msg_t;

//void bmk_level1_main(uint32_t cid) {
//	uint8_t p;
//	bmk_info_low("bmk_level1_main %d p=%p", cid, &p);
//}

static int thread_func(void *ud) {
	uintptr_t target_core = (uintptr_t)ud;
	bmk_cpuset_t cpuset;

	bmk_info_low("thread_func: core=%d target=%d",
			bmk_get_procid(), (int)target_core);

	bmk_cpuset_zero(&cpuset);
	bmk_cpuset_set(target_core, &cpuset);
	bmk_thread_setaffinity(0, &cpuset);

	while (bmk_get_procid() != target_core) {
		bmk_thread_yield();
	}

	bmk_info_low("thread_func (post-setaffinity): core=%d target=%d",
			bmk_get_procid(), (int)target_core);

	return 0;
}

void bmk_main(void) {
	msg_t			m;
	uint8_t *t1_stk = (uint8_t *)malloc(4096);
	uint8_t *t2_stk = (uint8_t *)malloc(4096);
	uint8_t *t3_stk = (uint8_t *)malloc(4096);
	bmk_thread_t t1, t2, t3;

	bmk_info_low("Hello from bmk_main");

	bmk_thread_init(&t1, t1_stk, 4096, &thread_func, (uintptr_t)1);
	bmk_thread_init(&t2, t2_stk, 4096, &thread_func, (uintptr_t)2);
	bmk_thread_init(&t3, t3_stk, 4096, &thread_func, (uintptr_t)3);

	bmk_thread_join(&t1);
	bmk_thread_join(&t2);
	bmk_thread_join(&t3);
}


