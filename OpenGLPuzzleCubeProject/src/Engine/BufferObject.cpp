/**
*	@file BufferObject.cpp
*/
#include "BufferObject.h"
#include <iostream>

/**
*	�o�b�t�@�I�u�W�F�N�g���쐬����
*
*	@param target	�o�b�t�@�I�u�W�F�N�g�̎��
*	@param size		���_�f�[�^�̃T�C�Y
*	@param data		���_�f�[�^�ւ̃|�C���^
*	@param usage	�o�b�t�@�I�u�W�F�N�g�̃A�N�Z�X�^�C�v
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
*	Buffer Object��j������
*/
void BufferObject::Destroy() {

	if (id) {
		glDeleteBuffers(1, &id);
		id = 0;
	}
}

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
bool BufferObject::BufferSubData(GLintptr offset, GLsizeiptr size, const GLvoid* data) {

	if (offset + size > this->size) {
		std::cerr << "[Warning]: BufferObject::" << __func__ << " �]����̈悪�o�b�t�@�T�C�Y�𒴂��Ă��܂�\n";
		std::cerr << "buffer_size:" << this->size << " offset:" << offset << "size:" << size;
		if (offset >= this->size) {
			return false;
		}
		//�\�Ȕ͈͂����]�����s��
		size = this->size - offset;
	}

	glBindBuffer(target, id);

	glBufferSubData(target, offset, size, data);
	glBindBuffer(target, 0);
	const GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "[Error] BufferObject::" << __func__ << " �f�[�^�̓]���Ɏ��s\n";
		
	}

	return error == GL_NO_ERROR;
}

/**
*	VAO���쐬����
*
*	@param vbo	���_�o�b�t�@�I�u�W�F�N�g��ID
*	@param ibo	�C���f�b�N�X�o�b�t�@�I�u�W�F�N�g��ID
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
*	VAO��j������
*/
void VertexArrayObject::Destroy() {

	if (id) {
		glDeleteVertexArrays(1, &id);
		id = 0;
	}
}

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
void VertexArrayObject::VertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, size_t offset) {

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, reinterpret_cast<GLvoid*>(offset));
}

/**
*	VAO�������Ώۂɂ���
*/
void VertexArrayObject::Bind() const { glBindVertexArray(id); }

/**
*	VAO�������Ώۂ���O��
*/
void VertexArrayObject::UnBind() const { glBindVertexArray(0); }


