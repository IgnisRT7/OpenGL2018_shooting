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

	static OffscreenBufferPtr Create(int w, int h,GLenum f = GL_RGBA8);
	GLuint GetFramebuffer() const { return framebuffer; }
	GLuint GetTexture() const { return tex->Id(); }

private:

	OffscreenBuffer() = default;
	OffscreenBuffer(const OffscreenBuffer&) = delete;
	OffscreenBuffer& operator=(const OffscreenBuffer&) = delete;
	~OffscreenBuffer();

	TexturePtr tex;			///< �t���[���o�b�t�@�p�e�N�X�`��
	GLuint depthbuffer = 0;	///< �k�x�o�b�t�@�I�u�W�F�N�g
	GLuint framebuffer = 0;	///< �t���[���o�b�t�@�I�u�W�F�N�g


};