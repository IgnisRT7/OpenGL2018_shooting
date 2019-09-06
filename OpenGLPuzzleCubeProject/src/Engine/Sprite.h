/**
*	@file	Sprite.h
*	@brief	2D描画の管理
*	@author	tn-mai(講義資料製作者)
*/
#pragma once

#include "BufferObject.h"
#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>

/**
*	矩形構造体
*/
struct Rect {

	glm::vec2 origin;	///< 左下原点
	glm::vec2 size;		///< 縦横の幅
};

/**
*	スプライトクラス
*/
class Sprite {
public:

	Sprite() = default;

	/**
	*	Spriteコンストラクタ
	*
	*	@param tex	スプライトに使用するテクスチャオブジェクト
	*/
	explicit Sprite(const TexturePtr&);
	virtual ~Sprite() = default;
	Sprite(const Sprite&) = default;
	Sprite& operator=(const Sprite&) = default;

	/**
	*	スプライトの座標設定
	*
	*	@param p	座標値
	*/
	void Position(const glm::vec3& p) { position = p; }

	/**
	*	スプライトの座標取得
	*
	*	@return 座標値
	*/
	const glm::vec3& Position() const { return position; }

	/**
	*	スプライトの回転設定
	*	
	*	@param r	回転値
	*/
	void Rotation(float r) { rotation = r; }

	/**
	*	スプライトの回転情報の取得
	*
	*	@return 回転値
	*/
	float Rotation() const { return rotation; }

	/**
	*	スプライトのスケーリングの設定
	*
	*	@param s	スケーリング値
	*/
	void Scale(const glm::vec2& s) { scale = s; }

	/**
	*	スプライトのスケーリングの取得
	*
	*	@return	スケーリング値
	*/
	const glm::vec2& Scale() const { return scale; }

	/**
	*	スプライトの頂点色の設定
	*
	*	@param c	色情報
	*/
	void Color(const glm::vec4& c) { color = c; }

	/**
	*	スプライトの頂点色の取得
	*
	*	@return 色情報
	*/
	const glm::vec4& Color() const { return color; }

	/**
	*	描画区画の設定
	*
	*	@param r	描画区画
	*/
	void Rectangle(const Rect& r) { rect = r; }

	/**
	*	描画区間の取得
	*
	*	@return 描画区画
	*/
	const Rect& Rectangle() const { return rect; }

	/**
	*	使用するテクスチャを指定する
	*
	*	@param tex	テクスチャオブジェクト
	*/
	void Texture(const TexturePtr& tex);

	/**
	*	使用するテクスチャを取得する
	*
	*	@return テクスチャオブジェクト
	*/
	const TexturePtr Texture() const { return texture; }

	/*
	*	シェーダの設定
	*
	*	@param p シェーダオブジェクト
	*/
	void Program(const Shader::ProgramPtr p) { program = p; }

	/**
	*	シェーダの取得
	*
	*	@return シェーダオブジェクト
	*/
	Shader::ProgramPtr Program() const { return program; }

private:

	glm::vec3 position = glm::vec3(0);
	glm::f32 rotation = 0;
	glm::vec2 scale = glm::vec2(1);
	glm::vec4 color = glm::vec4(1);
	Rect rect = { glm::vec2(0,0),glm::vec2(1,1) };
	TexturePtr texture;
	Shader::ProgramPtr program;	
};

using SpritePtr = std::shared_ptr<Sprite>;

/**
*	HPゲージ表示用スプライトクラス
*/
class HealthGuage :public Sprite {
public:

	HealthGuage() = default;

	/**
	*	HPゲージの初期化処理
	*
	*	@param tex	使用するテクスチャ
	*/
	explicit HealthGuage(const TexturePtr& tex);

	virtual ~HealthGuage() = default;
	HealthGuage(const HealthGuage&) = default;
	HealthGuage& operator=(const HealthGuage&) = default;

	/**
	*	表示比率の設定
	*	@param r	完全表示を1としたときの比率
	*/
	void Ratio(float r);

	/**
	*	表示比率の取得
	*
	*	@return 完全表示を1としたときの比率
	*/
	float Ratio() const { return ratio; }

private:

	float ratio = 1;
};

/**
*	スプライト描画クラス
*/
class SpriteRenderer {
public:

	SpriteRenderer() = default;
	~SpriteRenderer() = default;
	SpriteRenderer(const SpriteRenderer&) = delete;
	SpriteRenderer& operator=(const SpriteRenderer&) = delete;

	/**
	*	スプライト描画クラスを初期化する
	*
	*	@param maxSpriteCount	描画可能な最大スプライト数
	*	@param vsPath			頂点シェーダファイル名
	*	@param fsPath			フラグメントシェーダファイル名
	*
	*	@retval true	初期化成功
	*	@retval false	初期化失敗
	*/
	bool Init(size_t maxSpriteCount,Shader::ProgramPtr program);

	/**
	*	頂点データの作成を開始する
	*/
	void BeginUpdate();

	/**
	*	頂点データを追加する
	*
	*	@retval true	追加成功
	*	@retval false	頂点バッファが満杯で追加できない
	*/
	bool AddVertices(const Sprite&);

	/**
	*	頂点データの作成を終了する
	*/
	void EndUpdate();
	//void Update();

	/**
	*	スプライトを描画する
	*
	*	@param texture		描画に使用するテクスチャ
	*	@param screenSize	画面サイズ
	*/
	void Draw(const glm::vec2&)const;

	/**
	*	スプライト描画データを消去する
	*/
	void Clear();

private:

	BufferObject vbo;
	BufferObject ibo;
	VertexArrayObject vao;
	Shader::ProgramPtr program;

	struct Vertex {
		glm::vec3 position;	///< 座標
		glm::vec4 color;	///< 色
		glm::vec2 texCoord;	///< テクスチャ座標
	};
	std::vector<Vertex> vertices;	//頂点データ配列

	struct Primitive {
		size_t count;	///< 頂点数
		size_t offset;	///< データ先頭からのオフセット
		TexturePtr texture;
		Shader::ProgramPtr program;	///< プリミティブを描画するシェーダ
	};
	std::vector<Primitive> primitives;
};
