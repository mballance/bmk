#include <bmk.h>
#include "libfemto/include/stdlib.h"

extern char _bss_end, _memory_end;

// Only configure core0
void bmk_level0_main(void) {
	_malloc_addblock(&_bss_end, &_memory_end - &_bss_end);

	{
		uint8_t *stk = (uint8_t *)malloc(16384);
		bmk_info_low("bmk_level0_main");

		bmk_init_core(0, stk, 16384);
	}
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

static int t1_f(void *ud) {
	uint32_t i;
	msg_t *m = (msg_t *)ud;
	bmk_info_low("Hello from t1_f\n");

	for (i=0; i<16; i++) {
		// This thread sends first
		bmk_info_low("--> t1: send %d", i);
		bmk_mutex_lock(&m->send_mutex);
		m->send_ready = 1;
		bmk_cond_signal(&m->send_cond);
		bmk_mutex_unlock(&m->send_mutex);
		bmk_info_low("<-- t1: send %d", i);

		// Now, wait for receive
		bmk_info_low("--> t1: recv %d", i);
		bmk_mutex_lock(&m->recv_mutex);
		while (!m->recv_ready) {
			bmk_cond_wait(&m->recv_cond, &m->recv_mutex);
		}
		m->recv_ready = 0;
		bmk_mutex_unlock(&m->recv_mutex);
		bmk_info_low("<-- t1: recv %d", i);
	}

	bmk_info_low("Goodbye from t1_f\n");

	return 0;
}

static int t2_f(void *ud) {
	uint32_t i;
	msg_t *m = (msg_t *)ud;
	bmk_info_low("Hello from t2_f\n");

	for (i=0; i<16; i++) {
		// This thread receives first
		bmk_info_low("--> t2: recv %d", i);
		bmk_mutex_lock(&m->send_mutex);
		while (!m->send_ready) {
			bmk_cond_wait(&m->send_cond, &m->send_mutex);
		}
		m->send_ready = 0;
		bmk_cond_signal(&m->send_cond);
		bmk_mutex_unlock(&m->send_mutex);
		bmk_info_low("<-- t2: recv %d", i);

		// Now, send something back
		bmk_info_low("--> t2: send %d", i);
		bmk_mutex_lock(&m->recv_mutex);
		m->recv_ready = 1;
		bmk_cond_signal(&m->recv_cond);
		bmk_mutex_unlock(&m->recv_mutex);
		bmk_info_low("<-- t2: send %d", i);
	}

	bmk_info_low("Goodbye from t2_f\n");

	return 0;
}

void bmk_main(void) {
	msg_t			m;
	uint8_t *t1_stk = (uint8_t *)malloc(4096);
	uint8_t *t2_stk = (uint8_t *)malloc(4096);
	bmk_thread_t t1, t2;

	bmk_info_low("Hello from bmk_main");

	bmk_mutex_init(&m.send_mutex);
	bmk_cond_init(&m.send_cond);
	m.send_ready = 0;
	bmk_mutex_init(&m.recv_mutex);
	bmk_cond_init(&m.recv_cond);
	m.recv_ready = 0;

	bmk_thread_init(&t1, t1_stk, 4096, &t1_f, &m);
	bmk_thread_init(&t2, t2_stk, 4096, &t2_f, &m);

	bmk_thread_join(&t1);
	bmk_thread_join(&t2);
}


