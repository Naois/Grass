#version 330 core

layout(location = 0) in vec2 pos;

out vec2 fragpos;

void main()
{
    gl_Position = vec4(pos, 0, 1);
    fragpos = pos;
}