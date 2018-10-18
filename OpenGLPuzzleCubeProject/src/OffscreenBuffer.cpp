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
*	@param iformat	
*
*	@return 作成したオフスクリーンバッファへのポインタ
*/
OffscreenBufferPtr OffscreenBuffer::Create(int w, int h, GLenum iformat) {

	struct Impl : OffscreenBuffer {};
	OffscreenBufferPtr offscreen = std::make_shared<Impl>();
	if (!offscreen) {
		return {};
	}

	//入力フォーマットタイプに応じて深度バッファを作成するようにする
	GLenum format = GL_RGBA;
	switch (iformat) {
	default :
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
	if (!offscreen->tex) {
		return{};
	}

	if (format == GL_DEPTH_COMPONENT) {
		//シャドウマップ用の処理

		//	オフスクリーンテクスチャの割り当て
		//tips 色情報の描画は行わないので深度情報のみの描画となる
		glGenRenderbuffers(1, &offscreen->framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, offscreen->framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, offscreen->tex->Id(),0);
		glDrawBuffer(GL_NONE);
	}
	else {
		//通常の描画用の処理

		//深度バッファの作成と割り当て
		glGenRenderbuffers(1, &offscreen->depthbuffer);						
		glBindRenderbuffer(GL_RENDERBUFFER, offscreen->depthbuffer);			
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);	//私は深度バッファです
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		//フレームバッファの作成と割り当て
		glGenFramebuffers(1, &offscreen->framebuffer);						
		glBindFramebuffer(GL_FRAMEBUFFER, offscreen->framebuffer);			
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, offscreen->depthbuffer); //私はフレームバッファでdepthbufferを深度バッファとして扱います
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offscreen->tex->Id(), 0);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "ERROR: OffscreenBuffer の作成に失敗" << std::endl;
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