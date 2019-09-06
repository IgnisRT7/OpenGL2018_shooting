/**
*	@file BufferObject.h
*	@brief	���_�E�C���f�b�N�X�Ȃǂ̃o�b�t�@�Ǘ�����p
*	@author tn-mai(�u�`���������)
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
	*	�o�b�t�@�I�u�W�F�N�g���쐬����
	*
	*	@param target	�o�b�t�@�I�u�W�F�N�g�̎��
	*	@param size		���_�f�[�^�̃T�C�Y
	*	@param data		���_�f�[�^�ւ̃|�C���^
	*	@param usage	�o�b�t�@�I�u�W�F�N�g�̃A�N�Z�X�^�C�v
	*/
	bool Init(GLenum target, GLsizeiptr size, const GLvoid* data = nullptr, GLenum usage = GL_STATIC_DRAW);
	
	/**
	*	Buffer Object��j������
	*/
	void Destroy();

	/**
	*	�o�b�t�@�Ƀf�[�^��]������
	*
	*	@param offset	�]���J�n�ʒu(�o�C�g�P��)
	*	@param size		�]������o�C�g��
	*	@param data		�]������f�[�^�ւ̃|�C���^
	*
	*	@retval true	�]������
	*	@retval false	�]�����s
	*/
	bool BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data);

	/**
	*	�o�b�t�@�I�u�W�F�N�g�̎擾
	*
	*	@return �o�b�t�@�I�u�W�F�N�g
	*/
	GLuint Id() const { return id; }

	/**
	*	�o�b�t�@�̃T�C�Y�̎擾
	*
	*	@return �o�b�t�@�T�C�Y
	*/
	size_t Size() const { return size; }

private:

	GLuint id = 0;
	GLsizeiptr size = 0;
	GLenum target = 0;
};

/**
*	@class VertexArrayObject 
*	@brief vao�Ǘ��p�N���X
*/
class VertexArrayObject {
public:

	VertexArrayObject() = default;
	~VertexArrayObject() { Destroy(); }
	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	/**
	*	VAO���쐬����
	*
	*	@param vbo	���_�o�b�t�@�I�u�W�F�N�g��ID
	*	@param ibo	�C���f�b�N�X�o�b�t�@�I�u�W�F�N�g��ID
	*/
	bool Init(GLuint vbo, GLuint ibo);

	/**
	*	VAO��j������
	*/
	void Destroy();

	/**
	*	���_�A�g���r���[�g�̃C���f�b�N�X
	*
	*	@param index		���_�A�g���r���[�g�̃C���f�b�N�X
	*	@param size			���_�A�g���r���[�g�̗v�f��
	*	@param type			���_�A�g���r���[�g�̌^
	*	@param normlized	GL_TRUE-�v�f�𐳋K������,GL_FALSE=���K�����Ȃ�
	*	@param stride		���̒��_�f�[�^�܂ł̃o�C�g��
	*	@param offset		���_�f�[�^�擪����̃o�C�g�I�t�Z�b�g
	*/
	void VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset);

	/**
	*	VAO�������Ώۂɂ���
	*/
	void Bind() const;

	/**
	*	VAO�������Ώۂ���O��
	*/
	void UnBind() const;

	GLuint Id() const { return id; }

private:

	GLuint id = 0;
};


