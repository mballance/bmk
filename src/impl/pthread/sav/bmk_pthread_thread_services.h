
#ifndef INCLUDED_BMK_PTHREAD_THREAD_SERVICES_H
#define INCLUDED_BMK_PTHREAD_THREAD_SERVICES_H
#include "bmk_thread_types.h"
#include <pthread.h>
#include <ucontext.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct bmk_thread_pthread_s {
	// We want our own user threads for this
	ucontext_t					ctxt;
	uint64_t					procmask;
	bmk_thread_main_f			main_f;
	void						*main_ud;
	uint32_t					dead;
	struct bmk_thread_pthread_s *next; //
} bmk_thread_pthread_t;

typedef struct bmk_mutex_pthread_s {

} bmk_mutex_pthread_t;

typedef struct bmk_cond_pthread_s {

} bmk_cond_pthread_t;

typedef bmk_thread_pthread_t bmk_thread_t;
typedef bmk_mutex_pthread_t bmk_mutex_t;
typedef bmk_cond_pthread_t bmk_cond_t;


#ifdef __cplusplus
}
#endif
#endif /* INCLUDED_BMK_PTHREAD_THREAD_SERVICES_H */
