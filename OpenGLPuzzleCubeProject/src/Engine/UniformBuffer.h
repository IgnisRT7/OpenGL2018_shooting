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
	
	static UniformBufferPtr Create(GLsizeiptr size, GLuint bindingPoint, const char* name);
	bool BufferSubData(const GLvoid* data, GLintptr offset = 0, GLsizeiptr size = 0);
	void BindBufferRange(GLintptr offset, GLsizeiptr size) const; 
	void* MapBuffer() const;                                      
	void UnmapBuffer() const;

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