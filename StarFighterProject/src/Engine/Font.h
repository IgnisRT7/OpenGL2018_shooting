/**
*	@file Font.h
*	@brief	シーン内で描画するためのフォント
*	@author	takuya Yokoyama , tn-mai(講義資料製作者)
*/
#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "BufferObject.h"
#include "Shader.h"

namespace Font {

	struct Vertex;

	/// フォント情報
	struct FontInfo {
		int id = -1;		///< 文字コード
		glm::u16vec2 uv[2];	///< フォント画像のテクスチャ座標
		glm::vec2 size;		///< フォント画像の表示サイズ
		glm::vec2 offset;	///< 表示位置をずらす距離
		float xadvance = 0;	///< カーソルを進める距離
	};

	/**
	*	ビットマップフォント描画クラス
	*/
	class Renderer {
	public:

		Renderer() = default;
		~Renderer() = default;
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		/**
		*	フォント描画オブジェクトを初期化する
		*
		*	@pararm maxChar	最大描画文字数
		*	@param screen	描画先スクリーンの大きさ
		*
		*	@retval true	初期化成功
		*	@retval false	初期化失敗
		*/
		bool Init(size_t maxChar, const glm::vec2& ss);

		/**
		*	フォントファイルを読み込む
		*
		*	@param filename フォントファイル名
		*
		*	@retval true	読み込み成功
		*	@retval false	読み込み失敗
		*/
		bool LoadFromFile(const char* filename);

		/**
		*	拡縮情報の設定
		*
		*	@param s	拡縮情報
		*/
		void Scale(const glm::vec2& s) { scale = s; }

		/**
		*	拡縮情報の取得
		*
		*	@return 拡縮情報
		*/
		const glm::vec2& Scale() const { return scale; }

		/**
		*	文字色を設定する
		*
		*	@param c 文字色
		*/
		void Color(const glm::vec4& c);

		/**
		*	文字色を取得する
		*
		*	@return 文字色
		*/
		glm::vec4 Color() const;

		/**
		*	文字列を追加する
		*
		*	@param position 表示開始座標
		*	@param str		追加する文字列
		*
		*	@retval true	追加成功
		*	@retval false	追加失敗
		*/
		bool AddString(const glm::vec2& position, const char* str,bool isCenter = false);

		/**
		*	VBOをシステムメモリにマッピングする
		*/
		void MapBuffer();

		/**
		*	VBOのマッピングを解除する
		*/
		void UnmapBuffer();

		/**
		*	フォントを描画する
		*/
		void Draw() const;

	private:

		BufferObject vbo, ibo;			/// 描画に使用されるバッファオブジェクト			
		VertexArrayObject vao;			/// 描画に使用されるバッファオブジェクト

		GLsizei vboCapacity = 0;		/// VBOに格納可能な最大長点数
		std::vector<FontInfo> fontList;	/// フォント位置情報のリスト
		std::string texFilename;		/// フォントテクスチャファイル名
		Shader::ProgramPtr progFont;	/// フォント描画用シェーダプログラム
		glm::vec2 reciprocalScreenSize;	/// 画面サイズの逆数

		glm::vec2 scale = glm::vec2(1, 1);	/// フォントを描画するときの拡大率
		glm::vec2 screenScale;
		glm::u8vec4 color = glm::u8vec4(255, 255, 255, 255);	///< フォントを描画するときの色
		GLsizei vboSize = 0;			/// VBOに格納されている頂点数
		Vertex* pVBO = nullptr;			/// VBOへのポインタ
	};
}

