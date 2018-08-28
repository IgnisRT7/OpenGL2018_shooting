#version 410

layout(location=0) in vec2 inTexCoord;
layout(location=1) in vec4 inColor;

out vec4 fragColor;
uniform sampler2D colorSampler;
const float smoothing = 1.0 / 16.0;
void main()
{

	//テクスチャの色
	vec4 texColor = texture(colorSampler,inTexCoord);
	
	float distance = texColor.r;
	float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);

	fragColor = vec4(vec3(inColor.rgb * texColor.rgb), 1 - alpha);

   //fragColor = texture(colorSampler,inTexCoord) * vec4 ;
}