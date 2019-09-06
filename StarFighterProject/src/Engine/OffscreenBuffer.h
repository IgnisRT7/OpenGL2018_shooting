/**
*	@file	OffscreenBuffer.h
*	@brief	�t���[���o�b�t�@�Ȃǂ̃o�b�N�o�b�t�@�Ǘ�
*	@author	tn-mai(�u�`���������)
*/
#pragma once
#include <GL/glew.h>
#include <memory>
#include "Texture.h"

class OffscreenBuffer;
using OffscreenBufferPtr = std::shared_ptr<OffscreenBuffer>;

/**
*	�I�t�X�N���[���o�b�t�@
*/
class OffscreenBuffer {
public:

	/**
	*	�I�t�X�N���[���o�b�t�@���쐬����
	*
	*	@param w �I�t�X�N���[���o�b�t�@�̕�(�s�N�Z���P��)
	*	@param h �I�t�X�N���[���o�b�t�@�̍���(�s�N�Z���P��)
	*
	*	@return �쐬�����I�t�X�N���[���o�b�t�@�ւ̃|�C���^
	*/
	static OffscreenBufferPtr Create(int w, int h, GLenum f = GL_RGBA8);

	/**
	*	�t���[���o�b�t�@�̎擾
	*
	*	@return	�t���[���o�b�t�@�I�u�W�F�N�g
	*/
	GLuint GetFramebuffer() const { return framebuffer; }

	/**
	*	�t���[���o�b�t�@�Ɏg�p���Ă���e�N�X�`���̎擾
	*
	*	@return �e�N�X�`��ID
	*/
	GLuint GetTexture() const { return tex->Id(); }

	/**
	*	�e�N�X�`���̕����擾
	*
	*	@return �e�N�X�`���̕�
	*/
	GLuint Width() const { return tex->Width(); }

	/**
	*	�e�N�X�`���̍������擾
	*
	*	@return �e�N�X�`���̍���
	*/
	GLuint Height() const { return tex->Height(); }

	/**
	*	�T�C�Y�ύX����
	*
	*	@param w	�������̕ύX��̃T�C�Y
	*	@param h	�c�����̕ύX��̃T�C�Y
	*
	*	warning! ���̊֐��͌��ݖ������ł�
	*/
	void Resize(int w, int h);

private:

	OffscreenBuffer() = default;
	OffscreenBuffer(const OffscreenBuffer&) = delete;
	OffscreenBuffer& operator=(const OffscreenBuffer&) = delete;
	~OffscreenBuffer();

	TexturePtr tex;			///< �t���[���o�b�t�@�p�e�N�X�`��
	GLuint depthbuffer = 0;	///< �k�x�o�b�t�@�I�u�W�F�N�g
	GLuint framebuffer = 0;	///< �t���[���o�b�t�@�I�u�W�F�N�g


};