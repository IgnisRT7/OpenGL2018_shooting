/**
*	@file GameEngine.h
*/
#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <random>
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
#include "Sprite.h"

#include "Scene.h"

#define COMPONENT_TYPEPTR(type) std::shared_ptr<type>			///シーンコンポーネントの型用マクロ

#define FILEPASS_SHADER "Res/Shader/"

/**
*	ゲームエンジンクラス
*/
class GameEngine {
	friend class SceneStack;
public:

	static GameEngine& Instance();
	bool Init(int w, int h, const char* title);
	void Run();

	//オーディオ関係のラッパー関数
	bool InitAudio(const char* acfPath, const char* acbPath, const char* awbPath, const char* dspBusName);
	void PlayAudio(int playerId, int cueId);
	void StopAudio(int playerId);
	void StopAllAudio();

	//リソース読み込み
	bool LoadTextureFromFile(const char* filename);
	bool LoadMeshFromFile(const char* filename);
	bool LoadFontFromFile(const char* filename);

	//エンティティ追加処理
	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);
	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, const char* normalName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);

	//スプライト関連
	void AddSprite(Sprite& s);

	//エンティティ削除処理
	void RemoveEntity(Entity::Entity*);
	void RemoveAllEntity();

	//エンティティ検索処理
	template<typename T>
	Entity::Entity* FindEntityData() { return entityBuffer->FindEntityData<T>(); }

	//フォント関連の処理
	bool AddString(const glm::vec2& pos, const char* str, bool isCenter = false) {
		return fontRenderer.AddString(pos, str, isCenter);
	}
	void FontScale(const glm::vec2& scale) { fontRenderer.Scale(scale); }
	void FontColor(const glm::vec4& color) { fontRenderer.Color(color); }

	//シーン追加・削除処理
	void PushScene(ScenePtr s);
	void PopScene();
	void ReplaceScene(ScenePtr s);

	const glm::vec2& WindowSize() const { return windowSize; }

	///影生成パラメータ
	struct ShadowParameter {
		glm::vec3 lightPos;	///< 影を発生させるライトの位置
		glm::vec3 lightDir;	///< 影を発生させるライトの方向
		glm::vec3 lightUp;	///< 影を発生させるライトの上方向
		glm::f32 near;		///< 描画範囲のニア平面
		glm::f32 far;		///< 描画範囲のファー平面
		glm::vec2 range;	///< 描画範囲の幅と高さ
	};

	//シャドウパラメータの取得・設定処理
	void Shadow(const ShadowParameter& param) { shadowParameter = param; }
	const ShadowParameter& Shadow() const { return shadowParameter; }

	//ライトパラメータの取得・設定処理
	void Light(int index, const Uniform::PointLight& light);
	const Uniform::PointLight& Light(int index) const;

	//環境光の取得・設定処理
	void AmbientLight(const glm::vec4& color);
	const glm::vec4& AmbientLight() const;

	//輝度の閾値の取得・設定処理
	void KeyValue(float k) { keyValue = k; }
	const float KeyValue() const { return keyValue; }

	//表示・非表示の処理 現在未使用
	void GroupVisibility(int groupId, int index, bool isVisible) { entityBuffer->GroupVisibility(groupId, index, isVisible); }
	bool GroupVisibility(int groupId, int index) const { return entityBuffer->GroupVisibility(groupId, index); }

	//コリジョンの設定・削除処理
	void CollisionHandler(int gid0, int gid1);
	void ClearCollisionHandlerList();

	std::mt19937& Rand();
	const GamePad& GetGamePad() const;
	const TexturePtr& GetTexture(const char* filename) const;

	//グローバルデータの取得設定処理
	double& UserVariable(const char* name) { return userNumbers[name]; }

	//メインカメラ設定処理
	void MainCamera(const std::shared_ptr<CameraComponent>& c) { mainCamera = c; }
	const std::shared_ptr<CameraComponent>& MainCamera() const { return mainCamera; }
	

	void TimeScale(float t) { timeScale = t; }
	float DeltaTime(float delta) const { return deltaTime; }
	float FPS() const { return fps; }
	void EnableShadow(bool b) { isEnableShadow = b; }
	void ToggleDrawOutline() { isDrawOutline = !isDrawOutline; }
	void SceneFadeStart(bool param) { isSceneFadeStart = param; }
	
private:

	GameEngine() = default;
	~GameEngine();
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;

	bool Update(float  delta);
	void UpdateFps();

	void Render();
	void RenderEntity() const;
	void RenderShadow() const;
	void RenderStencil() const;
	void RenderBloomEffect() const;
	void RenderFrameBuffer() const;

	void PushLevel();
	void PopLevel();
	void ClearLevel();

private:
	
	bool isInitalized = false;

	glm::vec2 windowSize;
	glm::vec2 viewportRect[2];

	//最終出力用バッファ
	BufferObject vbo, ibo;
	GLuint vao = 0;

	//画面輝度を調整するためのPBO
	BufferObject pbo[2];
	int pboIndexForWriting = -1;
	float luminanceScale = 1.0f;
	float keyValue = 0.18f;

	float deltaTime = 0;		///< 経過時間

	//fpsカウント用変数
	int bufferCount = 0;
	float fpsBuffer[60];
	float fps;

	bool isDrawOutline = false;		///< アウトラインを描画するか否か
	bool isEnableShadow = false;	///< シャドウマッピングを行うか

	UniformBufferPtr uboLight;
	UniformBufferPtr uboPostEffect;
	std::unordered_map<std::string, Shader::ProgramPtr> shaderMap;

	static const int bloomBufferCount = 4;
	ShadowParameter shadowParameter;

	//オフスクリーンバッファ群
	OffscreenBufferPtr offscreen;					/// バックバッファ	
	OffscreenBufferPtr offBloom[bloomBufferCount];	/// ブルームバッファ
	OffscreenBufferPtr offDepth;					/// 深度バッファ				
	OffscreenBufferPtr offStencil;					/// ステンシルバッファ

	//2D表示用スプライトレンダラ
	SpriteRenderer spriteRenderer;

	using TextureLevel = std::unordered_map<std::string, TexturePtr>;
	static const size_t minimalStackSize = 1;
	std::vector<TextureLevel> textureStack;

	Mesh::BufferPtr meshBuffer;
	Entity::BufferPtr entityBuffer;
	Font::Renderer fontRenderer;

	Uniform::LightData lightData;
	std::mt19937 rand;

	std::unordered_map<std::string, double> userNumbers;

	float timeScale = 1.0f;

	float sceneFadeTimer = 3.0f;
	bool isSceneFadeStart = false;

	//TODO : 試作用メインカメラコンポーネント
	std::shared_ptr<CameraComponent> mainCamera;

};
