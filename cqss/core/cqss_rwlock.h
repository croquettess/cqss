/**
 * @file cqss_rwlock.h
 * @author croquettess
 * @date 2024-05-07
 *
 * @brief 读写锁
 */
#ifndef CQSS_CORE_CQSS_RWLOCK_H_
#define CQSS_CORE_CQSS_RWLOCK_H_

#include "cqss_atomic.h"

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus

typedef cqss_atomic_size_t cqss_rwlock_t;

/**
 * @brief 初始化读写锁
 *
 * @param lock 读写锁指针
 */
void cqss_rwlock_init(cqss_rwlock_t *lock);

/**
 * @brief 销毁读写锁
 *
 * @param lock 读写锁指针
 */
void cqss_rwlock_destroy(cqss_rwlock_t *lock);

/**
 * @brief 读锁
 *
 * @param lock 读写锁指针
 */
void cqss_rwlock_wtlock(cqss_rwlock_t *lock);

/**
 * @brief 写锁
 *
 * @param lock 读写锁指针
 */
void cqss_rwlock_rdlock(cqss_rwlock_t *lock);

/**
 * @brief 解锁
 *
 * @param lock 读写锁指针
 */
void cqss_rwlock_unlock(cqss_rwlock_t *lock);

#ifdef __cplusplus
}
#endif  //__cplusplus

#endif  // CQSS_CORE_CQSS_RWLOCK_H_
