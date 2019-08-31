/**
*	@file GameEngine.h
*	@brief	エンジンの制御用
*	@brief	Takuya Yokoyama , tn-mai
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

/**
*	ゲームエンジンクラス
*/
class GameEngine {
	friend class SceneStack;
public:

	/**
	*	ゲームエンジンのインスタンスを取得する
	*
	*	@return ゲームエンジンのインスタンス
	*/
	static GameEngine& Instance();

	/**
	*	ゲームエンジンを初期化する
	*
	*	@param w	ウインドウの描画の幅(ピクセル)
	*	@param h	ウインドウの描画の高さ(ピクセル)
	*	@param titleウインドウタイトル
	*
	*	@retval true	初期化成功
	*	@retval false	初期化失敗
	*
	*	Run関数を呼び出す前に、一度だけ呼び出しておく必要がある
	*/
	bool Init(int w, int h, const char* title);

	/**
	*	ゲームを実行する
	*/
	void Run();

	/**
	*	@copydoc Audio::Initialize
	*/
	bool InitAudio(const char* acfPath, const char* acbPath, const char* awbPath, const char* dspBusName);

	/**
	*	@copydoc Audio::Play
	*/
	void PlayAudio(int playerId, int cueId);

	/**
	*	@copydoc Audio::Stop
	*/
	void StopAudio(int playerId);

	/**
	*	@copydoc Audio::StopAll
	*/
	void StopAllAudio();

	/**
	*	テクスチャを読み込む
	*
	*	@param filename テクスチャファイル名
	*
	*	@retval true	読み込み成功
	*	@retval false	読み込み失敗
	*/
	bool LoadTextureFromFile(const char* filename);

	/**
	*	メッシュを読み込む
	*
	*	@param filename メッシュファイル名
	*
	*	@retval true	読み込み成功
	*	@retval false	読み込み失敗
	*/
	bool LoadMeshFromFile(const char* filename);

	/**
	*	フォントを読み込む
	*
	*	@param filename フォントファイル名
	*
	*	@retval true	読み込み成功
	*	@retval false	読み込み失敗
	*/
	bool LoadFontFromFile(const char* filename);

