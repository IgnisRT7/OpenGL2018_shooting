#version 410

layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexCoord;


out vec4 fragColor;

uniform sampler2D colorSampler[3];

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

/**
*	�w�肳�ꂽ�s�N�Z���ʒu�̃e�N�X�`���摜���擾���܂�
*
*	@param vec2 texCoord	�e�N�X�`�����W
*	@param vec2 texOffset	����
*	
*	@return		�e�N�X�`�����W���獷���ʒu�ɂ���s�N�Z�����(vec4)
*/
vec4 GetOffsetedTexture(vec2 texCoord, vec2 texOffset){

	vec2 pixelSize = 1 / textureSize(colorSampler[2],0);

	return texture(colorSampler[2],texCoord + pixelSize * texOffset);
}

/**
*	sobel�t�B���^��K�p���܂�
*
*	@param	���ׂ钆�S�ƂȂ�e�N�X�`�����W
*/
float GetSobelValue(vec2 texCoord){

	//float val = length(GetOffsetedTexture(texCoord,vec2(-1,1)).rgb);

	float val1 = 
		length(GetOffsetedTexture(texCoord, vec2(-1,  1)))	 * -1 +
		length(GetOffsetedTexture(texCoord, vec2(-1,  0)))	 * -2 +
		length(GetOffsetedTexture(texCoord, vec2(-1, -1)))	 * -1 +
		length(GetOffsetedTexture(texCoord, vec2( 1,  1)))	 * 1 +
		length(GetOffsetedTexture(texCoord, vec2( 1,  0)))	 * 2 +
		length(GetOffsetedTexture(texCoord, vec2( 1, -1)))	 * 1;

	float val2 =
		length(GetOffsetedTexture(texCoord, vec2(-1,  1)))	 * -1;
		length(GetOffsetedTexture(texCoord, vec2( 0,  1)))	 * -2;
		length(GetOffsetedTexture(texCoord, vec2( 1,  1)))	 * -1;
		length(GetOffsetedTexture(texCoord, vec2(-1, -1)))	 * 1;
		length(GetOffsetedTexture(texCoord, vec2( 0, -1)))	 * 2;
		length(GetOffsetedTexture(texCoord, vec2( 1, -1)))	 * 1;

	return 0;
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

//	vec2 centerCoord = (inTexCoord - 0.5) * 2;

//	fragColor.rgb = texture(colorSampler[2],inTexCoord).rgb ;
//	- texture(colorSampler[2],inTexCoord * 0.8).rgb; 
	//fragColor.rgb =vec3( length( texture(colorSampler[2],).rgb));
	//fragColor.rgb = vec3(GetSobelValue(inTexCoord));
}