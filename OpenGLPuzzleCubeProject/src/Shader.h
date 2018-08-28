/**
*	@file Shader.h
*/
#pragma once

#include <GL/glew.h>
#include <string>
#include <memory>


namespace Shader {

	class Program;
	using ProgramPtr = std::shared_ptr<Program>;	///< �v���O�����I�u�W�F�N�g�̃|�C���^

	GLuint CreateProgramFromFile(const char* vsFilename, const char* fsFilename);

	/**
	*	�V�F�[�_�v���O�����N���X
	*/
	class Program {
	public:

		static ProgramPtr Create(const char* vsFilename, const char* fsFilename);

		bool UniformBlockBinding(const char* blocblockName, GLuint bindingPoint);
		void UseProgram();
		void BindTexture(GLenum unit, GLenum type, GLuint texture);

	private:
		Program() = default;
		~Program();
		Program(const Program&) = delete;
		Program& operator=(const Program&) = delete;

	private:

		GLuint program = 0;			///< �v���O�����I�u�W�F�N�g
		GLint samplerLocation = -1;	///< �T���v���[�̐�
		int samperCount = 0;		///< �T���v���[�̐�
		std::string name;			///< �v���O������

	};

}