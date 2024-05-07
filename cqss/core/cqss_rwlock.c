/**
 * @file cqss_rwlock.c
 * @author croquettess
 * @date 2024-05-07
 *
 * @brief 读写锁实现
 */

#include "cqss_rwlock.h"

#include "cqss_core.h"


#define CQSS_RWLOCK_MAX_SPIN ((cqss_size_t)2048)
#define CQSS_RWLOCK_UNLOCK ((cqss_size_t)0)
#define CQSS_RWLOCK_WTLOCK ((cqss_size_t)-1)

/**
 * @brief 读写锁是否被加锁
 *
 * @param lock 读写锁指针
 * @return true 已加锁
 * @return false 未加锁
 */
static inline bool cqss_rwlock_is_lock(cqss_rwlock_t *lock);

/**
 * @brief 读写锁是否被加写锁
 *
 * @param lock 读写锁指针
 * @param ref_cnt 读写锁引用计数
 * @return true 已加写锁
 * @return false 未加写锁
 */
static inline bool cqss_rwlock_is_wtlock(cqss_rwlock_t *lock,
                                         cqss_atomic_size_t *ref_cnt);
/**
 * @brief 尝试加写锁
 *
 * @param lock 读写锁指针
 * @return cqss_size_t CQSS_RWLOCK_UNLOCK
 */
static inline cqss_size_t cqss_rwlock_try_wtlock(cqss_rwlock_t *lock);

/**
 * @brief
 *
 * @param lock 读写锁指针
 * @param ref_cnt 读写锁引用计数
 * @return cqss_size_t 加锁前的读写锁引用计数
 */
static inline cqss_size_t cqss_rwlock_try_rdlock(cqss_rwlock_t *lock,
                                                 cqss_atomic_size_t ref_cnt);

static inline bool cqss_rwlock_is_lock(cqss_rwlock_t *lock) {
  return CQSS_RWLOCK_UNLOCK != cqss_atomic_load(lock);
}

static inline bool cqss_rwlock_is_wtlock(cqss_rwlock_t *lock,
                                         cqss_atomic_size_t *ref_cnt) {
  cqss_atomic_store(ref_cnt, *lock);
  return CQSS_RWLOCK_WTLOCK == *ref_cnt;
}

static inline cqss_size_t cqss_rwlock_try_wtlock(cqss_rwlock_t *lock) {
  return cqss_atomic_cmp_set(lock, CQSS_RWLOCK_UNLOCK, CQSS_RWLOCK_WTLOCK);
}

static inline cqss_size_t cqss_rwlock_try_rdlock(cqss_rwlock_t *lock,
                                                 cqss_atomic_size_t ref_cnt) {
  return cqss_atomic_cmp_set(lock, ref_cnt, ref_cnt + 1);
}

void cqss_rwlock_init(cqss_rwlock_t *lock) { cqss_atomic_store(lock, 0); }

void cqss_rwlock_destroy(cqss_rwlock_t *lock) { (void)lock; }

void cqss_rwlock_wtlock(cqss_rwlock_t *lock) {
  while (true) {
    if (!cqss_rwlock_is_lock(lock) && cqss_rwlock_try_wtlock(lock)) {
      return;
    }

    if (cqss_ncore > 1) {
      for (cqss_size_t n = 1; n < CQSS_RWLOCK_MAX_SPIN; n <<= 1) {
        for (cqss_size_t i = 0; i < n; ++i) {
          cqss_cpu_pause();
        }

        if (!cqss_rwlock_is_lock(lock) && cqss_rwlock_try_wtlock(lock)) {
          return;
        }
      }
    }
    cqss_sched_yield();
  }
}

void cqss_rwlock_rdlock(cqss_rwlock_t *lock) {
  cqss_atomic_size_t ref_cnt;

  while (true) {
    if (!cqss_rwlock_is_wtlock(lock, &ref_cnt) &&
        cqss_rwlock_try_rdlock(lock, ref_cnt)) {
      return;
    }

    if (cqss_ncore > 1) {
      for (cqss_size_t n = 1; n < CQSS_RWLOCK_MAX_SPIN; n <<= 1) {
        for (cqss_size_t i = 0; i < n; ++i) {
          cqss_cpu_pause();
        }

        if (!cqss_rwlock_is_wtlock(lock, &ref_cnt) &&
            cqss_rwlock_try_rdlock(lock, ref_cnt)) {
          return;
        }
      }
    }
    cqss_sched_yield();
  }
}

void cqss_rwlock_unlock(cqss_rwlock_t *lock) {
  cqss_atomic_size_t ref_cnt;
  if (cqss_rwlock_is_wtlock(lock, &ref_cnt)) {
    (void)cqss_atomic_cmp_set(lock, CQSS_RWLOCK_WTLOCK, CQSS_RWLOCK_UNLOCK);
    return;
  }

  while (true) {
    if (cqss_atomic_cmp_set(lock, ref_cnt, ref_cnt - 1)) {
      return;
    }
    cqss_atomic_store(&ref_cnt, *lock);
  }
}
