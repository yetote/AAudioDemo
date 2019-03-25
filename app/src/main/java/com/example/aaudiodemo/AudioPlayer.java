package com.example.aaudiodemo;

/**
 * @author yetote QQ:503779938
 * @name AAudioDemo
 * @class name：com.example.aaudiodemo
 * @class describe
 * @time 2019/3/25 9:52
 * @change
 * @chang time
 * @class describe
 */
public class AudioPlayer {
    static {
        System.loadLibrary("native-lib");
    }

    public AudioPlayer() {

    }

    public native void createEngine();
}
