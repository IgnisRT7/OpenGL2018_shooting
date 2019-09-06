/**
*	@file	Texture.h
*	@brief	�e�N�X�`���̐����E�Ǘ�
*	@author	tn-mai(�u�`���������)
*/

#pragma once

#include <GL/glew.h>
#include <memory>

class Texture;
using TexturePtr = std::shared_ptr<Texture>;	///< �e�N�X�`���|�C���^

/**
*	�e�N�X�`���N���X
*/
class Texture {
public:

	/**
	*	2D�e�N�X�`�����쐬����
	*
	*	@param width	�e�N�X�`���̕�(�s�N�Z����)
	*	@param height	�e�N�X�`���̍���(�s�N�Z����)
	*	@param iformat	�e�N�X�`���̃f�[�^�`��
	*	@param format	�A�N�Z�X����v�f
	*	@param data		�e�N�X�`���̃f�[�^�ւ̃|�C���^
	*
	*	@return �쐬�ɐ��������ꍇ�̓e�N�X�`���|�C���^��Ԃ�
	*			���s�����ꍇ��nullptr��Ԃ�
	*/
	static TexturePtr Create(int width, int height, GLenum iformat, GLenum format, const void* data);

	/**
	*	�t�@�C������2D�e�N�X�`����ǂݍ���
	*
	*	@param	filename �t�@�C����
	*
	*	@return �쐬�ɐ��������ꍇ�̓e�N�X�`���|�C���^��Ԃ�
	*			���s�����ꍇ��nullptr��Ԃ�
	*/
	static TexturePtr LoadFromFile(const char*);

	/**
	*	�e�N�X�`���̎擾
	*
	*	@return �e�N�X�`��ID
	*/
	GLuint Id() const { return texId; }

	/**
	*	�e�N�X�`���̉������擾
	*
	*	@return �e�N�X�`���̉���
	*/
	GLsizei Width() const { return width; }

	/**
	*	�e�N�X�`���̏c�����擾
	*
	*	@return �e�N�X�`���̏c��
	*/
	GLsizei Height() const { return height; }

private:

	Texture() = default;
	~Texture();
	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	GLuint texId = 0;
	int width = 0;
	int height = 0;
};
