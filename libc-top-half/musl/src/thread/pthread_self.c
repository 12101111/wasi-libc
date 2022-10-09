#include "pthread_impl.h"
#include <threads.h>

#if !defined(__wasilibc_unmodified_upstream) && defined(__wasm__) &&           \
    defined(_REENTRANT)
// We need some place to store the thread ID. This WebAssembly global fits the
// bill and is used by `__get_tp` elsewhere.
__asm__(".globaltype __wasilibc_pthread_self, i32\n"
        "__wasilibc_pthread_self:\n");
#endif

uintptr_t __get_tp(void) {
#if _REENTRANT
  int val;
  __asm__("global.get __wasilibc_pthread_self\n"
          "local.set %0"
          : "=r"(val));
  return val;
#else
  return 0;
#endif
}

void __set_tp(void *p) {
#if _REENTRANT
  __asm__("local.get %0\n"
          "global.set __wasilibc_pthread_self\n"
          :: "r"(p));
#endif
}

static pthread_t __pthread_self_internal()
{
	return __pthread_self();
}

weak_alias(__pthread_self_internal, pthread_self);
weak_alias(__pthread_self_internal, thrd_current);
