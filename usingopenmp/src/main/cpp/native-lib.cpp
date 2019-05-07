#include <jni.h>
#include <string>
#include <sstream>
#include <omp.h>
#include <fstream>
#include <cmath>
#include <cinttypes>
#include "trace_log.h"
#include "trace_time.h"


extern "C"
JNIEXPORT void JNICALL
Java_com_tom_usingopenmp_OpenMPTest_doSomething(JNIEnv *env, jclass type) {

    int i;
    int threadID = 0;

    #pragma omp parallel for private(i, threadID)
    for(i = 0 ; i < 16 ; i++ )
    {
        threadID = omp_get_thread_num();
        #pragma omp critical
        {
            LOGD("Thread %d reporting\n", threadID);
        }
    }


}

extern "C"
JNIEXPORT void JNICALL
Java_com_tom_usingopenmp_OpenMPTest_howManyCore(JNIEnv *env, jclass type) {

//    std::stringstream str_stream;
//    #pragma omp parallel
//    str_stream << '1'; //根据不同机型的最大线程数N，此句程序会执行N次
//    LOGE("num of core = %s ",  str_stream.str().c_str());


    int numOfCore = 0 ;
    #pragma omp parallel
    numOfCore ++ ; //根据不同机型的最大线程数N，此句程序会执行N次


    #pragma omp parallel
    {
        int cores = omp_get_num_procs(); // 最大核数
        int tid = omp_get_thread_num();  // 当前线程ID
        int total_threads = omp_get_num_threads(); // 线程总数
        int max_threads = omp_get_max_threads(); // 这个处理器可以获取到的总线程数目

        if (tid == 0) {
            LOGD("%i : You have %d cores Processor.\n", tid, cores);
            LOGD("%i : OpenMP generated %d threads.[max = %d].\n", tid, total_threads, max_threads);
        }
        LOGD("%i : This is print by thread[%i]\n",tid,tid);
    };


    LOGE("numOfCore = %d ",  numOfCore );

}

extern "C"
JNIEXPORT void JNICALL
Java_com_tom_usingopenmp_OpenMPTest_moreThread(JNIEnv *env, jclass type) {

    int requested_threads = 4;
    double sum = 0 ;


    omp_set_nested(1);
    omp_set_dynamic(0);
    omp_set_num_threads(requested_threads);


    int64_t before = getCurrentThreadRuntimeNs();
    #pragma omp parallel for num_threads(requested_threads)
    for (int i = 0; i < 4000; i++)
    {
//        if (i == 0) {
//            LOGD("Number of threads = %d" , omp_get_num_threads() );
//            LOGD("Max threads allowed = %d" , omp_get_max_threads() );
//        }
        double x = pow(i, 10);
//        LOGD("i = %d id = %d x = %f", i, omp_get_thread_num(),x);
        LOGD("i = %d id = %d x = %f", i, -1, x);
    }

    int64_t after = getCurrentThreadRuntimeNs();
    LOGE("cost = %" PRId64, (after-before) );

}