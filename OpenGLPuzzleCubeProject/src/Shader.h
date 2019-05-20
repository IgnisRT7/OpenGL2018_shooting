/**
*	@file Shader.h
*/
#pragma once

#include <GL/glew.h>
#include <string>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


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
		void BindShadowTexture(GLenum type, GLuint texture);
		void SetViewIndex(int index);

		void SetVectorParameter(glm::vec3 p, std::string name);

		void SetBoolParameter(bool b, std::string name);

		void SetFloatParameter(float f, std::string name);


	private:
		Program() = default;
		~Program();
		Program(const Program&) = delete;
		Program& operator=(const Program&) = delete;

	private:

		GLuint program = 0;			///< プログラムオブジェクト
		GLint samplerLocation = -1;	///< サンプラーの位置
		int samperCount = 0;		///< サンプラーの数
		GLint viewIndexLocation = 1;///< 始点インデックスの位置
		GLuint depthSamplerLocation = -1;
		std::string name;			///< プログラム名

	};

}