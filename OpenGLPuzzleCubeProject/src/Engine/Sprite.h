/**
*	@file Sprite.h
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
	explicit Sprite(const TexturePtr&);
	virtual ~Sprite() = default;
	Sprite(const Sprite&) = default;
	Sprite& operator=(const Sprite&) = default;

	//座標の設定・取得
	void Position(const glm::vec3& p) { position = p; }
	const glm::vec3& Position() const { return position; }

	//回転の設定・取得
	void Rotation(float r) { rotation = r; }
	float Rotation() const { return rotation; }

	//拡大率の設定・取得
	void Scale(const glm::vec2& s) { scale = s; }
	const glm::vec2& Scale() const { return scale; }

	//色の設定・取得
	void Color(const glm::vec4& c) { color = c; }
	const glm::vec4& Color() const { return color; }

	//矩形の設定・取得
	void Rectangle(const Rect& r) { rect = r; }
	const Rect& Rectangle() const { return rect; }

	//テクスチャの設定・取得
	void Texture(const TexturePtr& tex);
	const TexturePtr Texture() const { return texture; }

	//使用するシェーダの設定
	void Program(const Shader::ProgramPtr p) { program = p; }
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
	explicit HealthGuage(const TexturePtr& tex);
	virtual ~HealthGuage() = default;
	HealthGuage(const HealthGuage&) = default;
	HealthGuage& operator=(const HealthGuage&) = default;

	void Ratio(float r);
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

	bool Init(size_t maxSpriteCount,Shader::ProgramPtr program);
	void BeginUpdate();
	bool AddVertices(const Sprite&);
	void EndUpdate();
	//void Update();
	void Draw(const glm::vec2&)const;
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
