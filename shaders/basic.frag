#version 330 core

in vec3 pos;
in vec3 worldpos;

out vec4 colour;

uniform vec4 fogcolour;

void main()
{
    colour = vec4(vec3(worldpos.y+0.5) * vec3(0,0,0),1);
    colour = mix(fogcolour, colour, exp(0.05*pos.z));
}