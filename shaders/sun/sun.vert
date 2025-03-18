#version 330 core

in vec3 pos;

uniform mat4 screentoworlddir;

out vec3 dir;

void main()
{
    gl_Position = vec4(pos, 1);
    dir = (screentoworlddir*gl_Position).xyz;
}