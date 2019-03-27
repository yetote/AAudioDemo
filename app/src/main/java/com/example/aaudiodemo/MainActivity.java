package com.example.aaudiodemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    AudioPlayer audioPlayer;
    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        audioPlayer = new AudioPlayer();
        String path = this.getExternalCacheDir().getPath() + "/res/test.pcm";
        Log.e(TAG, "onCreate: " + path);
        audioPlayer.createEngine(path);
    }

}
