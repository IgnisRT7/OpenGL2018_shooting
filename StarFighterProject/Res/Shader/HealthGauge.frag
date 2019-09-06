#version 410

layout(location=0) in vec2 inTexCoord;

out vec4 fragColor;

uniform sampler2D colorSampler;
uniform float ratio = 1;

void main(){

	vec4 texColor = texture(colorSampler,vec2(ratio,inTexCoord.y));
	float inRatio = inTexCoord.x;

	fragColor =vec4(texColor.rgb, ceil(max(0, ratio - inRatio)));
}