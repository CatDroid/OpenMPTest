//
// Created by Administrator on 2019/5/7.
//

#ifndef OPENMPTEST_TRACE_TIME_H
#define OPENMPTEST_TRACE_TIME_H

#include <time.h>

static inline int64_t getCurrentThreadRuntimeNs(){
    struct timespec tm;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tm);
    int64_t now = tm.tv_sec * 1000000000LL + tm.tv_nsec ;
    return now;
}

#endif //OPENMPTEST_TRACE_TIME_H
