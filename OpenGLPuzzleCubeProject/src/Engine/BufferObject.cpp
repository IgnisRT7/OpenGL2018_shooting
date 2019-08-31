/**
*	@file BufferObject.cpp
*/

#include "BufferObject.h"
#include <iostream>


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

void BufferObject::Destroy() {

	if (id) {
		glDeleteBuffers(1, &id);
		id = 0;
	}
}

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

bool VertexArrayObject::Init(GLuint vbo, GLuint ibo) {

	Destroy();
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);

	return glGetError() == GL_NO_ERROR;
}

void VertexArrayObject::Destroy() {

	if (id) {
		glDeleteVertexArrays(1, &id);
		id = 0;
	}
}

void VertexArrayObject::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset) {

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<GLvoid*>(offset));
}

void VertexArrayObject::Bind() const { glBindVertexArray(id); }

void VertexArrayObject::UnBind() const { glBindVertexArray(0); }


