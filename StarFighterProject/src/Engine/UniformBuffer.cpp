/**
*	@file UniformBuffer.cpp
*/
#include "UniformBuffer.h"
#include <iostream>

UniformBufferPtr UniformBuffer::Create(GLsizeiptr size, GLuint bindingPoint, const char* name) {

	struct Impl : UniformBuffer { Impl() {} ~Impl() {} };

	//バッファのメモリ確保
	UniformBufferPtr p = std::make_shared<Impl>();
	if (!p) {
		std::cerr << "ERROR: UBO '" << name << "'の作成に失敗" << std::endl;
		return {};
	}

	glGenBuffers(1, &p->ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, p->ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, p->ubo);

	const GLenum result = glGetError();
	if (result != GL_NO_ERROR) {
		std::cerr << "ERROR: UBO '" << name << "'の作成に失敗" << std::endl;
		return {};
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	p->size = size;
	p->bindingPoint = bindingPoint;
	p->name = name;

	return p;
}

UniformBuffer::~UniformBuffer() {

	if (ubo) {
		glDeleteBuffers(1, &ubo);
	}
}

bool UniformBuffer::BufferSubData(const GLvoid* data, GLintptr offset, GLsizeiptr size) {

	if (offset * size > this->size) {

		std::cerr << "ERROR(" << name << "): 転送範囲がバッファサイズを超えています(buffer=" <<
			this->size << "offset" << offset << " size =" << size << ")" << std::endl;
		return false;
	}

	if (offset == 0 && size == 0) {
		size = this->size;
	}
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);

	auto err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "ERROR(" << name << "): バッファの転送に失敗" << std::endl;

	}
	return true;
}

void UniformBuffer::BindBufferRange(GLintptr offset, GLsizeiptr size) const
{
	glBindBufferRange(GL_UNIFORM_BUFFER, bindingPoint, ubo, offset, size);
	auto err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "ERROR(" << __func__ << "): 失敗" << std::endl;

	}
}

void* UniformBuffer::MapBuffer() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	return glMapBufferRange(GL_UNIFORM_BUFFER, 0, size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	auto err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "ERROR(" << __func__ << "): 失敗" << std::endl;

	}
}

void UniformBuffer::UnmapBuffer() const
{
	glUnmapBuffer(GL_UNIFORM_BUFFER);
}


