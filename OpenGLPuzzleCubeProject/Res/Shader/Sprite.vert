#version 410

layout(location=0) in vec3 inPosition;
layout(location=1) in vec4 inColor;
layout(location=2) in vec2 inTexCoord;

layout(location=0) out vec2 outTexCoord;

uniform mat4 matVP;

void main(){
	
	outTexCoord = inTexCoord;
	gl_Position = matVP * vec4(inPosition,1.0);
}



