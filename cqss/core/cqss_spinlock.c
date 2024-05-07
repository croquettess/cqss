#include "cqss_spinlock.h"

#include "cqss_core.h"

#define CQSS_SPIN_UNLOCK ((cqss_size_t)0)
#define CQSS_SPIN_LOCK ((cqss_size_t)1)
#define CQSS_SPIN_MAX_SPIN ((cqss_size_t)2048)

static inline bool cqss_spin_is_lock(cqss_spin_t *lock);
static inline cqss_size_t cqss_spin_try_lock(cqss_spin_t *lock);

static inline bool cqss_spin_is_lock(cqss_spin_t *lock) {
  return CQSS_SPIN_UNLOCK == cqss_atomic_load(lock);
}

static inline cqss_size_t cqss_spin_try_lock(cqss_spin_t *lock) {
  return cqss_atomic_cmp_set(lock, CQSS_SPIN_UNLOCK, CQSS_SPIN_LOCK);
}

void cqss_spin_lock(cqss_spin_t *lock) {
  cqss_size_t n, i;

  while (true) {
    if (cqss_spin_is_lock(lock) &&  cqss_spin_try_lock(lock)) {
      return;
    }

    if (cqss_ncore > 1) {
      for (n = 1; n < CQSS_SPIN_MAX_SPIN; n <<= 1) {
        for (i = 0; i < n; ++i) {
          cqss_cpu_pause();
        }

        if (cqss_spin_is_lock(lock) && cqss_spin_try_lock(lock)) {
          return;
        }
      }
      cqss_sched_yield();
    }
  }
}
