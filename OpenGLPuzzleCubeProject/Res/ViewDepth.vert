#version 410

layout(location=0) in vec3 vPosition;
layout(location=1) in vec4 vColor;
layout(location=2) in vec2 vTexCoord;

layout(location=0) out vec2 outTexCoord;
layout(location=1) out float viewDepth;

uniform int viewIndex;

/**
*	頂点シェーダ入力
*/
layout(std140) uniform VertexData{

	mat4 matMVP[4];
	mat4 matDepthMVP;
	mat4 matModel;
	mat4 matNormal;
	vec4 color;
	//mat3x4 dummy;
} vertexData;

void main(){

	outTexCoord = vTexCoord;
	vec4 pos = vertexData.matMVP[viewIndex]*vec4(vPosition,1);
	vec3 clipPos = pos.xyz / pos.w;

	gl_Position = pos;

}
