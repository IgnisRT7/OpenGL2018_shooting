/**
*	@file OffscreenBuffer.cpp
*/
#pragma once
#include "OffscreenBuffer.h"

/**
*	オフスクリーンバッファを作成する
*
*	@param w オフスクリーンバッファの幅(ピクセル単位)	
*	@param h オフスクリーンバッファの高さ(ピクセル単位)
*
*	@return 作成したオフスクリーンバッファへのポインタ
*/
OffscreenBufferPtr OffscreenBuffer::Create(int w, int h, GLenum f) {

	struct Impl : OffscreenBuffer {};
	OffscreenBufferPtr offscreen = std::make_shared<Impl>();

	offscreen->tex = Texture::Create(w, h, f, GL_RGBA, nullptr);

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

	if (!offscreen) {
		return {};
	}

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