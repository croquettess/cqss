/**
 * @file cqss_atomic.h
 * @author croquettess
 * @date 2024-04-25
 *
 * @brief	原子变量
 */
#ifndef CQSS_CORE_CQSS_ATOMIC_H_
#define CQSS_CORE_CQSS_ATOMIC_H_

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "cqss_core.h"

typedef volatile int cqss_atomic_int_t;

typedef volatile cqss_int8_t cqss_atomic_int8_t;
typedef volatile cqss_int16_t cqss_atomic_int16_t;
typedef volatile cqss_int32_t cqss_atomic_int32_t;
typedef volatile cqss_int64_t cqss_atomic_int64_t;
typedef volatile cqss_ssize_t cqss_atomic_ssize_t;

typedef volatile cqss_uint8_t cqss_atomic_uint8_t;
typedef volatile cqss_uint16_t cqss_atomic_uint16_t;
typedef volatile cqss_uint32_t cqss_atomic_uint32_t;
typedef volatile cqss_uint64_t cqss_atomic_uint64_t;
typedef volatile cqss_size_t cqss_atomic_size_t;
typedef volatile uintptr_t cqss_atomic_pointer_t;

/**
 * @brief 设置内存屏障
 *
 * @return
 */
#define cqss_memory_barrier() __sync_synchronize()

/**
 * @brief 初始化原子变量
 *
 * @return
 */
#define cqss_atomic_store(ptr, val) (*(ptr) = val)

/**
 * @brief 加载原子变量
 *
 * @return
 */
#define cqss_atomic_load(ptr) (*(ptr))

/**
 * @brief 原子变量比较交换
 *
 * @return true 成功
 * @return false 失败
 */
#define cqss_atomic_cmp_set(lock, old, set) \
  __sync_bool_compare_and_swap(lock, old, set)

/**
 * @brief 自增
 *
 * @param ptr 原子变量指针
 * @return
 */
#define cqss_atomic_inc(ptr) __sync_fetch_and_add(ptr, 1)

/**
 * @brief 自减
 *
 * @param ptr 原子变量指针
 * @return
 */
#define cqss_atomic_dec(ptr) __sync_fetch_and_sub(ptr, 1)

/**
 * @brief 加法
 *
 * @param ptr 被加数原子变量指针
 * @param n   加数
 * @return
 */
#define cqss_atomic_add(ptr, n) __sync_fetch_and_add(ptr, n)

/**
 * @brief 减法
 *
 * @param ptr 被减数原子变量指针
 * @param n   减数
 * @return
 */
#define cqss_atomic_sub(ptr, n) __sync_fetch_and_sub(ptr, n)

/**
 * @brief 与运算
 *
 * @param ptr 被与数原子变量指针
 * @param n   与数
 * @return
 */
#define cqss_atomic_and(ptr, n) __sync_fetch_and_and(ptr, n)

/**
 * @brief 或运算
 *
 * @param ptr 被或数原子变量指针
 * @param n   或数
 * @return
 */
#define cqss_atomic_or(ptr, n) __sync_fetch_and_or(ptr, n)

/**
 * @brief 异或运算
 *
 * @param ptr 被异或数原子变量指针
 * @param n   异或数
 * @return
 */
#define cqss_atomic_xor(ptr, n) __sync_fetch_and_xor(ptr, n)

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // CQSS_CORE_CQSS_ATOMIC_H_
