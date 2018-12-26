/**
*	@file OffscreenBuffer.h
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

	static OffscreenBufferPtr Create(int w, int h, GLenum f = GL_RGBA8);
	GLuint GetFramebuffer() const { return framebuffer; }
	GLuint GetTexture() const { return tex->Id(); }

	GLuint Width() const { return tex->Width(); }
	GLuint Height() const { return tex->Height(); }

private:

	OffscreenBuffer() = default;
	OffscreenBuffer(const OffscreenBuffer&) = delete;
	OffscreenBuffer& operator=(const OffscreenBuffer&) = delete;
	~OffscreenBuffer();

	TexturePtr tex;			///< �t���[���o�b�t�@�p�e�N�X�`��
	GLuint depthbuffer = 0;	///< �k�x�o�b�t�@�I�u�W�F�N�g
	GLuint framebuffer = 0;	///< �t���[���o�b�t�@�I�u�W�F�N�g
};

/**
*	GBuffer�Ǘ��N���X
*/
class GBuffer {
public:

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,	//���W
		GBUFFER_TEXTURE_TYPE_DIFFUSE,	//��{�F
		GBUFFER_TEXTURE_TYPE_NORMAL,	//���[���h��Ԃ̖@��
		GBUFFER_TEXTURE_TYPE_TEXCOORD,	//
		GBUFFER_TEXTURE_TYPE_SCENEDEPTH,//�r���[��ԓ��̐[�x�l
		GBUFFER_NUM_TEXTURES,
	};



	static bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
	void BindForWrinting();
	void BindForReading();

private:

	GBuffer() = default;
	~GBuffer();
	GBuffer(const Texture&) = delete;
	Texture& operator=(const GBuffer&) = delete;

	GLuint m_fbo;
	GLuint m_textures[GBUFFER_NUM_TEXTURES];
	GLuint m_depthTexture;
};
