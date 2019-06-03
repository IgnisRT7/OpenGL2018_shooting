/**
*	@file GameMainScene.cpp
*/
#include "GameMainScene.h"
#include "../GameState.h"
#include "../../Res/Audio/CueSheet_0.h"
#include "../../Res/Audio/testProject_acf.h"
#include "../GameEngine.h"

#include "Entity/Enemy.h"

#include "GameEndScene.h"
#include "TitleScene.h"

namespace GameState {

	/// 背景
	void Landscape::Initialize() {
	}

	void Landscape::Update(float delta) {
		entity->Position(entity->Position() + glm::vec3(0, 0, -10.0f * delta));
	}

	/// 背景(ステージ3用)

	/**
	*	背景の初期化処理
	*/
	void SpaceSphereMain::Initialize() {
	}

	/**
	*	背景の更新処理
	*/
	void SpaceSphereMain::Update(float delta) {

		glm::vec3 rotSpace = glm::eulerAngles(entity->Rotation());
		rotSpace.x += glm::radians(2.5f) * delta;
		entity->Rotation(rotSpace);
	}


	///メインゲーム画面のクラス定義

	/**
	*	初期化処理
	*/
	bool MainGame::Initialize() {

		GameEngine& game = GameEngine::Instance();

		//リソースのロード
		game.LoadMeshFromFile("Res/Model/Player.fbx");
		game.LoadMeshFromFile("Res/Model/Blast.fbx");
		game.LoadMeshFromFile("Res/Model/Toroid.fbx");
		game.LoadMeshFromFile("Res/Model/ItemBox.fbx");
		game.LoadMeshFromFile("Res/Model/sampleSphere.fbx");
		game.LoadTextureFromFile("Res/Model/Player.dds");
		game.LoadTextureFromFile("Res/Model/Toroid.dds");
		game.LoadTextureFromFile("Res/Model/Toroid.Normal.bmp");
		game.LoadTextureFromFile("Res/Model/ItemBoxSpeed.dds");
		game.LoadTextureFromFile("Res/Model/ItemBoxBullet.dds");
		game.LoadTextureFromFile("Res/Model/sphere.dds");

		game.LoadMeshFromFile("Res/Model/Landscape.fbx");
		game.LoadTextureFromFile("Res/Model/BG02.Diffuse.dds");
		game.LoadTextureFromFile("Res/Model/BG02.Normal.bmp");

		game.LoadMeshFromFile("Res/Model/City01.fbx");
		game.LoadTextureFromFile("Res/Model/City01.Diffuse.dds");
		game.LoadTextureFromFile("Res/Model/City01.Normal.bmp");

		game.LoadMeshFromFile("Res/Model/SpaceSphere.fbx");
		game.LoadTextureFromFile("Res/Model/SpaceSphere.dds");

		return true;
	}

	/**
	*	終了処理
	*/
	void MainGame::Finalize() {
	}

	/**
	*	再生処理
	*/
	void MainGame::Play() {

		GameEngine& game = GameEngine::Instance();

		///衝突判定用ハンドラの定義
		game.CollisionHandler(EntityGroupId_PlayerShot, EntityGroupId_Enemy);
		game.CollisionHandler(EntityGroupId_EnemyShot, EntityGroupId_Player);
		game.CollisionHandler(EntityGroupId_Player, EntityGroupId_Enemy);
		game.CollisionHandler(EntityGroupId_Player, EntityGroupId_Item);

		/// ライト・輝度の設定
		game.AmbientLight(glm::vec4(0.05f, 0.1f, 0.2f, 1));
		game.Light(0, { glm::vec4(1,100,1,1),glm::vec4(12000,12000,12000,1) });

		playerData = std::make_shared<Player>();

		game.MainCamera(std::make_shared<CameraComponent>());
		game.MainCamera()->LookAt(glm::vec3(0, 30, 0), glm::vec3(0, 0, 10));

		///シャドウの設定
		GameEngine::ShadowParameter shadowParam;
		shadowParam.lightPos = glm::vec3(20, 50, 50);
		shadowParam.lightDir = glm::normalize(glm::vec3(-25, -50, 25));
		shadowParam.lightUp = glm::vec3(0, 1, 0);
		shadowParam.near = 10;
		shadowParam.far = 200;
		shadowParam.range = glm::vec2(500, 500);
		game.Shadow(shadowParam);

		game.TimeScale(1);

		sceneTimer = 0;
	}

	/**
	*	停止処理
	*/
	void MainGame::Stop() {

		GameEngine& game = GameEngine::Instance();
	}

