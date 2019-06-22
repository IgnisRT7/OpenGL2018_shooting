

#version 410

layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexCoord;
layout(location=2) in vec3 inWorldPosition;
layout(location=3) in mat3 inTBN;
layout(location=6) in vec3 inDepthCoord;


out vec4 fragColor;

//ライトデータ(点光源)
struct PointLight{
	vec4 position;	//座標(ワールド座標系)
	vec4 color;		//明るさ
};

const int maxLightCount = 4;	//ライトの数

//ライティングパラメータ
layout(std140) uniform LightData{
	vec4 eyePos[4];
	vec4 ambientColor;	//環境光
	PointLight light[maxLightCount];	//ライトのリスト
}lightData;

uniform int viewIndex;					//視点
uniform sampler2D colorSampler[2];		//0:オフスクリーンバッファ 1:
uniform sampler2DShadow depthSampler;			//影のサンプラ

const float shininess =2;
const float normFactor = (shininess + 2) * (1.0 / (2.0 * 3.14152926));

const float softShadowScale = 1.0 / 1600.0;
const vec3 poissonDisk[4] = vec3[](
	
	vec3( -0.94201624, -0.39906216,0) * softShadowScale,
	vec3( 0.94558609, -0.76890725,0) * softShadowScale,
	vec3( -0.094184101, -0.92938870,0) * softShadowScale,
	vec3( 0.34495938, 0.29387760,0) * softShadowScale
);

//影比率を取得する
float ShadowRatio(float bias){

	vec3 coord = inDepthCoord;
	coord.z -= bias;
	float shadow = 0.0;
	for (int i = 0; i < 4; ++i) {
		shadow += texture(depthSampler, coord + poissonDisk[i]);
	}
	return shadow * (1.0 / 4.0) * 0.5 + 0.5;
}

void main() {

  vec3 normal = texture(colorSampler[1], inTexCoord).xyz * 2 - 1;
  normal = inTBN * normal;

  vec3 lightColor = vec3(0);
  vec3 specularColor = vec3(0);

  for (int i = 0; i < maxLightCount; ++i) {

    vec3 lightVector = lightData.light[i].position.xyz - inWorldPosition;
    float lightPower = 1.0 / (dot(lightVector, lightVector) + 0.00001);
    vec3 normalizedLightVector = normalize(lightVector);
    float cosTheta = clamp(dot(normal, normalizedLightVector), 0, 1);
    lightColor += lightData.light[i].color.rgb * cosTheta * lightPower;

    vec3 eyeVector = normalize(lightData.eyePos[viewIndex].xyz - lightData.light[i].position.xyz);
    specularColor += lightData.light[i].color.rgb * 
		pow(max(dot(eyeVector, reflect(normalizedLightVector, normal)), 0), shininess) * lightPower * 0.25;
  }

  fragColor = inColor * texture(colorSampler[0], inTexCoord);

  float cosTheta = clamp(dot(normal,normalize(lightData.light[0].position.xyz - inWorldPosition)), 0, 1);
  float depthBias = 0.005 * tan(acos(cosTheta));
  float shadow = ShadowRatio(depthBias);

  fragColor.rgb *= lightColor * shadow + lightData.ambientColor.rgb;
  fragColor.rgb += specularColor * normFactor * shadow;

  //fragColor.rgb = lightColor;
 // fragColor.rgb = vec3(shadow);
  //fragColor.rgb = normalize(normal) * 0.5 + 0.5;

  //fragColor.rgb = texture(colorSampler[0],inTexCoord).rgb;	//テクスチャマッピングのみの描画
 
  //fragColor.rgb = vec3(texture(depthSampler,inDepthCoord + vec3(0, 0, -depthBias)) * 0.5 + 0.5);	//影のみの描画

  //fragColor.rgb = vec3(lightData.light[0].color);
}