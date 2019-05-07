#version 410

layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexCoord;


out vec4 fragColor;

uniform sampler2D colorSampler[3];

//ポストエフェクトデータ
layout(std140) uniform PostEffectData{
	
	mat4x4 matColor;	//色変換行列
	float luminanceScale;	//輝度増減係数
	float bloomThreshould;	//ブルームを発生させる閾値
} postEffect;

/**
*	ACES フィルム風トーンマッピング
*
*	@param rgb 入力カラー
*
*	@return トーンマッピングされたrgbカラー
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
*	指定されたピクセル位置のテクスチャ画像を取得します
*
*	@param vec2 texCoord	テクスチャ座標
*	@param vec2 texOffset	差分
*	
*	@return		テクスチャ座標から差分位置にあるピクセル情報(vec4)
*/
vec4 GetOffsetedTexture(vec2 texCoord, vec2 texOffset){

	vec2 pixelSize = 1 / textureSize(colorSampler[2],0);

	return texture(colorSampler[2],texCoord + pixelSize * texOffset);
}

/**
*	sobelフィルタを適用します
*
*	@param	調べる中心となるテクスチャ座標
*/
float GetSobelValue(vec2 texCoord){

	//sobelフィルター用の行列 x
	mat3 sx = mat3( 
		1.0, 2.0, 1.0, 
		0.0, 0.0, 0.0, 
	   -1.0, -2.0, -1.0 
	);
	//sobelフィルター用の行列 y
	mat3 sy = mat3( 
		1.0, 0.0, -1.0, 
		2.0, 0.0, -2.0, 
		1.0, 0.0, -1.0 
	);

	mat3 matPixel;

	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){

			vec4 c = GetOffsetedTexture(texCoord,vec2(j,i));
			
			matPixel[i][j] = length(c.rgb);
		}
	}

	float gx = dot(sx[0], matPixel[0]) + dot(sx[1], matPixel[1]) + dot(sx[2], matPixel[2]); 
	float gy = dot(sy[0], matPixel[0]) + dot(sy[1], matPixel[1]) + dot(sy[2], matPixel[2]);

	float g = sqrt(pow(gx, 2.0)+pow(gy, 2.0));

	return g;

	
}

bool debugDrawStencil = true;

void main(){

	if(!debugDrawStencil){
		//通常の描画処理


		vec4 ts;
		ts.xy = vec2(0.25) / vec2(textureSize(colorSampler[1],0));
		ts.zw = -ts.xy;
		fragColor.rgb = texture(colorSampler[0],inTexCoord).rgb;

		//ブルームエフェクトによる眩しさの設定
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
	}
	else{
		
		float s = GetSobelValue(inTexCoord);
		if(s <= 0.05){
			s=1;
		}

		vec3 textureColor = texture(colorSampler[0],inTexCoord).rgb;
		vec3 stencilColor = texture(colorSampler[2],inTexCoord).rgb;

		fragColor.rgb = mix(stencilColor,textureColor,s);

	}

//	vec2 centerCoord = (inTexCoord - 0.5) * 2;


//	- texture(colorSampler[2],inTexCoord * 0.8).rgb; 
	//fragColor.rgb =vec3( length( texture(colorSampler[2],).rgb));
	//fragColor.rgb = vec3(GetSobelValue(inTexCoord));
}