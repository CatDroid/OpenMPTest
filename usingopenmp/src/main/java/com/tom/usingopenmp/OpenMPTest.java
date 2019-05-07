package com.tom.usingopenmp;

public class OpenMPTest {

    static {
        System.loadLibrary("native-lib");
    }


    public static native void doSomething();

    public static native void howManyCore();

    public static native void moreThread();

}
