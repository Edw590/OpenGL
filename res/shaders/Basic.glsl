// Vertex shader
#version 110

attribute vec3 a_position;
attribute vec4 a_color;

varying vec4 v_color;

uniform float u_scale;

void main() {
    gl_Position = vec4(a_position * u_scale, 1.0);
    v_color = a_color;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Fragment shader
#version 110

varying vec4 v_color;

void main() {
    gl_FragColor = v_color;
}
