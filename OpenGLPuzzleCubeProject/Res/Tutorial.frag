

#version 410

layout(location=0) in vec4 inColor;
layout(location=1) in vec2 inTexCoord;
layout(location=2) in vec3 inWorldPosition;
layout(location=3) in mat3 inTBN;

out vec4 fragColor;

//���C�g�f�[�^(�_����)
struct PointLight{
	vec4 position;	//���W(���[���h���W�n)
	vec4 color;		//���邳
};

const int maxLightCount = 4;	//���C�g�̐�

//���C�e�B���O�p�����[�^
layout(std140) uniform LightData{
	
	vec4 eyePos[4];
	vec4 ambientColor;	//����
	PointLight light[maxLightCount];	//���C�g�̃��X�g
}lightData;

uniform int viewIndex;
uniform sampler2D colorSampler[2];

const float shininess =2;
const float normFactor = (shininess + 2) * (1.0 / (2.0 * 3.1415926));

void main() {

	vec3 normal = texture(colorSampler[1], inTexCoord).xyz * 2.0 - 1.0;
	normal = inTBN * normal;

	vec3 lightColor = lightData.ambientColor.rgb;
	vec3 specularColor = vec3(0);
	for(int i = 0; i < maxLightCount; ++i){

		vec3 lightVector = lightData.light[i].position.xyz - inWorldPosition;
		float lightPower = 1.0/dot(lightVector,lightVector);
		vec3 normalizedLightVector = normalize(lightVector);
		float cosTheta = clamp(dot(normal ,normalize(lightVector)), 0,1);
		lightColor += lightData.light[i].color.rgb * cosTheta*lightPower;

		vec3 eyeVector = normalize(lightData.eyePos[viewIndex].xyz - lightData.light[i].position.xyz);
		specularColor += lightData.light[i].color.rgb * pow(max(dot(eyeVector, reflect(normalizedLightVector, normal)), 0), shininess) * lightPower * 0.25;
	}

  fragColor = inColor * texture(colorSampler[0], inTexCoord);
  fragColor.rgb *= lightColor;
  fragColor.rgb += specularColor * normFactor;
}