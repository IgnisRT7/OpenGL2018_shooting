/**
*	@file Uniform.h
*/
#pragma once
#include <glm/glm.hpp>

namespace Uniform {

	/// ���_�f�[�^�̃p�����[�^�^	
	struct VertexData {
		glm::mat4 matMVP;
		glm::mat4 matModel;
		glm::mat4 matNormal;
		glm::vec4 color;
		glm::mat4x3 _dummy;
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
	struct LightData {
		glm::vec4 ambientColor;	///< ����
		PointLight light[maxLightCount];	///< ���C�g�̃��X�g
	};

	/**
	*	�|�X�g�G�t�F�N�g�f�[�^
	*/
	struct PostEffectData {

		glm::mat4x4 matColor;	///< �F�ϊ��s��
		float luminanceScale;	///< �P�x�����W��
		float bloomThreshold;	///< �u���[���𔭐�������臒l

	};
}