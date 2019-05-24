package com.example.aaudiodemo;

import androidx.appcompat.app.AppCompatActivity;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    AudioPlayer audioPlayer;
    private static final String TAG = "MainActivity";
    private GLSurfaceView glSurfaceView;
    private MyRenderer renderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        audioPlayer = new AudioPlayer();
        String path = this.getExternalCacheDir().getPath() + "/res/test.pcm";
        Log.e(TAG, "onCreate: " + path);
        renderer = new MyRenderer(this);
        glSurfaceView = findViewById(R.id.glSurfaceView);
        glSurfaceView.setRenderer(renderer);
        audioPlayer.createEngine(path);


        audioPlayer.setPlayerInterface(new PlayerInterface() {
            @Override
            public void onDraw(byte[] dataPosition) {
                for (int i = 0; i < dataPosition.length; i++) {
                    Log.e(TAG, "onDraw: dataPos," + dataPosition[i]);
                }
            }
        });
    }

}
