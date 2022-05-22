#version 460

layout(binding=0) uniform samplerCube SkyBoxTex;

in vec3 Vec;

out vec4 FragColour;



void main(){

vec3 texColor = texture(SkyBoxTex, normalize(Vec)).rgb;

FragColour = vec4(texColor, 1.0f);

}