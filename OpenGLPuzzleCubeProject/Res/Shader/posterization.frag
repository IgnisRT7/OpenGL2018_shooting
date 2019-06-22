#version 410

layout(location=0) in vec2 inTexCoord;

out vec4 fragColor;

uniform sampler2D colorSampler;

void main(){

	fragColor = texture(colorSampler, inTexCoord);

	const float numShades = 30;
	fragColor.rgb = ceil(fragColor.rgb * numShades) * (1 / numShades);

}