/**
*	@file	OffscreenBuffer.h
*	@brief	フレームバッファなどのバックバッファ管理
*	@author	tn-mai(講義資料製作者)
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

	/**
	*	オフスクリーンバッファを作成する
	*
	*	@param w オフスクリーンバッファの幅(ピクセル単位)
	*	@param h オフスクリーンバッファの高さ(ピクセル単位)
	*
	*	@return 作成したオフスクリーンバッファへのポインタ
	*/
	static OffscreenBufferPtr Create(int w, int h, GLenum f = GL_RGBA8);

	/**
	*	フレームバッファの取得
	*
	*	@return	フレームバッファオブジェクト
	*/
	GLuint GetFramebuffer() const { return framebuffer; }

	/**
	*	フレームバッファに使用しているテクスチャの取得
	*
	*	@return テクスチャID
	*/
	GLuint GetTexture() const { return tex->Id(); }

	/**
	*	テクスチャの幅を取得
	*
	*	@return テクスチャの幅
	*/
	GLuint Width() const { return tex->Width(); }

	/**
	*	テクスチャの高さを取得
	*
	*	@return テクスチャの高さ
	*/
	GLuint Height() const { return tex->Height(); }

	/**
	*	サイズ変更処理
	*
	*	@param w	横方向の変更後のサイズ
	*	@param h	縦方向の変更後のサイズ
	*
	*	warning! この関数は現在未実装です
	*/
	void Resize(int w, int h);

private:

	OffscreenBuffer() = default;
	OffscreenBuffer(const OffscreenBuffer&) = delete;
	OffscreenBuffer& operator=(const OffscreenBuffer&) = delete;
	~OffscreenBuffer();

	TexturePtr tex;			///< フレームバッファ用テクスチャ
	GLuint depthbuffer = 0;	///< 震度バッファオブジェクト
	GLuint framebuffer = 0;	///< フレームバッファオブジェクト


};