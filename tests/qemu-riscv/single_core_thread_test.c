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

//void bmk_level1_main(uint32_t cid) {
//	uint8_t p;
//	bmk_info_low("bmk_level1_main %d p=%p", cid, &p);
//}

static int t1_f(void *ud) {
  bmk_info_low("Hello from t1_f\n");
  return 0;
}

static int t2_f(void *ud) {
  bmk_info_low("Hello from t2_f\n");
  return 0;
}

void bmk_main(void) {
  uint8_t *t1_stk = (uint8_t *)malloc(4096);
  uint8_t *t2_stk = (uint8_t *)malloc(4096);
  bmk_thread_t t1, t2;

  bmk_info_low("Hello from bmk_main");

  bmk_thread_init(&t1, t1_stk, 4096, &t1_f, 0);
  bmk_thread_init(&t2, t2_stk, 4096, &t2_f, 0);

  bmk_thread_join(&t1);
  bmk_thread_join(&t2);
}


