#version 330 core

in vec3 pos;
in vec3 worldpos;
in vec3 normal;

out vec4 colour;

uniform vec4 fogcolour;
uniform vec3 lightdir;
uniform vec3 camerapos;

void main()
{
    colour = vec4(mix(vec3(0,0.2,0),vec3(0,1,0),2*worldpos.yyy),1);
    colour = mix(fogcolour, colour, exp(-0.005*length(pos)));
    
    vec3 basecolour = vec3(0.2,0.6,0.2);
    vec3 diffuse = dot(normal, lightdir) * basecolour;
    float specularamount = dot(normalize(worldpos - camerapos), lightdir - 2*normal*dot(lightdir,normal));
    vec3 specular = pow(clamp(specularamount,0,1),2.0)*vec3(1,1,1);
    vec3 ambient = basecolour;

    const float kd=0.1, ks=0.0, ka=0.9;
    float occlusion = 1.9*worldpos.y+0.05;
    colour = vec4((kd*diffuse + ks*specular + ka*ambient) * occlusion, 1);
}