#version 330 core

in vec3 dir;

uniform vec3 lightdir;
uniform vec3 skycolour;

out vec4 colour;

const float sunaspect = 0.01;

float rand2(vec2 co){
    return fract(sin(dot(co, vec2(58.12315,25.63636))) * 43758.5453)*2-1;
}

void main()
{
    vec3 norm = normalize(dir);

    float daylight = smoothstep(-0.4,0.2,dot(lightdir,vec3(0,1,0)));

    float skybrightness = pow(dot(norm, lightdir),2.0)*0.3 + 0.7;
    float sunbrightness = smoothstep(cos(1.5*sunaspect),cos(sunaspect),dot(norm, lightdir));
    float sunhalo = exp(dot(norm, lightdir) - 1);
    colour = vec4((daylight*0.8 + 0.2) * (skybrightness * skycolour + (sunbrightness+sunhalo*0.2)*vec3(1,1,1)),1);

    const float noiseamount = 0.01;
    colour += vec4(rand2(gl_FragCoord.xy),rand2(gl_FragCoord.xy+1),rand2(gl_FragCoord.xy+2),0)*noiseamount;
}