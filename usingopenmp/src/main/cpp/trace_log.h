//
// Created by Administrator on 2019/5/7.
//

#ifndef OPENMPTEST_TRACE_LOG_H
#define OPENMPTEST_TRACE_LOG_H

#include <android/log.h>
#define LOG_TAG "openmp"

#define LOGD(fmt,args...) \
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "[%s][%d]" fmt "\n", __FUNCTION__, __LINE__, ##args)


#define LOGE(fmt,args...) \
    __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "[%s][%d]" fmt "\n", __FUNCTION__, __LINE__, ##args)

#endif //OPENMPTEST_TRACE_LOG_H
