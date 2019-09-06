/**
*	@file UniformBuffer.h
*/
#pragma once
#include <GL/glew.h>
#include <string>
#include <memory>

class UniformBuffer;
using UniformBufferPtr = std::shared_ptr<UniformBuffer>;

/**
*	UBOクラス
*/
class UniformBuffer {
public:
	
	/**
	*	Uniformバッファを作成する
	*
	*	@param size			バッファのバイトサイズ
	*	@param bindingPoint バッファを割り当てるバインディング・ポイント
	*	@param name			バッファ名(デバッグ用)
	*
	*	@return 作成したUniformバッファへのポインタ
	*/
	static UniformBufferPtr Create(GLsizeiptr size, GLuint bindingPoint, const char* name);

	/**
	*	Uniformバッファにデータを転送する
	*
	*	@param data		転送するデータへのポインタ
	*	@param offset	転送先のバイトオフセット
	*	@param size		転送するバイト数
	*
	*	@retval true	転送成功
	*	@retval false	転送失敗
	*
	*	offsetとsizeの両方が0の場合、バッファサイズ=UBOサイズとして転送される
	*/
	bool BufferSubData(const GLvoid* data, GLintptr offset = 0, GLsizeiptr size = 0);

	/**
	*	指定された範囲をバインディング・ポイントに割り当てる.
	*
	*	@param offset 割り当てる範囲のバイトオフセット.
	*	@param size   割り当てる範囲のバイト数.
	*/
	void BindBufferRange(GLintptr offset, GLsizeiptr size) const; 

	/**
	*	UBOをシステムメモリにマップする.
	*
	*	@return マップしたメモリへのポインタ.
	*/
	void* MapBuffer() const;                                      

	/**
	* バッファの割り当てを解除する.
	*/
	void UnmapBuffer() const;

	/**
	*	ユニファームバッファのサイズを取得
	*
	*	@return サイズ
	*/
	GLsizeiptr Size() const { return size; }

private:

	UniformBuffer() = default;
	~UniformBuffer();
	UniformBuffer(const UniformBuffer&) = delete;
	UniformBuffer& operator=(const UniformBuffer&) = delete;

private:

	GLuint ubo = 0;			///< Uniform Buffer Object
	GLsizeiptr size = 0;	///< UBOのサイズ
	GLuint bindingPoint;	///< UBOの割当先バインディング・ポイント
	std::string name;		///< UBO名
};