#version 330 core

in vec2 fragpos;

out vec4 col;

uniform float time;

float smootherstep(float x)
{
    return 6*pow(x,5) - 15*pow(x,4) + 10*pow(x,3);
}

float rand2(vec2 co){
    return fract(sin(dot(co, vec2(58.12315,25.63636))) * 43758.5453)*2 - 1;
}

float perlin(vec2 input)
{
    vec2 base = floor(input);
    vec2 f = fract(input);
    vec2 v00 = vec2(rand2(base), rand2(base + 0.5));
    vec2 v01 = vec2(rand2(base + vec2(0,1)), rand2(base + vec2(0,1) + 0.5));
    vec2 v10 = vec2(rand2(base + vec2(1,0)), rand2(base + vec2(1,0) + 0.5));
    vec2 v11 = vec2(rand2(base + vec2(1,1)), rand2(base + vec2(1,1) + 0.5));
    float f00 = dot(v00, f);
    float f01 = dot(v01, f - vec2(0,1));
    float f10 = dot(v10, f - vec2(1,0));
    float f11 = dot(v11, f - vec2(1,1));
    return mix(mix(f00,f01,smootherstep(f.y)),mix(f10,f11,smootherstep(f.y)),smootherstep(f.x));
}

void main()
{
    col = vec4(perlin(5*fragpos)*0.5 + 0.5);
}