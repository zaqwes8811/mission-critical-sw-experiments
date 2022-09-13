//
// Created by zaqwes on 24.08.2022.
//

#ifndef IN_THE_VICINITY_CC_POSIX_CLOCK_H
#define IN_THE_VICINITY_CC_POSIX_CLOCK_H

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NSEC_PER_SEC (1000000000)
#define NSEC_PER_MSEC (1000000)
#define NSEC_PER_USEC (1000)
#define ERROR (-1)
#define OK (0)
#define TEST_SECONDS (0)
#define TEST_NANOSECONDS (NSEC_PER_MSEC * 10)

//#define MY_CLOCK CLOCK_REALTIME
//#define MY_CLOCK CLOCK_MONOTONIC
#define MY_CLOCK CLOCK_MONOTONIC_RAW
//#define MY_CLOCK CLOCK_REALTIME_COARSE
//#define MY_CLOCK CLOCK_MONOTONIC_COARSE

#ifdef __cplusplus
extern "C" {
#endif
void print_scheduler(void);
#ifdef __cplusplus
}
#endif

#endif  // IN_THE_VICINITY_CC_POSIX_CLOCK_H
