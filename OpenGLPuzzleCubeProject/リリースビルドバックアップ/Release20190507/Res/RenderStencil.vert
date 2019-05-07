#version 410

layout(location=0) in vec3 vPosition;

/**
*	���_�V�F�[�_����
*/
layout(std140) uniform VertexData{
	
	mat4 matMVP[4];
	mat4 matDepthMVP;
	mat3x4 matNormal;
	vec4 color;
}vertexData;

void main(){

	gl_Position = vertexData.matMVP[0] * vec4(vPosition, 1);

}




