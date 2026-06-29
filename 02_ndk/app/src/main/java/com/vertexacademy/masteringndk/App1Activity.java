package com.vertexacademy.masteringndk;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;


public class App1Activity extends AppCompatActivity {
    private static final String TAG = "NDK-Java-TAG";

    @Override protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        Log.d(TAG,"onCreate method call");
    }

    @Override protected void onPause()
    {
        super.onPause();
        Log.d(TAG,"onPause method call");
    }

    @Override protected void onResume()
    {
        super.onResume();
        Log.d(TAG,"onResume method call");
    }
}