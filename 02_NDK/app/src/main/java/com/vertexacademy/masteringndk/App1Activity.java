package com.vertexacademy.masteringndk;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;
import android.widget.Button;
import android.graphics.Typeface;
import android.widget.TextView;


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
        
        // ======== JNI Function Call ==========
        // Initialize and Read Font police through C++ code
        // This doesn't interfer with Java code
        audioEngine.renderFont(getAssets());

        // Now render the view    
        setContentView(R.layout.main); 

        // Get TextView Object define in layout/main.xml
        TextView textView = findViewById(R.id.tv);

        // Get the font file store in assets folder packed in our apk
        Typeface myFont = Typeface.createFromAsset(getAssets(), "Birthday Children.ttf");

        // Apply the font to textview object
        textView.setTypeface(myFont);
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