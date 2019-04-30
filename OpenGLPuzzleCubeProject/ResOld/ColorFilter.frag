#version 410

layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexCoord;


out vec4 fragColor;

uniform sampler2D colorSampler[2];

//�|�X�g�G�t�F�N�g�f�[�^
layout(std140) uniform PostEffectData{
	
	mat4x4 matColor;	//�F�ϊ��s��
	float luminanceScale;	//�P�x�����W��
	float bloomThreshould;	//�u���[���𔭐�������臒l
} postEffect;

/**
*	ACES �t�B�������g�[���}�b�s���O
*
*	@param rgb ���̓J���[
*
*	@return �g�[���}�b�s���O���ꂽrgb�J���[
*/
vec3 ACESFilimicToneMapping(vec3 rgb){

	float a = 2.51f;
	float b = 0.03f;
	float c = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	
	return (rgb * (a * rgb + b)) /(rgb * (c * rgb + d) + e);
}

void main(){

	vec4 ts;
	ts.xy = vec2(0.25) / vec2(textureSize(colorSampler[1],0));
	ts.zw = -ts.xy;
	fragColor.rgb = texture(colorSampler[0],inTexCoord).rgb;


	vec3 bloom = texture(colorSampler[1],inTexCoord + ts.xy).rgb;
	bloom += texture(colorSampler[1],inTexCoord + ts.zy).rgb;
	bloom += texture(colorSampler[1],inTexCoord + ts.xw).rgb;
	bloom += texture(colorSampler[1],inTexCoord + ts.zw).rgb;
	bloom *= 1.0 / 4.0;

	fragColor.rgb = texture(colorSampler[0],inTexCoord).rgb;
	fragColor.rgb += bloom;
	fragColor.rgb *= postEffect.luminanceScale;
	fragColor.rgb = ACESFilimicToneMapping(fragColor.rgb);
	fragColor.rgb += (postEffect.matColor * vec4(fragColor.rgb,1)).rgb;
	fragColor.a = 1.0f;
	fragColor *= inColor;

//		fragColor.rgb = texture(colorSampler[0],inTexCoord).rgb;
//	fragColor.rgb = bloom;
	
}