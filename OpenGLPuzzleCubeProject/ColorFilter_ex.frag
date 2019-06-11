#version 410

layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexCoord;

uniform float scenFadeTimerRate;

out vec4 fragColor;

uniform sampler2D colorSampler[3];

const int outlineThickness = 8;

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
*	�K�E�V�A���t�B���^�ɂ��ڂ������H���s���܂�
*/
vec4 GausianFilter(sampler2D texSampler,vec2 texCoord){

	mat3 kernel = mat3(0);//(
//		0.0625,0.125,0.0625,	//1/16,1/8,1/16
//		0.125,0.25,0.125,		//1/8,1/4,1/18
//		0.0625,0.125,0.0625	//1/16,1/8,1/16
//	);

	vec4 result = vec4(0,0,0,0);

	//texCoord�𒆐S��3x3�̃e�N�Z�����̎擾
	mat3 colorMat;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			vec3 texel = textureOffset(texSampler,texCoord,ivec2(j-1,i-1)).rgb;
			
			result += texel * kernel[j][i];
		}
	}

	return result;
}

/**
*	�m�C�Y����̃G�b�W���o���s���܂�
*
*	@param center	�T���v�����O���钆�S�ʒu
*
*	@return ���o�����G�b�W�l
*/
float GetEdge(sampler2D texSampler,vec2 texCoord){

	float result;
	
	//�m�C�Y�L�̊ȒP�ȃG�b�W���o����
	//vec4 c1 = texture(texSampler,texCoord) - textureOffset(texSampler,texCoord,ivec2(1,0));
	//vec4 c2 = texture(texSampler,texCoord) - textureOffset(texSampler,texCoord,ivec2(0,1));
	//result = abs(length(c1)) + abs(length(c2));
	
	//X�����̃G�b�W���o�p�J�[�l��
	mat3 kernelX = mat3(
		1.0,2.0,1.0,
		0.0,0.0,0.0,
		-1.0,-2.0,-1.0
	);

	//Y�����̃G�b�W���o�p�J�[�l��
	mat3 kernelY = mat3(
		1.0,0.0,-1.0,
		2.0,0.0,-2.0,
		1.0,0.0,-1.0
	);

	//prewitt�p�J�[�l��
	mat3 kernelPrewitt = mat3(
		0.0,-1.0,0.0,
		-1.0,4.0,-1.0,
		0.0,-1.0,0.0
	);

	//texCoord�𒆐S��3x3�̃e�N�Z�����̎擾
	mat3 colorMat;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			colorMat[j][i] = length(textureOffset(texSampler,texCoord,ivec2(j-1,i-1)* outlineThickness).rgb);
		}
	}

	//sobel filter �v�Z����
	float gx = dot(kernelX[0],colorMat[0]) + dot(kernelX[1],colorMat[1]) + dot(kernelX[2],colorMat[2]);
	float gy = dot(kernelY[0],colorMat[0]) + dot(kernelY[1],colorMat[1]) + dot(kernelY[2],colorMat[2]);
	result = sqrt(pow(gx,2.0) + pow(gy,2.0));

	//prewitt �v�Z����
	//float gx = dot(kernelPrewitt[0],colorMat[0]) + dot(kernelPrewitt[1],colorMat[1]) + dot(kernelPrewitt[2],colorMat[2]);
	//result = gx;

	return clamp(result,0,1);
}

void main(){

		vec4 ts;
		ts.xy = vec2(0.25) / vec2(textureSize(colorSampler[1],0));
		ts.zw = -ts.xy;
		fragColor.rgb = texture(colorSampler[0],inTexCoord).rgb;

		//�u���[���G�t�F�N�g�ɂ��ῂ����̐ݒ�
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
		fragColor.a = 1;
		fragColor *= inColor * scenFadeTimerRate;

	//	fragColor = vec4(1);

		//�A�E�g���C�����ʂ��~�b�N�X���������̂������o��
		//fragColor.rgb = mix(fragColor.rgb,texture(colorSampler[2],inTexCoord).rgb,GetEdge(colorSampler[2],inTexCoord));

		//�X�e���V���o�b�t�@�����̂܂܏����o��
		//fragColor.rgb = texture(colorSampler[2],inTexCoord).rgb;

		//�X�e���V���o�b�t�@�ɃA�E�g���C�����o�������̂������o��
		//fragColor.rgb = texture(colorSampler[2],inTexCoord).rgb * GetEdge(colorSampler[2],inTexCoord);

		//fragColor.rgb = texture(colorSampler[0],inTexCoord).rgb;

		//vec3(GetEdge(colorSampler[2],inTexCoord));

//	vec2 centerCoord = (inTexCoord - 0.5) * 2;

//	- texture(colorSampler[2],inTexCoord * 0.8).rgb; 
	//fragColor.rgb =vec3( length( texture(colorSampler[2],).rgb));
	//fragColor.rgb = vec3(GetSobelValue(inTexCoord));
}