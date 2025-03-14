#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;

uniform mat4 view;
out vec2 fraguv;

void main()
{
    mat4 m = view;
    gl_Position = m*vec4(pos, 1);
    fraguv = uv;
}