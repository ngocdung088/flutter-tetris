package tech.soit.flutter_tetris

import android.content.res.AssetManager
import android.os.Bundle
import androidx.annotation.NonNull;
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.plugins.GeneratedPluginRegistrant

class MainActivity : FlutterActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        initJni(assetManager = assets)
    }

    override fun configureFlutterEngine(@NonNull flutterEngine: FlutterEngine) {
        GeneratedPluginRegistrant.registerWith(flutterEngine);
    }


    companion object {
        init {
            System.loadLibrary("low-latency-audio")
        }

        @JvmStatic
        private external fun initJni(assetManager: AssetManager)
    }
}