	/**
	*	エンティティを追加する
	*
	*	@param groupId	エンティティのグループID
	*	@param pos		エンティティの座標
	*	@param meshName	エンティティの表示に使用するメッシュ名
	*	@param texName	エンティティの表示に使うテクスチャファイル名
	*	@param func		エンティティの状態を更新する関数(または関数オブジェクト)
	*	@param shader	エンティティの表示に使うシェーダ名
	*
	*	@return 追加したエンティティへのポインタ
	*			これ以上エンティティを追加できない場合はnullptrが返される
	*			回転や拡大率はこのポインタ経由で設定する
	*			なお、このポインタをアプリケーション側で保持する必要はない
	*/
	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);

	/**
*	エンティティを追加する
*
*	@param groupId	エンティティのグループID
*	@param pos		エンティティの座標
*	@param meshName	エンティティの表示に使用するメッシュ名
*	@param texName	エンティティの表示に使うテクスチャファイル名
*	@param texNormal
*	@param func		エンティティの状態を更新する関数(または関数オブジェクト)
*	@param shader	エンティティの表示に使うシェーダ名
*
*	@return 追加したエンティティへのポインタ
*			これ以上エンティティを追加できない場合はnullptrが返される
*			回転や拡大率はこのポインタ経由で設定する
*			なお、このポインタをアプリケーション側で保持する必要はない
*/
	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, const char* normalName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);

	/**
	*	スプライトの追加処理
	*
	*	@param s	追加するスプライト
	*/
	void AddSprite(Sprite& s);

	/**
	*	エンティティを削除する
	*
	*	@param 削除するエンティティのポインタ
	*/
	void RemoveEntity(Entity::Entity*);

	/**
	*	全てのエンティティを削除する
	*/
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

	/**
	*	シーンをプッシュします
	*/
	void PushScene(ScenePtr s);

	/**
	*	シーンをポップします
	*/
	void PopScene();

	/**
	*	シーンを取り換えます
	*/
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

	/**
	*	ライトを設定する
	*
	*	@param indes	設定するライトのインデっクス
	*	@param lilght	ライトデータ
	*/
	void Light(int index, const Uniform::PointLight& light);

	/**
	*	ライトを取得する
	*
	*	@param inex 取得するライトのインデックス
	*
	*	@return ライトデータ
	*/
	const Uniform::PointLight& Light(int index) const;

	/**
	*	環境光を設定する
	*
	*	@param color	環境光の明るさ
	*/
	void AmbientLight(const glm::vec4& color);

	/**
	*	環境光を取得する
	*
	*	@return 環境光の明るさ
	*/
	const glm::vec4& AmbientLight() const;

	//輝度の閾値の取得・設定処理
	void KeyValue(float k) { keyValue = k; }
	const float KeyValue() const { return keyValue; }

	//表示・非表示の処理 現在未使用
	void GroupVisibility(int groupId, int index, bool isVisible) { entityBuffer->GroupVisibility(groupId, index, isVisible); }
	bool GroupVisibility(int groupId, int index) const { return entityBuffer->GroupVisibility(groupId, index); }

	/**
	*	衝突解決ハンドラを設定する
	*
	*	@param gid0		衝突対象のグループID
	*	@param gid1		衝突対象のグループID
	*	@param handler	衝突解決ハンドラ
	*
	*	衝突が発生し衝突解決ハンドラが呼び出されるとき、
	*	より小さいグループIDを持つエンティティから先に渡される。
	*	ココで指定したグループIDの順序とは無関係であることに注意すること
	*
	*	CollisionHandler(10,1,Func)
	*	というコードでハンドラを登録したとする、衝突が発生すると、
	*	Func(グループID=1のエンティティ、グループID=10のエンティティ)
	*	のように呼び出される
	*/
	void CollisionHandler(int gid0, int gid1);

	/**
	*	衝突ハンドラのリストをクリアする
	*/
	void ClearCollisionHandlerList();

	/**
	*	乱数オブジェクトを取得する
	*
	*	@return 乱数オブジェクト
	*/
	std::mt19937& Rand();

	/**
	*	ゲームパッドの状態を取得する
	*/
	const GamePad& GetGamePad() const;

	/**
	*	テクスチャの取得
	*
	*	@param filename テクスチャ名
	*
	*	@return true	取得したテクスチャ
	*	@return false	ダミーのテクスチャ
	*/
	const TexturePtr& GetTexture(const char* filename) const;

	//グローバルデータの取得設定処理
	double& UserVariable(const char* name) { return userNumbers[name]; }

	//メインカメラ設定処理
	void MainCamera(const std::shared_ptr<CameraComponent>& c) { mainCamera = c; }
	const std::shared_ptr<CameraComponent>& MainCamera() const { return mainCamera; }
	
	/**
	*	アスペクト比に応じたビューポートサイズの変更
	*
	*	@param width	画面横幅
	*	@param height	画面縦幅
	*	@param aspect	アスペクト比
	*/
	void CalculateViewPortByAspect(int width,int height,float aspect);

	//シェーダ取得処理
	Shader::ProgramPtr& Shader(const char* pass) { return shaderMap[pass]; }

	void TimeScale(float t) { timeScale = t; }
	float DeltaTime() const { return deltaTime; }
	float FPS() const { return fps; }
	void EnableShadow(bool b) { isEnableShadow = b; }
	void ToggleDrawOutline() { isDrawOutline = !isDrawOutline; }
	void SceneFadeStart(bool param) { isSceneFadeStart = param; }

	
private:

	GameEngine() = default;
	~GameEngine();
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;

	/**
	*	ゲームの状態を更新する
	*
	*	@param delta	前回の更新からの経過時間(秒)
	*/
	bool Update(float  delta);

	/**
	*	Fpsの更新処理
	*/
	void UpdateFps();

	/**
	*	ゲームの状態を描画する
	*/
	void Render();

	/**
	*	エンティティを描画する
	*/
	void RenderEntity() const;

	/**
	*	デプスシャドウマップを描画する
	*/
	void RenderShadow() const;

	/**
	*	ステンシルバッファを描画する
	*/
	void RenderStencil() const;

	/**
	*	ブルームエフェクトを描画する
	*/
	void RenderBloomEffect() const;

	/**
	*	オフスクリーンバッファに描画する
	*/
	void RenderFrameBuffer() const;

	/**
	*	リソーススタックに新しいリソースレベルを作成する
	*/
	void PushLevel();

	/*
	*	リソーススタックの末尾のリソースレベルを除去する
	*/
	void PopLevel();

	/**
	*	リソーススタックの末尾のリソースレベルを空の状態にする
	*/
	void ClearLevel();

private:
	
	bool isInitalized = false;

	glm::vec2 windowSize;
	glm::vec2 viewportRect[2];
	float windowAspect = 1.0f;

	//最終出力用バッファ
	BufferObject vbo, ibo;
	VertexArrayObject vao;

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
