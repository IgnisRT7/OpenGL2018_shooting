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
*	オフスクリーンバッファ
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

	TexturePtr tex;			///< フレームバッファ用テクスチャ
	GLuint depthbuffer = 0;	///< 震度バッファオブジェクト
	GLuint framebuffer = 0;	///< フレームバッファオブジェクト


};