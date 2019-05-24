package com.example.aaudiodemo;

import android.util.Log;

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
    private PlayerInterface playerInterface;
    private static final String TAG = "AudioPlayer";

    public void setPlayerInterface(PlayerInterface playerInterface) {
        this.playerInterface = playerInterface;
    }

    static {
        System.loadLibrary("native-lib");
    }

    public AudioPlayer() {

    }

    void onDraw(byte[] dataPos) {
        if (playerInterface != null) {
            playerInterface.onDraw(dataPos);
        } else {
            Log.e(TAG, "onDraw: 接口为null");
        }
    }

    public native void createEngine(String path);
}
