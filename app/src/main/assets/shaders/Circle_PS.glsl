#version 300 es
precision mediump float;

in vec2 vLocalPos;
out vec4 FragColor;

void main() {
    float dist = length(vLocalPos);
    float alpha = smoothstep(1.0, 0.25, dist); // smooth edge
    if (alpha <= 0.0) discard;                // optional discard
    float alpha2 = smoothstep(1.0, 0.6, dist); // smooth edge
    FragColor = mix(vec4(0.755,0.08,0.2,1), vec4(0.798,0.441,0.759,1), alpha) * alpha2;
}