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
		glm::vec3 target;
		glm::vec3 up;
	};

	static GameEngine& Instance();
	bool Init(int w, int h, const char* title);
	void Run();
	void UpdateFunc(const UpdateFuncType& func);
	const UpdateFuncType& UpdateFunc() const;

	bool LoadTextureFromFile(const char* filename);
	bool LoadMeshFromFile(const char* filename);

	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName,const char* texName, Entity::Entity::UpdateFuncType func, const char* shader = nullptr);
	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName,const char* normalName, Entity::Entity::UpdateFuncType func,const char* shader = nullptr);
	void RemoveEntity(Entity::Entity*);
	void Light(int index, const Uniform::PointLight& light);
	const Uniform::PointLight& Light(int index) const;
	void AmbientLight(const glm::vec4& color);
	const glm::vec4& AmbientLight() const;
	void Camera(const CameraData& cam);
	const CameraData& Camera() const;
	std::mt19937& Rand();
	const GamePad& GetGamePad() const;

	bool InitAudio(const char* acfPath, const char* acbPath, const char* awbPath, const char* dspBusName);
	void PlayAudio(int playerId, int cueId);
	void StopAudio(int playerId);

	void CollisionHandler(int gid0, int gid1, Entity::CollisionHandlerType handler);
	const Entity::CollisionHandlerType& CollisionHandler(int gid0, int gid1) const;
	void ClearCollisionHandlerList();

	const TexturePtr& GetTexture(const char* filename) const {
		static const TexturePtr dummy;
		auto itr = textureBuffer.find(filename);
		return itr != textureBuffer.end() ? itr->second : dummy;
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

private:

	GameEngine() = default;
	~GameEngine();
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;
	void Update(double  delta);
	void Render();

private:

	bool isInitalized = false;
	UpdateFuncType updateFunc;

	GLuint vbo = 0;
	GLuint ibo = 0;
	GLuint vao = 0;

	BufferObject pbo[2];
	int pboIndexForWriting = -1;
	float luminanceScale = 1.0f;
	
	UniformBufferPtr uboLight;
	UniformBufferPtr uboPostEffect;
	std::unordered_map<std::string, Shader::ProgramPtr> shaderMap;

	OffscreenBufferPtr offscreen;
	static const int bloomBufferCount = 4;
	OffscreenBufferPtr offBloom[bloomBufferCount];


	std::unordered_map<std::string, TexturePtr> textureBuffer;
	Mesh::BufferPtr meshBuffer;
	Entity::BufferPtr entityBuffer;
	Font::Renderer fontRenderer;

	Uniform::LightData lightData;
	CameraData camera;
	std::mt19937 rand;

	std::unordered_map<std::string, double> userNumbers;

};
