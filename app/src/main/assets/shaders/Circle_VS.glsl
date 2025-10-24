#version 300 es
precision mediump float;

layout(location = 0) in vec2 aPos;        // quad [-1..1]
layout(location = 1) in vec2 aInstance;   // instance position

uniform mat4 uMVPMatrix;

out vec2 vLocalPos;

void main() {
    vec2 pos = aPos * vec2(30, 30) + aInstance;       // move quad to instance
    gl_Position = uMVPMatrix * vec4(pos, 0.0, 1.0);

    // Pass local position for circle computation
    vLocalPos = aPos;                  // use aPos, not aPosition
}