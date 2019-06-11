package com.tom.usingopenmp;

import java.nio.ByteBuffer;

public class OpenMPTest {

    static {
        System.loadLibrary("native-lib");
    }


    public static native void doSomething();

    public static native void howManyCore();

    public static native void moreThread();

    public static native void doTask();


    public static native boolean rgba2yuv420p(
            ByteBuffer rgba, ByteBuffer yuv, int widht, int height , boolean useOpenMP);

}
