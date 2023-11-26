#version 410 core

in vec3 v_Colors;
out vec4 color;

void main() {
    color = vec4(v_Colors.r, v_Colors.g, v_Colors.b, 1.0f);
}