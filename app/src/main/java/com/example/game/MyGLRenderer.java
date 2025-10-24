package com.example.game;

import android.content.res.AssetManager;
import android.content.res.Resources;
import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;
import javax.microedition.khronos.egl.EGLConfig;

public class MyGLRenderer implements GLSurfaceView.Renderer {

    private final AssetManager assetManager;

    public MyGLRenderer(AssetManager assetManager) {
        this.assetManager = assetManager;
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        int screenWidth = Resources.getSystem().getDisplayMetrics().widthPixels;
        int screenHeight = Resources.getSystem().getDisplayMetrics().heightPixels;

        NativeRenderer.init(assetManager, screenWidth, screenHeight);  // Pass AssetManager into native code
    }
    @Override
    public void onDrawFrame(GL10 gl) {
        NativeRenderer.render(); // Call into C++
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        NativeRenderer.resize(width, height); // Call into C++
    }
}