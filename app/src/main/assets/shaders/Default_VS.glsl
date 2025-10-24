#version 100
attribute vec4 vPosition;
attribute vec2 a_TexCoord;

varying vec2 v_TexCoord;

uniform mat4 uMVPMatrix;
uniform float offsetX;
uniform float offsetY;
uniform vec2 scale;
uniform float uvScale;

void main() {
    vec4 scaledPos = vPosition;
    scaledPos.xy *= scale;          // model-space scaling
    scaledPos.xy += vec2(offsetX, offsetY); // offset in model space or world space

    gl_Position = uMVPMatrix * scaledPos;

    v_TexCoord = a_TexCoord * uvScale;
}