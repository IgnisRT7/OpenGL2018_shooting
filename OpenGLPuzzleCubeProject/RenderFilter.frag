#version 140

uniform sampler2D texSampler;

uniform mat3 kernelA;

uniform mat3 kernelX;
uniform mat3 kernelY;

/**
*	サンプラーからエッジ検出を行います
*
*	@param texCoord	評価するテクスチャの中心座標
*	@param isUseXAndYFilter	X方向とY方向のフィルタを適用するか
*/
float CalcEdgeBySobelFilter(vec2 texCoord){

	float result = 0;

	//texCoordを中心に3x3のテクセル情報の取得
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