/**
*	@file	Shader.h
*	@brief	�V�F�[�_�̊Ǘ��p
*	@author	Takuya Yokoyama , tn-mai(�u�`���������)
*/
#pragma once

#include <GL/glew.h>
#include <string>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Shader {

	//�v���O�����쐬���Ɏg�p���郍�P�[�V���������X�g
	struct LocationNameList {
		const char* viewIndexSampler = "viewIndexSampler";
		const char* depthSampler = "depthSampler";
		const char* matVPSampler = "matVP";
	}const locationNameList;

	class Program;
	using ProgramPtr = std::shared_ptr<Program>;	///< �v���O�����I�u�W�F�N�g�̃|�C���^

	GLuint CreateProgramFromFile(const char* vsFilename, const char* fsFilename);

	/**
	*	�V�F�[�_�v���O�����N���X
	*/
	class Program {
	public:

		/**
		*	�V�F�[�_�v���O�������쐬����
		*
		*	@param vsCode;
		*	@param fsCode;
		*
		*	@return �쐬�����v���O�����I�u�W�F�N�g
		*/
		static ProgramPtr Create(const char* vsFilename, const char* fsFilename);

		/**
		*	Uniform �u���b�N���o�C���f�B���O�E�|�C���g�Ɋ��蓖�Ă�
		*
		*	@param blockName	���蓖�Ă�Uniform�u���b�N�̖��O
		*	@param bindingPoint	������̃o�C���f�B���O�E�|�C���g
		*
		*	@retval	true	���蓖�Đ���
		*	@retval false	���蓖�Ď��s
		*/
		bool UniformBlockBinding(const char* blocblockName, GLuint bindingPoint);

		/**
		*	�`��p�v���O�����ɐݒ肷��
		*/
		void UseProgram();

		/**
		*	�e�N�X�`�����e�N�X�`���E�C���[�W���j�b�g�Ɋ��蓖�Ă�
		*
		*	@param unit		������̃e�N�X�`���E�C���[�W�E���j�b�g�ԍ�(GL_TExTURE0�`)
		*	@param texure	���蓖�Ă�e�N�X�`���I�u�W�F�N�g
		*	@param type		���蓖�Ă�e�N�X�`���̎��(GL_TEXTURE_1D,GLTEXTURE_2D,etc)
		*/
		void BindTexture(GLenum unit, GLuint texture, GLenum type = GL_TEXTURE_2D);

		/**
		*	�f�v�X�e�N�X�`�����e�N�X�`���E�C���[�W�E���j�b�g�Ɋ��蓖�Ă�
		*
		*	@param type		���蓖�Ă�e�N�X�`���̎�� (GL_TEXTURE_1D,GL_TEXTURE_2D,etc)
		*	@param texture	���蓖�Ă�e�N�X�`���I�u�W�F�N�g
		*/
		void BindShadowTexture(GLenum type, GLuint texture);

		/**
		*	�g�p����J�����̃C���f�b�N�X���w�肷��
		*/
		void SetViewIndex(int index);

		/**
		*	�u�[���^�̃p�����[�^��ݒ肷��
		*
		*	@param b	�p�����[�^
		*	@param name	�p�����[�^��
		*/
		void SetBoolParameter(bool b, std::string name);

		/**
		*	float�^�p�����[�^��ݒ肷��
		*
		*	@param name �p�����[�^��
		*/
		void SetFloatParameter(float f, std::string name);

		/**
		*	�x�N�g���^�̃p�����[�^��ݒ肷��
		*
		*	@param p	�p�����[�^
		*	@param n	�p�����[�^��
		*/
		void SetVectorParameter(glm::vec3 p, std::string name);

		/**
		*	4x4f �r���[�ˉe�ϊ��s���ݒ肷��
		*
		*	@param matVP	�ݒ肷��ϊ��s��
		*/
		void SetMatViewProjection(glm::mat4&);

	private:
		Program() = default;
		~Program();
		Program(const Program&) = delete;
		Program& operator=(const Program&) = delete;

	private:

		GLuint program = 0;				///< �v���O�����I�u�W�F�N�g
		GLint samplerLocation = -1;		///< �T���v���[�̈ʒu
		int samperCount = 0;			///< �T���v���[�̐�
		GLint viewIndexLocation = 1;	///< �n�_�C���f�b�N�X�̈ʒu
		GLuint depthSamplerLocation = -1;
		GLuint viewProjMatrixLocation = -1;	///< �r���[�ˉe�ϊ��s��̈ʒu
		std::string name;			///< �v���O������

		glm::mat4 matVP;				///< �r���[�ˉe�ϊ��p�s��
	};

}