/**
*	@file BufferObject.cpp
*/
#include "BufferObject.h"
#include <iostream>

/**
*	バッファオブジェクトを作成する
*
*	@param target	バッファオブジェクトの種類
*	@param size		頂点データのサイズ
*	@param data		頂点データへのポインタ
*	@param usage	バッファオブジェクトのアクセスタイプ
*/
bool BufferObject::Init(GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage) {

	Destroy();
	glGenBuffers(1, &id);
	glBindBuffer(target, id);
	glBufferData(target, size, data, usage);
	glBindBuffer(target, 0);

	this->target = target;
	this->size = size;

	return glGetError() == GL_NO_ERROR;
}

/**
*	Buffer Objectを破棄する
*/
void BufferObject::Destroy() {

	if (id) {
		glDeleteBuffers(1, &id);
		id = 0;
	}
}

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
bool BufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data) {

	if (offset + size > this->size) {
		std::cerr << "[Warning]: BufferObject::" << __func__ << " 転送先領域がバッファサイズを超えています\n";
		std::cerr << "buffer_size:" << this->size << " offset:" << offset << "size:" << size;
		if (offset >= this->size) {
			return false;
		}
		//可能な範囲だけ転送を行う
		size = this->size - offset;
	}

	glBindBuffer(target, id);

	glBufferSubData(target, offset, size, data);
	glBindBuffer(target, 0);
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[Error] BufferObject::" << __func__ << " データの転送に失敗\n";
		
	}

	return error == GL_NO_ERROR;
}

/**
*	VAOを作成する
*
*	@param vbo	頂点バッファオブジェクトのID
*	@param ibo	インデックスバッファオブジェクトのID
*/
bool VertexArrayObject::Init(GLuint vbo, GLuint ibo) {

	Destroy();
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);

	return glGetError() == GL_NO_ERROR;
}

/**
*	VAOを破棄する
*/
void VertexArrayObject::Destroy() {

	if (id) {
		glDeleteVertexArrays(1, &id);
		id = 0;
	}
}

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
void VertexArrayObject::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset) {

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<GLvoid*>(offset));
}

/**
*	VAOを処理対象にする
*/
void VertexArrayObject::Bind() const { glBindVertexArray(id); }

/**
*	VAOを処理対象から外す
*/
void VertexArrayObject::UnBind() const { glBindVertexArray(0); }