	/**
	*	画面に表示する情報の描画処理
	*/
	void MainGame::DrawScreenInfo() {

		GameEngine& game = GameEngine::Instance();

		if (sceneTimer <= 0) {
			//スコア表示処理

			char str[16];
			snprintf(str, 16, "SCORE :%08.0f", game.UserVariable("score"));
			game.FontScale(glm::vec2(3));
			game.FontColor(glm::vec4(1, 0, 0, 1));
			game.AddString(glm::vec2(-0.95f, 0.95f), str);

			snprintf(str, 16, "P :%02.0f", glm::max(0.0, static_cast<double>(playerData->RemainingPlayer())));
			game.AddString(glm::vec2(-0.95f, 0.8f), str);

			game.FontScale(glm::vec2(1));
			snprintf(str, 16, "P :%02.0f", game.FPS());
			game.AddString(glm::vec2(-0.95f, -0.85f), str);
		}
	}

	/**
	*	ステージの読み込み処理
	*/
	void MainGame::StageLoad(){

		GameEngine& game = GameEngine::Instance();
		
		game.StopAllAudio();
		game.RemoveAllEntity();

		++stageNo;

		auto playerEntity = game.AddEntity(EntityGroupId_Player, glm::vec3(0, 0, 0),
			"Aircraft", "Res/Model/Player.dds", playerData);

		//ステージごとのロード処理
		switch (stageNo % 3) {
		case 1: {
			game.PlayAudio(0, CRI_CUESHEET_0_MAINSCENE);

			stageTimer = 3;

			game.KeyValue(0.16f);

			//背景の更新処理
			for (int z = 0; z < 5; ++z) {
				const float offsetZ = static_cast<float>(z * 40 * 5);

				for (int x = 0; x < 5; ++x) {
					const float offsetX = static_cast<float>(x * 40 - 80) * 5.0f;
					game.AddEntity(EntityGroupId_Background, glm::vec3(offsetX, -100.0, offsetZ),
						"Landscape01", "Res/Model/BG02.Diffuse.dds", "Res/Model/BG02.Normal.bmp", std::make_shared<Landscape>());
				}
			}

			break;
		}
		case 2: {
			game.PlayAudio(0, CRI_CUESHEET_0_MAIN2SCENE);

			stageTimer = 1200;

			game.KeyValue(0.24f);

			//背景の初期化処理
			for (int z = 0; z < 5; ++z) {
				const float offsetZ = static_cast<float>(z * 40);

				for (int x = 0; x < 5; ++x) {
					const float offsetX = static_cast<float>(x * 40 - 80);
					game.AddEntity(EntityGroupId_Background, glm::vec3(offsetX, -10, offsetZ),
						"City01", "Res/Model/City01.Diffuse.dds", "Res/Model/City01.Normal.bmp", std::make_shared<Landscape>());
					game.AddEntity(EntityGroupId_Background, glm::vec3(offsetX, -10, offsetZ),
						"City01.Shadow", "Res/Model/City01.Diffuse.dds", "Res/Model/City01.Normal.bmp", std::make_shared<Landscape>());
				}
			}
			break;
			
		}
		case 0: {
			game.PlayAudio(0, CRI_CUESHEET_0_BOSSSCENE);

			stageTimer = 30;

			game.KeyValue(0.02f);

			game.AddEntity(EntityGroupId_Background, glm::vec3(0),
				"SpaceSphere", "Res/Model/SpaceSphere.bmp", std::make_shared<SpaceSphereMain>(), "NonLighting");

			break;
		}
		}
	}

	/**
	*	メインゲーム画面の更新
	*/
	void MainGame::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		//デバッグ用シャドウマッピング切り替え処理
		GamePad gamepad = game.GetGamePad();
		if (gamepad.buttonDown & GamePad::START) {
			static bool shadowMapping = false;
			shadowMapping = !shadowMapping;
			game.EnableShadow(shadowMapping);
		}

		static const float stageTime = 30;

		if (stageTimer < 0) {
			StageLoad();
		}

		stageTimer -= delta;

		//スポーン座標範囲
		std::uniform_int_distribution<> distributerX(-12, 12);
		std::uniform_int_distribution<> distributerZ(40, 44);

		interval -= delta;

		//敵スポナーの出現処理
		if (interval <= 0) {

			const std::uniform_real_distribution<> rndInterval(0.5f, 1);
			const std::uniform_int_distribution<> rndAddingCount(1, 5);

			const glm::vec3 pos(distributerX(game.Rand()), 0, distributerZ(game.Rand()));

			if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, pos,
				"Res/Model/Toroid.fbx", "Res/Model/Player.dds", std::make_shared<EnemyLaunchType>())) {

				p->Collision(collisionDataList[EntityGroupId_Others]);
			}

			interval = 5;
		}

		DrawScreenInfo();

		//シーン遷移処理
		if (sceneTimer == 0 && playerData->RemainingPlayer() < 0) {

			if (sceneTimer == 0) sceneTimer = 3;
			return;

		}
		if (sceneTimer > 0 && (sceneTimer -= delta) <= 0) {
			game.ReplaceScene(std::make_shared<GameEnd>());
			return;
		}
	}
}
