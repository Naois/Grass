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

float smootherstep(float x)
{
    return 6*pow(x,5) - 15*pow(x,4) + 10*pow(x,3);
}

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898,78.233))) * 43758.5453);
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
    return vec3(noise * dir, noise).xzy;
}

#define INDICES 13
#define TIERS 6
#define PI 3.14159265

const float spacing = 0.08;
const float gridsize = 400;

void main()
{
    vec2 basepos = vec2(mod(float(gl_InstanceID), float(gridsize)), gl_InstanceID / gridsize) * spacing;
    float angle = PI * rand2(basepos + vec2(0.3,0.6));
    vec2 straight = vec2(cos(angle), sin(angle));
    vec3 right = vec3(-straight.y, 0, straight.x);

    float r1 = rand(basepos);
    float r2 = rand(basepos + 0.5);
    basepos += vec2(r1,r2);

    vec3 w = wind((basepos)/4.0);
    float phase = rand(basepos + 0.2)*2*PI;
    const float flutterfreq = 10;
    float flutter = 2* w.y*w.y * sin(flutterfreq*time+phase);
    
    float d = floor(float(gl_VertexID) / 2.0)/TIERS; // This is the parameter for the distance along the blade
    float width = 1-d;

    float c = 1 + 2*(w.y*w.y + dot(w.xz,straight))*(smoothstep(-0.5,0.5,dot(w.xz,straight))-0.2) + flutter; //should  depend on wind
    const float l = 1.0;
    float k = 0; //should depend on wind
    float r = 1.0/c;
    float a = l*abs(c);
    float h = 0.4;
    vec2 displace = vec2(h*r*(cos(a*d) - 1), h*abs(r)*mix(sin(a*d),d,k));
    // displace *= vec2(0,1);

    vec3 centrepos = (displace.xxy * vec3(straight, 1)).xzy;
    vec3 finalpos = vec3(basepos,0).xzy + centrepos + (0.05*h*width*(mod(float(gl_VertexID), 2.0)-0.5)* right);
    finalpos -= 0.01*(mod(float(gl_VertexID), 2.0)-0.5) * dot(straight, normalize(billboard[2].xz)) * billboard[0].xyz;
    gl_Position = vec4(finalpos, 1.0);
    pos = (camera*gl_Position).xyz;
    worldpos = gl_Position.xyz;
    gl_Position = perspective * camera * gl_Position;
}