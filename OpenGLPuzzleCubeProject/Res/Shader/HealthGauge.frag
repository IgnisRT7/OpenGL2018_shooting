#version 410

layout(location=0) in vec2 inTexCoord;

out vec4 fragColor;

uniform sampler2D colorSampler[2];
uniform float ratio;

void main(){

	vec4 texColor = texture(colorSampler[0],inTexCoord);

	fragColor=vec4(inTexCoord,0,1);
}