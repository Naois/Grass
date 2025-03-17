#version 330 core

// This doesn't work. It should work, but for some reason, it's being converted to float by opengl.
// I'll chase it down some other time when I actually need an int vertex attribute
layout(location = 0) in int vertindex;

out vec3 pos;
out vec3 worldpos;

uniform mat4 perspective;
uniform mat4 camera;
uniform mat4 billboard;
uniform float time;

#define gridsize 400
#define spacing 0.1

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898,78.233))) * 43758.5453);
}

float smootherstep(float x)
{
    return 6*pow(x,5) - 15*pow(x,4) + 10*pow(x,3);
}

float rand2(vec2 co){
    return fract(sin(dot(co, vec2(58.12315,25.63636))) * 43758.5453)*2-1;
}

float perlin(vec2 loc)
{
    vec2 base = floor(loc);
    vec2 f = fract(loc);
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

vec3 wind(vec2 loc)
{
    const vec2 dir = vec2(1, 2);
    const vec2 dir2 = vec2(0.5, -1);
    float noise = perlin(loc + dir*0.5*time);
    float noiseb = perlin(-loc + dir2*0.5*time);
    return vec3(noise * 3 * dir + noiseb * 1 * dir2, noise * 3 + noiseb).xzy;
}

void main()
{
    vec3 basepos = vec3(mod(float(gl_InstanceID), float(gridsize)), 0, gl_InstanceID / gridsize) * spacing;
    float r1 = rand(basepos.xz);
    float r2 = rand(basepos.xz + 0.5);
    basepos += vec3(r1, 0, r2) * spacing;
    vec3 w = wind(basepos.xz);
    vec3 offset = (gl_VertexID == 0) ? -billboard[0].xyz * spacing: (gl_VertexID == 1) ? billboard[0].xyz * spacing: (vec3(0,5,0)+w) * 0.1;
    gl_Position = perspective * camera * vec4(basepos+offset,1);
    pos = (camera * vec4(basepos+offset,1)).xyz;
    worldpos = basepos+offset;
}