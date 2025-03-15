#version 330 core

in vec3 pos;
in vec3 worldpos;

out vec4 colour;

uniform vec4 fogcolour;

void main()
{
    colour = vec4(mix(vec3(0,0.5,0),vec3(0,1,0),worldpos.yyy),1);
    colour = mix(fogcolour, colour, exp(-0.05*length(pos)));
}