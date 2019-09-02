/**
*	@file Mesh.h
*	@brief	メッシュの管理用
*	@author	tn-mai(講義資料製作者)
*/
#pragma once

#include "BufferObject.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

namespace Mesh {

	class Mesh;
	class Buffer;

	using MeshPtr = std::shared_ptr<Mesh>;	///< メッシュデータポインタ
	using BufferPtr = std::shared_ptr<Buffer>;	///< メッシュバッファポインタ

	/**
	*	マテリアル構造体
	*/
	struct Material {

		GLenum type;	///< インデックスデータ型
		GLsizei size;	///< 描画するインデックス数
		GLvoid* offsest;///< 描画開始インデックスのバイトオフセット
		GLint baseVertex;///< インデックス0とみなされる頂点配列内の位置
		glm::vec4 color; ///< マテリアルの色
	};

	/**
	*	メッシュ
	*	メッシュデータはBufferクラスから作成できるようになっている
	*/
	class Mesh {
		friend class Buffer;
	public:

		/**
		*	メッシュ名の取得
		*
		*	@return メッシュ名
		*/
		const std::string& Name() const { return name; }

		/**
		*	メッシュを描画する
		*
		*	@param buffer	描画に使用するメッシュバッファへのポインタ
		*/
		void Draw(const BufferPtr& buffer) const;

	private:


		Mesh() = default;
		Mesh(const Mesh&) = default;

		/**
		*	コンストラクタ
		*
		*	@param meshName	メッシュデータ名
		*	@param begin	描画するマテリアルの先頭インデックス
		*	@param end		描画するマテリアルの終端インデックス
		*/
		Mesh(const std::string& n, size_t begin, size_t end);

		~Mesh() = default;
		Mesh& operator=(const Mesh&) = default;

	private:

		std::string name;	///< メッシュデータ名
		std::vector<std::string> textureList;	///テクスチャ名のリスト
		size_t beginMaterial = 0;	///< 描画するマテリアルの先頭インデックス
		size_t endMaterial = 0;		///< 描画するマテリアルの終端インデックス
	};

	/**
	*	メッシュバッファ
	*/
	class Buffer {
	public:

		/**
		*	メッシュバッファを作成する
		*
		*	@param vboSize	格納可能な総頂点数
		*	@param iboSize	バッファに格納可能な総インデックス数
		*/
		static BufferPtr Create(int vboSize, int iboSize);

		/**
		*	メッシュをファイルから読み込む
		*
		*	@param filename メッシュファイル名
		*
		*	@retval true	読み込み成功
		*	@retval false	読み込み失敗
		*/
		bool LoadMeshFromFile(const char* filename);

		/**
		*	メッシュを取得する
		*
		*	@param name メッシュ名
		*
		*	@return nameに対応するメッシュへのポインタ
		*/
		const MeshPtr& GetMesh(const char* name) const;

		/**
		*	マテリアルを取得する
		*
		*	@param index マテリアルインデックス
		*
		*	@return indexに対応するマテリアル
		*/
		const Material& GetMaterial(size_t index) const;

		/**
		*	バッファが保持するVAOをOpenGLの処理対象に設定する
		*/
		void BindVAO() const;

		/**
		*	バッファの紐づけの解除
		*/
		void UnBindVAO() const;

		/**
		*	スタックに新しいリソースレベルを作成する
		*/
		void PushLevel();

		/**
		*	スタックの末尾にあるリソースレベルを消去する
		*/
		void PopLevel();

		/**
		*	末尾のリソースレベルを空の状態にする
		*/
		void ClearLevel();
		int Size() { return levelStack.size(); }

	private:

		Buffer() = default;
		~Buffer();
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

	private:

		BufferObject ibo;					///< 頂点データ
		BufferObject vbo;
		VertexArrayObject vao;				///< インデックスデータ

		std::vector<Material> materialList;	///< マテリアルリスト

		//tips :ココから下は上記vbo,ibo,materialListのどこに現在読み込むべき
		//		データがあるかの情報を格納するデータ群です

		struct Level {
			GLintptr vboEnd = 0;			///< 読み込み済み頂点データの終端
			GLintptr iboEnd = 0;			///< 読み込み済みインデックスデータ終端
			size_t materialBaseOffset = 0;	///< マテリアルの各開始位置
			std::unordered_map<std::string, MeshPtr> meshList;	///< メッシュリスト
		};
		std::vector <Level>	levelStack;				///< リソーススタック
		static const size_t minimalStackSize = 1;	///< スタックサイズはこれより小さくならない
	};



}
