package com.vertexacademy.masteringndk;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;
import android.widget.Button;


public class App1Activity extends AppCompatActivity 
{
    private static final String TAG = "NDKApp-TAG";
    private AudioEngine audioEngine;
    
    @Override protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        audioEngine = new AudioEngine();

        //Log.d(TAG, "onCreate method call");

        /*Button playButton = new Button(this);
        playButton.setText("Jouer le son OpenAL (440Hz)");
        setContentView(playButton);

        // Initialisation du AudioEngine
        audioEngine = new AudioEngine();
        boolean success = audioEngine.initOpenAL();

        if(!success){
            Toast.makeText(this, "echec de l'initialisation de l'audio", Toast.LENGTH_LONG).show();
        }

        // Evenement au clis sur le bouton
        playButton.setOnClickListener(v -> {
            audioEngine.playTestSound();
        });*/
        setContentView(R.layout.main);
        audioEngine.renderFont(getAssets());

    }

    @Override protected void onDestroy()
    {
        super.onDestroy();
        //Log.d(TAG, "onDestroy method call");
        /*if (audioEngine != null){
            audioEngine.shutdownOpenAL();
        }*/
    }

}