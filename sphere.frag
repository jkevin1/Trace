#version 330 core

layout(std140) uniform;

in vec2 mapping;
out vec3 outputColor;

uniform float sphereRadius = 0.5;
uniform vec3 cameraSpherePos = vec3(0.0, 0.0, 0.0);

void Impostor(out vec3 cameraPos, out vec3 cameraNormal)
{
    float lensqr = dot(mapping, mapping);
    if(lensqr > 1.0)
        discard;
    	
    cameraNormal = vec3(mapping, sqrt(1.0 - lensqr));
    cameraPos = (cameraNormal * sphereRadius) + cameraSpherePos;
}

void main()
{
    vec3 cameraPos;
    vec3 cameraNormal;
    
    Impostor(cameraPos, cameraNormal);
    
    outputColor = cameraNormal;
}