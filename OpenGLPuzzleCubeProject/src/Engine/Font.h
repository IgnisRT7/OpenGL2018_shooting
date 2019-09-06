/**
*	@file Font.h
*	@brief	�V�[�����ŕ`�悷�邽�߂̃t�H���g
*	@author	takuya Yokoyama , tn-mai(�u�`���������)
*/
#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "BufferObject.h"
#include "Shader.h"

namespace Font {

	struct Vertex;

	/// �t�H���g���
	struct FontInfo {
		int id = -1;		///< �����R�[�h
		glm::u16vec2 uv[2];	///< �t�H���g�摜�̃e�N�X�`�����W
		glm::vec2 size;		///< �t�H���g�摜�̕\���T�C�Y
		glm::vec2 offset;	///< �\���ʒu�����炷����
		float xadvance = 0;	///< �J�[�\����i�߂鋗��
	};

	/**
	*	�r�b�g�}�b�v�t�H���g�`��N���X
	*/
	class Renderer {
	public:

		Renderer() = default;
		~Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		/**
		*	�t�H���g�`��I�u�W�F�N�g������������
		*
		*	@pararm maxChar	�ő�`�敶����
		*	@param screen	�`���X�N���[���̑傫��
		*
		*	@retval true	����������
		*	@retval false	���������s
		*/
		bool Init(size_t maxChar, const glm::vec2& ss);

		/**
		*	�t�H���g�t�@�C����ǂݍ���
		*
		*	@param filename �t�H���g�t�@�C����
		*
		*	@retval true	�ǂݍ��ݐ���
		*	@retval false	�ǂݍ��ݎ��s
		*/
		bool LoadFromFile(const char* filename);

		/**
		*	�g�k���̐ݒ�
		*
		*	@param s	�g�k���
		*/
		void Scale(const glm::vec2& s) { scale = s; }

		/**
		*	�g�k���̎擾
		*
		*	@return �g�k���
		*/
		const glm::vec2& Scale() const { return scale; }

		/**
		*	�����F��ݒ肷��
		*
		*	@param c �����F
		*/
		void Color(const glm::vec4& c);

		/**
		*	�����F���擾����
		*
		*	@return �����F
		*/
		glm::vec4 Color() const;

		/**
		*	�������ǉ�����
		*
		*	@param position �\���J�n���W
		*	@param str		�ǉ����镶����
		*
		*	@retval true	�ǉ�����
		*	@retval false	�ǉ����s
		*/
		bool AddString(const glm::vec2& position, const char* str,bool isCenter = false);

		/**
		*	VBO���V�X�e���������Ƀ}�b�s���O����
		*/
		void MapBuffer();

		/**
		*	VBO�̃}�b�s���O����������
		*/
		void UnmapBuffer();

		/**
		*	�t�H���g��`�悷��
		*/
		void Draw() const;

	private:

		BufferObject vbo, ibo;			/// �`��Ɏg�p�����o�b�t�@�I�u�W�F�N�g			
		VertexArrayObject vao;			/// �`��Ɏg�p�����o�b�t�@�I�u�W�F�N�g

		GLsizei vboCapacity = 0;		/// VBO�Ɋi�[�\�ȍő咷�_��
		std::vector<FontInfo> fontList;	/// �t�H���g�ʒu���̃��X�g
		std::string texFilename;		/// �t�H���g�e�N�X�`���t�@�C����
		Shader::ProgramPtr progFont;	/// �t�H���g�`��p�V�F�[�_�v���O����
		glm::vec2 reciprocalScreenSize;	/// ��ʃT�C�Y�̋t��

		glm::vec2 scale = glm::vec2(1, 1);	/// �t�H���g��`�悷��Ƃ��̊g�嗦
		glm::vec2 screenScale;
		glm::u8vec4 color = glm::u8vec4(255, 255, 255, 255);	///< �t�H���g��`�悷��Ƃ��̐F
		GLsizei vboSize = 0;			/// VBO�Ɋi�[����Ă��钸�_��
		Vertex* pVBO = nullptr;			/// VBO�ւ̃|�C���^
	};
}

