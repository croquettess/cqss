#ifndef CQSS_CORE_CQSS_CORE_H_
#define CQSS_CORE_CQSS_CORE_H_

#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef char cqss_int8_t;
typedef short cqss_int16_t;
typedef int cqss_int32_t;
typedef long cqss_int64_t;
typedef long cqss_ssize_t;

typedef unsigned char cqss_uint8_t;
typedef unsigned short cqss_uint16_t;
typedef unsigned int cqss_uint32_t;
typedef unsigned long cqss_uint64_t;
typedef unsigned long cqss_size_t;
typedef uintptr_t cqss_uintptr_t;

// extern cqss_size_t cqss_ncore = sysconf(_SC_NPROCESSORS_ONLN);
extern cqss_size_t cqss_ncore;

/**
 * @brief 主动让出 CPU 时间片
 *
 */
#define cqss_sched_yield() sched_yield();

/**
 * @brief 主动让出 CPU 时间片（某个线程完成了一项任务，或者某个线程需要等待某个条件时使用）
 *
 */
#if defined(__x86_64__) || defined(__i386__)
    #define cqss_cpu_pause() __asm__ volatile ("pause" ::: "memory")
#elif defined(__aarch64__)
    #define cqss_cpu_pause() __asm__ volatile ("yield" ::: "memory")
#else
    #define cqss_cpu_pause() usleep(1)
#endif

#endif // CQSS_CORE_CQSS_CORE_H_
