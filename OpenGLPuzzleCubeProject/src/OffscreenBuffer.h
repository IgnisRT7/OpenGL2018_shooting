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

	TexturePtr tex;			///< フレームバッファ用テクスチャ
	GLuint depthbuffer = 0;	///< 震度バッファオブジェクト
	GLuint framebuffer = 0;	///< フレームバッファオブジェクト
};

/**
*	GBuffer管理クラス
*/
class GBuffer {
public:

	enum GBUFFER_TEXTURE_TYPE {
		GBUFFER_TEXTURE_TYPE_POSITION,	//座標
		GBUFFER_TEXTURE_TYPE_DIFFUSE,	//基本色
		GBUFFER_TEXTURE_TYPE_NORMAL,	//ワールド空間の法線
		GBUFFER_TEXTURE_TYPE_TEXCOORD,	//
		GBUFFER_TEXTURE_TYPE_SCENEDEPTH,//ビュー空間内の深度値
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
