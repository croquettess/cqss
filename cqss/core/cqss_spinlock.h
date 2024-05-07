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

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

typedef cqss_atomic_size_t cqss_spin_t;

/**
 * @brief 初始化自旋锁
 *
 * @param ptr 自旋锁指针
 */
#define cqss_spin_init(ptr) cqss_atomic_store(ptr, 0)

/**
 * @brief 销毁自旋锁
 *
 * @param ptr 自旋锁指针
 */
#define cqss_spin_destroy(ptr)

/**
 * @brief 加锁
 *
 * @param ptr 自旋锁指针
 */
void cqss_spin_lock(cqss_spin_t *lock);

/**
 * @brief 解锁
 *
 * @param ptr 自旋锁指针
 */
#define cqss_spin_unlock(ptr) cqss_atomic_store(ptr, 0)

#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  // CQSS_CORE_CQSS_SPINLOCK_H_
