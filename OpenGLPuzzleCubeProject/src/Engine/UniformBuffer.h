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
*	UBO�N���X
*/
class UniformBuffer {
public:
	
	/**
	*	Uniform�o�b�t�@���쐬����
	*
	*	@param size			�o�b�t�@�̃o�C�g�T�C�Y
	*	@param bindingPoint �o�b�t�@�����蓖�Ă�o�C���f�B���O�E�|�C���g
	*	@param name			�o�b�t�@��(�f�o�b�O�p)
	*
	*	@return �쐬����Uniform�o�b�t�@�ւ̃|�C���^
	*/
	static UniformBufferPtr Create(GLsizeiptr size, GLuint bindingPoint, const char* name);

	/**
	*	Uniform�o�b�t�@�Ƀf�[�^��]������
	*
	*	@param data		�]������f�[�^�ւ̃|�C���^
	*	@param offset	�]����̃o�C�g�I�t�Z�b�g
	*	@param size		�]������o�C�g��
	*
	*	@retval true	�]������
	*	@retval false	�]�����s
	*
	*	offset��size�̗�����0�̏ꍇ�A�o�b�t�@�T�C�Y=UBO�T�C�Y�Ƃ��ē]�������
	*/
	bool BufferSubData(const GLvoid* data, GLintptr offset = 0, GLsizeiptr size = 0);

	/**
	*	�w�肳�ꂽ�͈͂��o�C���f�B���O�E�|�C���g�Ɋ��蓖�Ă�.
	*
	*	@param offset ���蓖�Ă�͈͂̃o�C�g�I�t�Z�b�g.
	*	@param size   ���蓖�Ă�͈͂̃o�C�g��.
	*/
	void BindBufferRange(GLintptr offset, GLsizeiptr size) const; 

	/**
	*	UBO���V�X�e���������Ƀ}�b�v����.
	*
	*	@return �}�b�v�����������ւ̃|�C���^.
	*/
	void* MapBuffer() const;                                      

	/**
	* �o�b�t�@�̊��蓖�Ă���������.
	*/
	void UnmapBuffer() const;

	/**
	*	���j�t�@�[���o�b�t�@�̃T�C�Y���擾
	*
	*	@return �T�C�Y
	*/
	GLsizeiptr Size() const { return size; }

private:

	UniformBuffer() = default;
	~UniformBuffer();
	UniformBuffer(const UniformBuffer&) = delete;
	UniformBuffer& operator=(const UniformBuffer&) = delete;

private:

	GLuint ubo = 0;			///< Uniform Buffer Object
	GLsizeiptr size = 0;	///< UBO�̃T�C�Y
	GLuint bindingPoint;	///< UBO�̊�����o�C���f�B���O�E�|�C���g
	std::string name;		///< UBO��
};