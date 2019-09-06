/**
*	@file	Shader.h
*	@brief	シェーダの管理用
*	@author	Takuya Yokoyama , tn-mai(講義資料製作者)
*/
#pragma once

#include <GL/glew.h>
#include <string>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace Shader {

	//プログラム作成時に使用するロケーション名リスト
	struct LocationNameList {
		const char* viewIndexSampler = "viewIndexSampler";
		const char* depthSampler = "depthSampler";
		const char* matVPSampler = "matVP";
	}const locationNameList;

	class Program;
	using ProgramPtr = std::shared_ptr<Program>;	///< プログラムオブジェクトのポインタ

	GLuint CreateProgramFromFile(const char* vsFilename, const char* fsFilename);

	/**
	*	シェーダプログラムクラス
	*/
	class Program {
	public:

		/**
		*	シェーダプログラムを作成する
		*
		*	@param vsCode;
		*	@param fsCode;
		*
		*	@return 作成したプログラムオブジェクト
		*/
		static ProgramPtr Create(const char* vsFilename, const char* fsFilename);

		/**
		*	Uniform ブロックをバインディング・ポイントに割り当てる
		*
		*	@param blockName	割り当てるUniformブロックの名前
		*	@param bindingPoint	割当先のバインディング・ポイント
		*
		*	@retval	true	割り当て成功
		*	@retval false	割り当て失敗
		*/
		bool UniformBlockBinding(const char* blocblockName, GLuint bindingPoint);

		/**
		*	描画用プログラムに設定する
		*/
		void UseProgram();

		/**
		*	テクスチャをテクスチャ・イメージユニットに割り当てる
		*
		*	@param unit		割当先のテクスチャ・イメージ・ユニット番号(GL_TExTURE0～)
		*	@param texure	割り当てるテクスチャオブジェクト
		*	@param type		割り当てるテクスチャの種類(GL_TEXTURE_1D,GLTEXTURE_2D,etc)
		*/
		void BindTexture(GLenum unit, GLuint texture, GLenum type = GL_TEXTURE_2D);

		/**
		*	デプステクスチャをテクスチャ・イメージ・ユニットに割り当てる
		*
		*	@param type		割り当てるテクスチャの種類 (GL_TEXTURE_1D,GL_TEXTURE_2D,etc)
		*	@param texture	割り当てるテクスチャオブジェクト
		*/
		void BindShadowTexture(GLenum type, GLuint texture);

		/**
		*	使用するカメラのインデックスを指定する
		*/
		void SetViewIndex(int index);

		/**
		*	ブール型のパラメータを設定する
		*
		*	@param b	パラメータ
		*	@param name	パラメータ名
		*/
		void SetBoolParameter(bool b, std::string name);

		/**
		*	float型パラメータを設定する
		*
		*	@param name パラメータ名
		*/
		void SetFloatParameter(float f, std::string name);

		/**
		*	ベクトル型のパラメータを設定する
		*
		*	@param p	パラメータ
		*	@param n	パラメータ名
		*/
		void SetVectorParameter(glm::vec3 p, std::string name);

		/**
		*	4x4f ビュー射影変換行列を設定する
		*
		*	@param matVP	設定する変換行列
		*/
		void SetMatViewProjection(glm::mat4&);

	private:
		Program() = default;
		~Program();
		Program(const Program&) = delete;
		Program& operator=(const Program&) = delete;

	private:

		GLuint program = 0;				///< プログラムオブジェクト
		GLint samplerLocation = -1;		///< サンプラーの位置
		int samperCount = 0;			///< サンプラーの数
		GLint viewIndexLocation = 1;	///< 始点インデックスの位置
		GLuint depthSamplerLocation = -1;
		GLuint viewProjMatrixLocation = -1;	///< ビュー射影変換行列の位置
		std::string name;			///< プログラム名

		glm::mat4 matVP;				///< ビュー射影変換用行列
	};

}