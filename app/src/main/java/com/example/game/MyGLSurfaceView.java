package com.example.game;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class MyGLSurfaceView extends GLSurfaceView implements SensorEventListener  {

    private SensorManager sensorManager;
    private Sensor rotationSensor;
    private Sensor accelerometer;
    private float[] forwardVector = new float[3]; // X, Y, Z
    public MyGLSurfaceView(Context context) {
        super(context);
        setEGLContextClientVersion(3); // Use OpenGL ES 2.0
        setRenderer(new MyGLRenderer(context.getAssets()));

        // Initialize sensors
        sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
        rotationSensor = sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_LINEAR_ACCELERATION);
        sensorManager.registerListener(this, rotationSensor, SensorManager.SENSOR_DELAY_FASTEST);
        sensorManager.registerListener(this, accelerometer, SensorManager.SENSOR_DELAY_FASTEST);


    }

    public float[] getForwardVector() {
        return forwardVector;
    }

    @Override
    public void onSensorChanged(SensorEvent event) {

        if (event.sensor.getType() == Sensor.TYPE_LINEAR_ACCELERATION) {
            float x = event.values[0];
            float y = event.values[1];
            float z = event.values[2];
            NativeRenderer.AddAcceleration(x, y, z);
            return;
        }

        if (event.sensor.getType() != Sensor.TYPE_ROTATION_VECTOR) return;

        float[] rotationMatrix = new float[9];
        SensorManager.getRotationMatrixFromVector(rotationMatrix, event.values);

        // Optional: remap axes depending on device orientation
        float[] remappedMatrix = new float[9];
        SensorManager.remapCoordinateSystem(rotationMatrix,
                SensorManager.AXIS_X,
                SensorManager.AXIS_Z,
                remappedMatrix);

        // Forward vector (-Z axis in OpenGL convention)
        forwardVector[0] = -remappedMatrix[6];
        forwardVector[1] = -remappedMatrix[7];
        forwardVector[2] = -remappedMatrix[8];

        // Optionally push to native
        NativeRenderer.updateOrientation(forwardVector[0], forwardVector[1], forwardVector[2]);
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {}

    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        int action = event.getActionMasked();
        int pointerIndex = event.getActionIndex();
        int pointerId = event.getPointerId(pointerIndex);

        switch (action) {
            case MotionEvent.ACTION_DOWN:
            case MotionEvent.ACTION_POINTER_DOWN: {
                float x = event.getX(pointerIndex);
                float y = event.getY(pointerIndex);
                NativeRenderer.onTouchDown(x, y, pointerId);
                break;
            }

            case MotionEvent.ACTION_MOVE: {
                // For move, all pointers may have changed → loop through all
                for (int i = 0; i < event.getPointerCount(); i++) {
                    int id = event.getPointerId(i);
                    float x = event.getX(i);
                    float y = event.getY(i);
                    NativeRenderer.onTouchMove(x, y, id);
                }
                break;
            }

            case MotionEvent.ACTION_UP:
            case MotionEvent.ACTION_POINTER_UP: {
                float x = event.getX(pointerIndex);
                float y = event.getY(pointerIndex);
                NativeRenderer.onTouchUp(x, y, pointerId);
                break;
            }
        }
        return true;  // Return true to indicate event was handled
    }
}


