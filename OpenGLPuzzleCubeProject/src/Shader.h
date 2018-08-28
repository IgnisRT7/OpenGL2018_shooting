/**
*	@file Shader.h
*/
#pragma once

#include <GL/glew.h>
#include <string>
#include <memory>


namespace Shader {

	class Program;
	using ProgramPtr = std::shared_ptr<Program>;	///< プログラムオブジェクトのポインタ

	GLuint CreateProgramFromFile(const char* vsFilename, const char* fsFilename);

	/**
	*	シェーダプログラムクラス
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

		GLuint program = 0;			///< プログラムオブジェクト
		GLint samplerLocation = -1;	///< サンプラーの数
		int samperCount = 0;		///< サンプラーの数
		std::string name;			///< プログラム名

	};

}