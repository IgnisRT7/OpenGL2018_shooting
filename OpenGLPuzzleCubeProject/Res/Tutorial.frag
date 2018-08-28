

#version 410

layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexCoord;
layout(location=2) in vec3 inWorldPosition;
layout(location=3) in vec3 inWorldNormal;

out vec4 fragColor;
uniform sampler2D colorSampler;

//ライトデータ(点光源)
struct PointLight{
	vec4 position;	//座標(ワールド座標系)
	vec4 color;		//明るさ
};

const int maxLightCount = 4;	//ライトの数

//ライティングパラメータ
layout(std140) uniform LightData{
	
	vec4 ambientColor;	//環境光
	PointLight light[maxLightCount];	//ライトのリスト
}lightData;

void main() {

	vec3 lightColor = lightData.ambientColor.rgb;
	for(int i = 0; i < maxLightCount; ++i){

		vec3 lightVector = lightData.light[i].position.xyz - inWorldPosition;
		float lightPower = 1.0/dot(lightVector,lightVector);
		float cosTheta = clamp(dot(inWorldNormal ,normalize(lightVector)), 0,1);
		lightColor += lightData.light[i].color.rgb * cosTheta*lightPower;
	}

  fragColor = inColor * texture(colorSampler, inTexCoord);
  fragColor.rgb *= lightColor;
  //fragColor.rgb = inWorldNormal * 0.5 + 0.5;
}