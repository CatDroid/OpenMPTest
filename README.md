### 编译

* cmake中只要增加fopenmp，不要加gomp，否则没有作用(原因不知)

  ```
  set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fopenmp")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp" )
  ```

* 线程组和并行区域

  * 也就是两个同时执行的独立的并行区域，会分别不同的线程组执行

  * 一个线程组到达一个工作共享结构时,要么全部线程被占用 要么不被占用 (相当于一个线程组同时只只是一个并行区)

* omp_get_wtime，得到的是elapse time，会受到sleep的影响