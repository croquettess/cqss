/**
 * @file cqss_spinlock.h
 * @author croquettess
 * @date 2024-04-25
 *
 * @brief 自旋锁
 */
#ifndef CQSS_CORE_CQSS_SPINLOCK_H_
#define CQSS_CORE_CQSS_SPINLOCK_H_

#include "cqss_atomic.h"

typedef cqss_atomic_int_t cqss_spinlock_t;

/**
 * @brief 初始化自旋锁
 *
 * @param ptr 自旋锁指针
 */
#define cqss_spinlock_init(ptr) (*(ptr) = 0)

/**
 * @brief 销毁自旋锁
 *
 * @param ptr 自旋锁指针
 */
#define cqss_spinlock_destroy(ptr)

/**
 * @brief 加锁(阻塞)
 *
 * @param ptr 自旋锁指针
 */
#define cqss_spinlock_lock(ptr) _cqss_spinlock_lock(ptr)

/**
 * @brief 加锁(非阻塞)
 *
 * @param ptr 自旋锁指针
 * @return 1: 加锁成功 0: 加锁失败
 */
#define cqss_spinlock_try_lock(ptr) cqss_atomic_cmp_set(ptr, 0, 1)

/**
 * @brief 解锁
 *
 * @param ptr 自旋锁指针
 */
#define cqss_spinlock_unlock(ptr) (*(ptr) = 0)

/**
 * @brief 阻塞加锁具体实现
 *
 * @param ptr 自旋锁指针
 */
#define _cqss_spinlock_lock(ptr)              \
  do {                                        \
    while (!cqss_atomic_cmp_set(ptr, 0, 1)) { \
    }                                         \
  } while (0)

#endif  // CQSS_CORE_CQSS_SPINLOCK_H_
