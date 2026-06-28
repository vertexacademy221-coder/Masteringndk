package vertexacademy.com.masteringndk;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
public class App1Activity extends Activity {

    private static final String TAG = "NDK-TAG";

    static {
        System.loadLibrary("native-lib");
    }

    @Override protected void onCreate (Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        onCreateNative();
    }

    @Override protected void onResume()
    {
        super.onResume();
        Log.d(TAG, "onResume method call");
    }

    @Override protected void onPause()
    {
        super.onPause();
        Log.d(TAG, "onPause method call");
    }

    // Using JNI Mechanism, we can call C++ function from Java code
    public static native void onCreateNative();
}