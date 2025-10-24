package com.example.game;

import android.content.res.AssetManager;

public class NativeRenderer {
    public static native void init(AssetManager assetManager, int screenWidth, int screenHeight);
    public static native void render();
    public static native void resize(int width, int height);
    public static native void onTouchMove(float x, float y, int pointerId);
    public static native void onTouchDown(float x, float y, int pointerId);
    public static native void onTouchUp(float x, float y, int pointerId);

    public static native void updateOrientation(float v, float v1, float v2);
    public static native void AddAcceleration(float v1, float v2, float v3);
}