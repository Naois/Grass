#version 330 core

layout(location = 0) in vec3 inpos;

uniform mat4 perspective;
uniform mat4 camera;

out vec3 pos;
out vec3 worldpos;
out vec3 normal;

void main()
{
    gl_Position = perspective * camera * vec4(inpos,1);
    pos = (camera * vec4(inpos,1)).xyz;
    worldpos = inpos;
    normal = vec3(1,0,0);
}