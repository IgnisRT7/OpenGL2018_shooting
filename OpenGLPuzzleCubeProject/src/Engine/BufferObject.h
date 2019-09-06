/**
*	@file BufferObject.h
*	@brief	頂点・インデックスなどのバッファ管理制御用
*	@author tn-mai(講義資料製作者)
*/
#pragma once
#include <GL/glew.h>

/**
*	VBO,IBO
*/
class BufferObject {
public:

	BufferObject() = default;
	~BufferObject() { Destroy(); }
	BufferObject(const BufferObject&) = delete;
	BufferObject& operator=(const BufferObject&) = delete;

	/**
	*	バッファオブジェクトを作成する
	*
	*	@param target	バッファオブジェクトの種類
	*	@param size		頂点データのサイズ
	*	@param data		頂点データへのポインタ
	*	@param usage	バッファオブジェクトのアクセスタイプ
	*/
	bool Init(GLenum target, GLsizeiptr size, const GLvoid* data = nullptr, GLenum usage = GL_STATIC_DRAW);
	
	/**
	*	Buffer Objectを破棄する
	*/
	void Destroy();

	/**
	*	バッファにデータを転送する
	*
	*	@param offset	転送開始位置(バイト単位)
	*	@param size		転送するバイト数
	*	@param data		転送するデータへのポインタ
	*
	*	@retval true	転送成功
	*	@retval false	転送失敗
	*/
	bool BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data);

	/**
	*	バッファオブジェクトの取得
	*
	*	@return バッファオブジェクト
	*/
	GLuint Id() const { return id; }

	/**
	*	バッファのサイズの取得
	*
	*	@return バッファサイズ
	*/
	size_t Size() const { return size; }

private:

	GLuint id = 0;
	GLsizeiptr size = 0;
	GLenum target = 0;
};

/**
*	@class VertexArrayObject 
*	@brief vao管理用クラス
*/
class VertexArrayObject {
public:

	VertexArrayObject() = default;
	~VertexArrayObject() { Destroy(); }
	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	/**
	*	VAOを作成する
	*
	*	@param vbo	頂点バッファオブジェクトのID
	*	@param ibo	インデックスバッファオブジェクトのID
	*/
	bool Init(GLuint vbo, GLuint ibo);

	/**
	*	VAOを破棄する
	*/
	void Destroy();

	/**
	*	頂点アトリビュートのインデックス
	*
	*	@param index		頂点アトリビュートのインデックス
	*	@param size			頂点アトリビュートの要素数
	*	@param type			頂点アトリビュートの型
	*	@param normlized	GL_TRUE-要素を正規化する,GL_FALSE=正規化しない
	*	@param stride		次の頂点データまでのバイト数
	*	@param offset		頂点データ先頭からのバイトオフセット
	*/
	void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset);

	/**
	*	VAOを処理対象にする
	*/
	void Bind() const;

	/**
	*	VAOを処理対象から外す
	*/
	void UnBind() const;

	GLuint Id() const { return id; }

private:

	GLuint id = 0;
};


