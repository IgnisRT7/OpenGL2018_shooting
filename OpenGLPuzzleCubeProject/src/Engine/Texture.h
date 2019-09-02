/**
*	@file	Texture.h
*	@brief	テクスチャの生成・管理
*	@author	tn-mai(講義資料製作者)
*/

#pragma once

#include <GL/glew.h>
#include <memory>

class Texture;
using TexturePtr = std::shared_ptr<Texture>;	///< テクスチャポインタ

/**
*	テクスチャクラス
*/
class Texture {
public:

	/**
	*	2Dテクスチャを作成する
	*
	*	@param width	テクスチャの幅(ピクセル数)
	*	@param height	テクスチャの高さ(ピクセル数)
	*	@param iformat	テクスチャのデータ形式
	*	@param format	アクセスする要素
	*	@param data		テクスチャのデータへのポインタ
	*
	*	@return 作成に成功した場合はテクスチャポインタを返す
	*			失敗した場合はnullptrを返す
	*/
	static TexturePtr Create(int width, int height, GLenum iformat, GLenum format, const void* data);

	/**
	*	ファイルから2Dテクスチャを読み込む
	*
	*	@param	filename ファイル名
	*
	*	@return 作成に成功した場合はテクスチャポインタを返す
	*			失敗した場合はnullptrを返す
	*/
	static TexturePtr LoadFromFile(const char*);

	/**
	*	テクスチャの取得
	*
	*	@return テクスチャID
	*/
	GLuint Id() const { return texId; }

	/**
	*	テクスチャの横幅を取得
	*
	*	@return テクスチャの横幅
	*/
	GLsizei Width() const { return width; }

	/**
	*	テクスチャの縦幅を取得
	*
	*	@return テクスチャの縦幅
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
