/**
*	@file OffscreenBuffer.cpp
*/
#pragma once
#include "OffscreenBuffer.h"
#include <iostream>

/**
*	オフスクリーンバッファを作成する
*
*	@param w オフスクリーンバッファの幅(ピクセル単位)
*	@param h オフスクリーンバッファの高さ(ピクセル単位)
*
*	@return 作成したオフスクリーンバッファへのポインタ
*/
OffscreenBufferPtr OffscreenBuffer::Create(int w, int h, GLenum iformat) {

	struct Impl : OffscreenBuffer {};
	OffscreenBufferPtr offscreen = std::make_shared<Impl>();

	GLenum format = GL_RGBA;
	switch (iformat) {
	default:
		format = GL_RGBA;
		break;

	case GL_DEPTH_COMPONENT16:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT32F:
		format = GL_DEPTH_COMPONENT;
		break;
	}

	offscreen->tex = Texture::Create(w, h, iformat, format, nullptr);

	if (format == GL_DEPTH_COMPONENT) {

		//フレームバッファの作成
		glGenFramebuffers(1, &offscreen->framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, offscreen->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, offscreen->tex->Id(), 0);
		glDrawBuffer(GL_NONE);
	}
	else {

		//深度バッファの作成
		glGenRenderbuffers(1, &offscreen->depthbuffer);						//深度バッファのメモリ確保
		glBindRenderbuffer(GL_RENDERBUFFER, offscreen->depthbuffer);			//深度バッファの紐づけ
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);	//このバッファの用途とサイズを決める
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//フレームバッファの作成
		glGenFramebuffers(1, &offscreen->framebuffer);						//フレームバッファのメモリ確保
		glBindFramebuffer(GL_FRAMEBUFFER, offscreen->framebuffer);			//フレームバッファの紐づけ(読み書き可能)
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, offscreen->depthbuffer);	//深度バッファをフレームバッファに紐づけ	
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offscreen->tex->Id(), 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: OffscreenBufferの作成に失敗" << std::endl;
		offscreen.reset();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return offscreen;
}

/**
*	デストラクタ
*/
OffscreenBuffer::~OffscreenBuffer() {

	if (framebuffer) {
		glDeleteFramebuffers(1, &framebuffer);
	}
	if (depthbuffer) {
		glDeleteRenderbuffers(1, &depthbuffer);
	}
}


///Gバッファ作成テスト

bool GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight) {


	auto p = std::make_shared<GBuffer>();

	//
	glGenFramebuffers(1, &p->m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,p->m_fbo);

	//テクスチャの作成(作成数 = マスキングテクスチャ列挙の数)
	glGenTextures(GBUFFER_NUM_TEXTURES, p->m_textures);
	glGenTextures(1, &p->m_depthTexture);

	//
	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; i++) {

		//テクスチャサイズの設定とフレームバッファへの割り当て
		glBindTexture(GL_TEXTURE_2D, p->m_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, p->m_textures[i], 0);
	}

	glBindTexture(GL_TEXTURE_2D, p->m_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, p->m_depthTexture, 0);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(sizeof(DrawBuffers) / sizeof(GLenum), DrawBuffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: gbuffer::Draw() error" << std::endl;
		return false;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return true;



}