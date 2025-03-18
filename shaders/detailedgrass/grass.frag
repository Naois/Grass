#version 330 core

in vec3 pos;
in vec3 worldpos;
in vec3 normal;
in vec3 bladecol;
in float lenfrac;

out vec4 colour;

uniform vec3 lightdir;
uniform vec3 camerapos;


void main()
{
    
    vec3 spotnormal = normalize(normal);

    float daylight = smoothstep(-0.4,0.2,dot(lightdir,vec3(0,1,0)));

    vec3 basecolour = bladecol;
    vec3 diffuse = clamp(dot(spotnormal, lightdir), 0, 1) * basecolour;
    float specularamount = dot(normalize(worldpos - camerapos), lightdir - 2*spotnormal*dot(lightdir,spotnormal));
    vec3 specular = pow(clamp(specularamount,0,1),8.0)*vec3(1.0,1.0,1.0);
    vec3 ambient = basecolour;


    float kd=0.4*daylight+0.1, ks=0.7*daylight+0.1, ka=0.1 + 0.3*daylight;
    float occlusion = 1*lenfrac+0.1;
    colour = vec4((kd*diffuse + ka*ambient + ks*specular) * occlusion, 1);
}