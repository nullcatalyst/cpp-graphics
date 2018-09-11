#version 410 core

// uniform mat4 Matrix;

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Color;

out vec4 color;

void main() {
    gl_Position = /*Matrix * */Position;
    color = Color;
}
