#include "cqss_spinlock.h"

#include "cqss_core.h"

#define CQSS_SPIN_UNLOCK ((cqss_size_t)0)
#define CQSS_SPIN_LOCK ((cqss_size_t)1)
#define CQSS_SPIN_MAX_SPIN ((cqss_size_t)2048)

/**
 * @brief 检查是否加锁
 *
 * @param lock 自旋锁指针
 * @return true 已加锁
 * @return false 未加锁
 */
static inline bool cqss_spin_is_lock(cqss_spin_t *lock);

/**
 * @brief 尝试加锁
 *
 * @param lock 自旋锁指针
 * @return true 加锁成功
 * @return false 加锁失败
 */
static inline bool cqss_spin_try_lock(cqss_spin_t *lock);

static inline bool cqss_spin_is_lock(cqss_spin_t *lock) {
  return CQSS_SPIN_LOCK == cqss_atomic_load(lock);
}

static inline bool cqss_spin_try_lock(cqss_spin_t *lock) {
  return cqss_atomic_cmp_set(lock, CQSS_SPIN_UNLOCK, CQSS_SPIN_LOCK);
}

void cqss_spin_lock(cqss_spin_t *lock) {
  cqss_size_t n, i;

  while (true) {
    if (!cqss_spin_is_lock(lock) &&  cqss_spin_try_lock(lock)) {
      return;
    }

    if (cqss_ncore > 1) {
      for (n = 1; n < CQSS_SPIN_MAX_SPIN; n <<= 1) {
        for (i = 0; i < n; ++i) {
          cqss_cpu_pause();
        }

        if (!cqss_spin_is_lock(lock) && cqss_spin_try_lock(lock)) {
          return;
        }
      }
      cqss_sched_yield();
    }
  }
}
