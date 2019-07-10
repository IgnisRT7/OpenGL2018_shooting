#version 410

layout(location=0) in vec2 inTexCoord;

out vec4 fragColor;

uniform sampler2D texSampler;

void main(){

	fragColor = texture(texSampler,inTexCoord);
	//fragColor =vec4(1);
}



