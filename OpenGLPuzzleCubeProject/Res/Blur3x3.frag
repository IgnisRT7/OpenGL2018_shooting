#version 410

layout(location=0) in vec2 inTexCoord;

out vec4 fragColor;

uniform sampler2D colorSampler;

void main(){

	vec4 ts;
	ts.xy = vec2(0.25) / vec2(textureSize(colorSampler,0));
	ts.zw = -ts.xy;

	fragColor = Texture(colorSampler,inTexCoord + ts.xy);
	fragColor += Texture(colorSampler,inTexCoord + ts.zy);
	fragColor += Texture(colorSampler,inTexCoord + ts.xw);
	fragColor += Texture(colorSampler,inTexCoord + ts.zw);

	fragColor *= 1.0 / 4.0;
	fragColor = 1.0;

}