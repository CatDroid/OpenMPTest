#include <jni.h>
#include <string>
#include <sstream>
#include <omp.h>
#include <fstream>
#include <cmath>
#include <cinttypes>
#include <unistd.h>
#include "trace_log.h"
#include "trace_time.h"

#include <arm_neon.h>


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

    omp_set_dynamic(0);
    omp_set_num_threads(4);

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


    //omp_set_nested(1); // 允许嵌套parallel
    omp_set_dynamic(0);  // 静态模式
    omp_set_num_threads(requested_threads);


    int64_t before = getCurrentThreadRuntimeNs();
    #pragma omp parallel for // num_threads(requested_threads)
    for (int i = 0; i < 40000; i++)
    {
//        if (i == 0) {
//            LOGD("Number of threads = %d" , omp_get_num_threads() );
//            LOGD("Max threads allowed = %d" , omp_get_max_threads() );
//        }
        double x = pow(i, 10);
        LOGD("i = %d threadID = %d level = %d x = %f", i, omp_get_thread_num(),omp_get_active_level() ,x);

    }

    int64_t after = getCurrentThreadRuntimeNs();
    LOGE("cost = %" PRId64, (after-before) );

}


/*
    # BT.601
    # 在老式的非SIMD体系结构中，浮点运算慢与定点运算，所以变换下：
    # RGB 转 YUV studio-swing Y的范围为[16-235] UV的范围为[16-240]
    Y = ((66R+129G+25B+128)>>8)+16
    U = ((-38R-74G+112B+128)>>8)+128
    V = ((112R-94G-18B+128)>>8)+128
    # RGB 转 YUV full-swing YUV的范围都为[0-255]
    Y = (77R+150G+29B+128)>>8
    U = (-43R-84G+127B+128)>>8)+128
    V = ((127R-106G-21B+128)>>8)+128


    ffplay -pixel_format gray -f rawvideo -s 1920x1080 temp.yuv
    ffplay -pixel_format yuv420p -f rawvideo -s 1920x1080 temp.yuv

 */

#define COLOR(x) (uint8_t)((x)>255?255:(x)<0?0:(x))

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_tom_usingopenmp_OpenMPTest_rgba2yuv420p(
        JNIEnv *env, jclass type, jobject _rgba, jobject _yuv,
        jint width, jint height,
        jboolean useOpenMP) {

    uint8_t* rgba = (uint8_t*)env->GetDirectBufferAddress(_rgba);
    uint8_t* yuv =  (uint8_t*)env->GetDirectBufferAddress(_yuv);

    if(useOpenMP){

        // 并行加速比 0.07/0.04  串型0.07s  4 个线程0.02s~0.04  8个线程 0.05~0.04 提升效果不明显

        omp_set_num_threads(8);

        double before = omp_get_wtime();
        uint8_t* y_plane = yuv ;
        uint8_t* u_plane = yuv + width*height ;
        uint8_t* v_plane = yuv + width*height*5/4 ;

        #pragma omp parallel for
        for(int i = 0 ; i < width*height ; i++ ){
            y_plane[i] = COLOR(
                    ( (  66 * rgba[4*i]
                         + 129 * rgba[4*i + 1]
                         +  25 * rgba[4*i + 2] + 128 ) >>8 ) + 16) ;
        }


//        #pragma omp simd
//        for(int i = 0 ; i < width*height ; i++ ){
//            y_plane[i] = COLOR(( (  66 * rgba[4*i]
//                         + 129 * rgba[4*i + 1]
//                         +  25 * rgba[4*i + 2] + 128 ) >>8 ) + 16) ;
//        }

//        {
//            uint8x8_t a = vhsub_u8 (vdup_n_u8(4),vdup_n_u8(6) ); // h = halve
//            uint8_t result[8];
//            vst1_u8(result, a);
//            LOGD("vhsub_u8 =  %x", result[0]); // ff
//
//
//            uint8_t a1 = 4;
//            uint8_t a2 = 6;
//            uint8_t a3 = a1 - a2 ;
//            LOGE("uint8_t = %x ", a3 ); // fe = -2 的补码
//        }

        int uv_height = height/2 ;
        int uv_width = width/2 ;

        #pragma omp parallel for
        for(int i = 0 ; i < uv_height ; i++ ){
            for(int j = 0 ; j < uv_width ; j++ ){
                int index = (i*2)*width + (j*2) ;
                uint8_t R =  rgba[ index*4  ];
                uint8_t G =  rgba[ index*4 + 1];
                uint8_t B =  rgba[ index*4 + 2];
                u_plane[i*uv_width + j] =
                        COLOR(((-38 * R - 74*G + 112*B + 128)>>8)+128) ;
                v_plane[i*uv_width + j] =
                        COLOR(((112 * R - 94*G - 18*B + 128)>>8)+128);
            }
        }

        double after = omp_get_wtime();

        LOGD("(use openmp)duration %lf seconds", (after-before));

    }else{



        double before = omp_get_wtime();
        uint8_t* y_plane = yuv ;
        uint8_t* u_plane = yuv + width*height ;
        uint8_t* v_plane = yuv + width*height*5/4 ;

        for(int i = 0 ; i < width*height ; i++ ){
            y_plane[i] = COLOR(
                    ( (  66 * rgba[4*i]
                      + 129 * rgba[4*i + 1]
                      +  25 * rgba[4*i + 2] + 128 ) >>8 ) + 16) ;
        }

        int uv_height = height/2 ;
        int uv_width = width/2 ;

        for(int i = 0 ; i < uv_height ; i++ ){
            for(int j = 0 ; j < uv_width ; j++ ){
                int index = (i*2)*width + (j*2) ;
                uint8_t R =  rgba[ index*4  ];
                uint8_t G =  rgba[ index*4 + 1];
                uint8_t B =  rgba[ index*4 + 2];
                u_plane[i*uv_width + j] =
                        COLOR(((-38 * R - 74*G + 112*B + 128)>>8)+128) ;
                v_plane[i*uv_width + j] =
                        COLOR(((112 * R - 94*G - 18*B + 128)>>8)+128);
            }
        }

        double after = omp_get_wtime();

        LOGD("(not use openmp) duration %lf seconds", (after-before));

    }

    return JNI_TRUE ;
}