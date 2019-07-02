/**
*	@file BufferObject.h
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

	bool Init(GLenum target, GLsizeiptr size, const GLvoid* data = nullptr, GLenum usage = GL_STATIC_DRAW);
	void Destroy();
	bool BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data);

	GLuint Id() const { return id; }
	size_t Size() const { return size; }

private:

	GLuint id = 0;
	GLsizeiptr size = 0;
	GLenum target = 0;
};

/**
*	VAO
*/
class VertexArrayObject {
public:

	VertexArrayObject() = default;
	~VertexArrayObject() { Destroy(); }
	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	bool Init(GLuint vbo, GLuint ibo);
	void Destroy();
	void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset);
	void Bind() const;
	void UnBind() const;
	GLuint Id() const { return id; }

private:

	GLuint id = 0;
};


