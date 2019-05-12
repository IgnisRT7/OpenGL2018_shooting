#version 140

uniform sampler2D texSampler;

uniform mat3 kernelA;

uniform mat3 kernelX;
uniform mat3 kernelY;

/**
*	�T���v���[����G�b�W���o���s���܂�
*
*	@param texCoord	�]������e�N�X�`���̒��S���W
*	@param isUseXAndYFilter	X������Y�����̃t�B���^��K�p���邩
*/
float CalcEdgeBySobelFilter(vec2 texCoord){

	float result = 0;

	//texCoord�𒆐S��3x3�̃e�N�Z�����̎擾
	mat3 colorMat;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			vec3 texel = textureOffset(texSampler,texCoord,ivec2(j-1,i-1)).rgb;
			
			result += texel * kernelA[j][i];
		}
	}




	return result;
}



layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexCoord;

layout(location=1) out vec4 fragColor;

void main(){



	
}