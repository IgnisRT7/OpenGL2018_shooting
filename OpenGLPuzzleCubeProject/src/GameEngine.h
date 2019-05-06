/**
*	@file GameEngine.h
*/
#pragma once

#include <GL/glew.h>

#include "Uniform.h"
#include "OffscreenBuffer.h"
#include "BufferObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Entity.h"
#include "Uniform.h"
#include "GamePad.h"
#include "Font.h"
#include <glm/glm.hpp>
#include <functional>
#include <random>

/**
*	ゲームエンジンクラス
*/
class GameEngine {
public:
	typedef std::function<void(double)> UpdateFuncType;	//ゲーム状態を更新する関数の型

														///カメラデータ
	struct CameraData {
		glm::vec3 position;
		glm::vec3 target = { 0, -1, 0 };
		glm::vec3 up = { 0, 0, 1 };
	};

	static GameEngine& Instance();
	bool Init(int w, int h, const char* title);
	void Run();
	void UpdateFunc(const UpdateFuncType& func);
	const UpdateFuncType& UpdateFunc() const;

	bool LoadTextureFromFile(const char* filename);
	bool LoadMeshFromFile(const char* filename);

	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);
	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, const char* normalName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);
	void RemoveEntity(Entity::Entity*);
	void RemoveAllEntity();
	void PushLevel();
	void PopLevel();
	void ClearLevel();

	Entity::Entity* FindEntity(std::string n);

	///影生成パラメータ
	struct ShadowParameter {
		glm::vec3 lightPos;	///< 影を発生させるライトの位置
		glm::vec3 lightDir;	///< 影を発生させるライトの方向
		glm::vec3 lightUp;	///< 影を発生させるライトの上方向
		glm::f32 near;		///< 描画範囲のニア平面
		glm::f32 far;		///< 描画範囲のファー平面
		glm::vec2 range;	///< 描画範囲の幅と高さ
	};
	void Shadow(const ShadowParameter& param) {
		shadowParameter = param;
	}
	const ShadowParameter& Shadow() const { return shadowParameter; }

	void Light(int index, const Uniform::PointLight& light);
	const Uniform::PointLight& Light(int index) const;
	void AmbientLight(const glm::vec4& color);
	const glm::vec4& AmbientLight() const;
	void KeyValue(float k) { keyValue = k; }
	const float KeyValue() const { return keyValue; }

	void Camera(size_t index, const CameraData& cam);
	const CameraData& Camera(size_t index) const;

	void GroupVisibility(int groupId, int index, bool isVisible) {
		entityBuffer->GroupVisibility(groupId, index, isVisible);
	}
	bool GroupVisibility(int groupId, int index) const {
		return entityBuffer->GroupVisibility(groupId,index);
	}

	std::mt19937& Rand();
	const GamePad& GetGamePad() const;

	bool InitAudio(const char* acfPath, const char* acbPath, const char* awbPath, const char* dspBusName);
	void PlayAudio(int playerId, int cueId);
	void StopAudio(int playerId);

	void CollisionHandler(int gid0, int gid1, Entity::CollisionHandlerType handler);
	const Entity::CollisionHandlerType& CollisionHandler(int gid0, int gid1) const;
	void ClearCollisionHandlerList();

	const TexturePtr& GetTexture(const char* filename) const {

		for (const auto& e : textureStack) {
			const auto itr = e.find(filename);
			if (itr != e.end()) {
				return itr->second;
			}
		}

		static const TexturePtr dummy;
		return dummy;
	}
	bool LoadFontFromFile(const char* filename) {
		return fontRenderer.LoadFromFile(filename);
	}
	bool AddString(const glm::vec2& pos, const char* str) {
		return fontRenderer.AddString(pos, str);
	}
	void FontScale(const glm::vec2& scale) { fontRenderer.Scale(scale); }
	void FontColor(const glm::vec4& color) { fontRenderer.Color(color); }

	double& UserVariable(const char* name) { return userNumbers[name]; }

	/*
	template<typename T>
	std::shared_ptr<T> FindEntityData() {
		return entityBuffer->FindEntityData<T>();
	}*/

	Entity::Entity* FindEntity(Entity::FindEntityFunc f);

private:

	GameEngine() = default;
	~GameEngine();
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;
	void Update(double  delta);
	void Render();
	void RenderShadow() const;
	void RenderStencil() const;

private:

	bool isInitalized = false;
	UpdateFuncType updateFunc;

	glm::vec2 windowSize;


	GLuint vbo = 0;
	GLuint ibo = 0;
	GLuint vao = 0;

	BufferObject pbo[2];
	int pboIndexForWriting = -1;
	float luminanceScale = 1.0f;
	float keyValue = 0.18f;

	UniformBufferPtr uboLight;
	UniformBufferPtr uboPostEffect;
	std::unordered_map<std::string, Shader::ProgramPtr> shaderMap;

	//オフスクリーンバッファ群
	OffscreenBufferPtr offscreen;					/// バックバッファ
	static const int bloomBufferCount = 4;			
	OffscreenBufferPtr offBloom[bloomBufferCount];	/// ブルームバッファ
	OffscreenBufferPtr offDepth;					/// 深度バッファ
	ShadowParameter shadowParameter;				

	OffscreenBufferPtr offStencil;					/// ステンシルバッファ

	using TextureLevel = std::unordered_map<std::string, TexturePtr>;
	static const size_t minimalStackSize = 1;
	std::vector<TextureLevel> textureStack;

	Mesh::BufferPtr meshBuffer;
	Entity::BufferPtr entityBuffer;
	Font::Renderer fontRenderer;

	Uniform::LightData lightData;
	CameraData camera[Uniform::maxViewCount];
	std::mt19937 rand;

	std::unordered_map<std::string, double> userNumbers;

};
