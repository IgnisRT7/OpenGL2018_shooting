#version 410

layout(location=0) in vec2 inTexCoord;
layout(location=1) in float viewDepth;

uniform sampler2D colorSampler;

void main(){

	float a = texture(colorSampler, inTexCoord).a;
	if(a <= 0.1){
		discard;
	}
	
}