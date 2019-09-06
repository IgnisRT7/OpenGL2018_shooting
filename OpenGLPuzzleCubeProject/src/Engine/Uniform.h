/**
*	@file	Uniform.h
*	@brief	�V�F�[�_�Ɏg�p���郆�j�t�@�[���o�b�t�@�u���b�N�̊Ǘ�
*	@author	tn-mai(�u�`���������)
*/

#pragma once
#include <glm/glm.hpp>

namespace Uniform {

	static const int maxViewCount = 4;

	/// ���_�f�[�^�̃p�����[�^�^	
	struct alignas(256) VertexData {
		glm::mat4 matMVP[maxViewCount];
		glm::mat4 matDepthMVP;
		glm::mat4 matModel;
		glm::mat4 matNormal;
		glm::vec4 color;
	};

	/**
	*	���C�g�f�[�^(�_����)
	*/
	struct PointLight {
		glm::vec4 position;	///< ���W(���[���h���W�n)
		glm::vec4 color;	///< ���邳
	};

	const int maxLightCount = 4;

	/**
	*	���C�e�B���O�p�����[�^
	*/
	struct alignas(256) LightData {
		glm::vec4 eyePos[maxViewCount];
		glm::vec4 ambientColor;	///< ����
		PointLight light[maxLightCount];	///< ���C�g�̃��X�g
	};

	/**
	*	�|�X�g�G�t�F�N�g�f�[�^
	*/
	struct alignas(256) PostEffectData {

		glm::mat4x4 matColor;	///< �F�ϊ��s��
		float luminanceScale;	///< �P�x�����W��
		float bloomThreshold;	///< �u���[���𔭐�������臒l

	};
}