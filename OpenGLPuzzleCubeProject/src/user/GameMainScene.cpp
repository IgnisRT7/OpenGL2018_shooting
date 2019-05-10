/**
*	@file GameMainScene.cpp
*/
#include "GameMainScene.h"
#include "../GameState.h"
#include "../../Res/Audio/SampleSound_acf.h"
#include "../../Res/Audio/SampleCueSheet.h"
#include "../GameEngine.h"

#include "Entity/Enemy.h"

#include "GameEndScene.h"

namespace GameState {

	/// 背景
	void Landscape::Initialize() {
	}

	void Landscape::Update(double delta) {
		entity->Position(entity->Position() + glm::vec3(0, 0, -4.0f * delta));
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
	void SpaceSphereMain::Update(double delta) {

		glm::vec3 rotSpace = glm::eulerAngles(entity->Rotation());
		rotSpace.x += static_cast<float>(glm::radians(2.5) * delta);
		entity->Rotation(rotSpace);
	}

	///メインゲーム画面のクラス定義

	/**
	*	メインゲーム画面のコンストラクタ
	*/
	MainGame::MainGame() {

		GameEngine& game = GameEngine::Instance();
		game.ClearCollisionHandlerList();
		game.CollisionHandler(EntityGroupId_PlayerShot, EntityGroupId_Enemy, nullptr);
		game.CollisionHandler(EntityGroupId_EnemyShot, EntityGroupId_Player, nullptr);
		game.CollisionHandler(EntityGroupId_Player, EntityGroupId_Enemy,nullptr);
		game.CollisionHandler(EntityGroupId_Player, EntityGroupId_Item, nullptr);

		playerData = std::make_shared<Player>();
	}

	/**
	*	テスト用プレイヤー捜索関数
	*/
	bool SearchPlayer(Entity::Entity* entity) {

		if (entity->CastTo<Player>()) {
			return true;
		}
		return false;
	}

	/**
	*	メインゲーム画面の更新
	*/
	void MainGame::operator()(double delta) {

		GameEngine& game = GameEngine::Instance();
		static const float stageTime = 30;

		if (stageTimer < 0) {

			++stageNo;

			///カメラの設定
			game.Camera(0, { glm::vec4(0,30,0,1),glm::vec3(0,0,10),glm::vec3(0,0,1) });
			game.Camera(1, { glm::vec4(0,20,-8,1),glm::vec3(0,0,12),glm::vec3(0,0,1) });
			game.GroupVisibility(EntityGroupId_Background, 0, true);
			game.GroupVisibility(EntityGroupId_Background, 1, false);

			game.GroupVisibility(EntityGroupId_Enemy, 0, false);
			game.GroupVisibility(EntityGroupId_Enemy, 0, true);

			game.GroupVisibility(EntityGroupId_Player, 0, false);
			game.GroupVisibility(EntityGroupId_Player, 0, true);

			game.GroupVisibility(EntityGroupId_Others, 0, false);
			game.GroupVisibility(EntityGroupId_Others, 0, true);

			/// ライト・輝度の設定
			game.AmbientLight(glm::vec4(0.05f, 0.1f, 0.2f, 1));
			game.Light(0, { glm::vec4(1,100,1,1),glm::vec4(12000,12000,12000,1) });
			game.KeyValue(0.24f);

			/// シャドウの設定
			GameEngine::ShadowParameter shadowParam;
			shadowParam.lightPos = glm::vec3(20, 50, 50);
			shadowParam.lightDir = glm::normalize(glm::vec3(-25, -50, 25));
			shadowParam.lightUp = glm::vec3(0, 0, 1);
			shadowParam.near = 10;
			shadowParam.far = 200;
			shadowParam.range = glm::vec2(300, 300);
			game.Shadow(shadowParam);

			/// エンティティの初期化
			game.RemoveAllEntity();
			game.ClearLevel();
			game.LoadMeshFromFile("Res/Model/Player.fbx");
			game.LoadMeshFromFile("Res/Model/Blast.fbx");
			game.LoadMeshFromFile("Res/Model/Toroid.fbx");
			game.LoadMeshFromFile("Res/Model/ItemBox.fbx");
			game.LoadTextureFromFile("Res/Model/Player.dds");
			game.LoadTextureFromFile("Res/Model/Toroid.dds");
			game.LoadTextureFromFile("Res/Model/Toroid.Normal.bmp");
			game.LoadTextureFromFile("Res/Model/ItemBoxSpeed.dds");
			game.LoadTextureFromFile("Res/Model/ItemBoxBullet.dds");

			switch (stageNo % 3) {
			case 1: {

				stageTimer = 30;

				game.KeyValue(0.16f);

				game.LoadMeshFromFile("Res/Model/Landscape.fbx");
				game.LoadTextureFromFile("Res/Model/BG02.Diffuse.dds");
				game.LoadTextureFromFile("Res/Model/BG02.Normal.bmp");

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

				stageTimer = 30;

				game.KeyValue(0.24f);
				game.LoadMeshFromFile("Res/Model/City01.fbx");
				game.LoadTextureFromFile("Res/Model/City01.Diffuse.dds");
				game.LoadTextureFromFile("Res/Model/City01.Normal.bmp");

				//背景の初期化処理
				for (int z = 0; z < 5; ++z) {
					const float offsetZ = static_cast<float>(z * 40);

					for (int x = 0; x < 5; ++x) {
						const float offsetX = static_cast<float>(x * 40 - 80);
						game.AddEntity(EntityGroupId_Background, glm::vec3(offsetX, -10, offsetZ),
							"City01", "Res/Model/City01.Diffuse.dds", "Res/Model/City01.Normal.bmp", std::make_shared<Landscape>());
						game.AddEntity(EntityGroupId_Background, glm::vec3(offsetX, -10, offsetX),
							"City01.Shadow", "Res/Model/City01.Diffuse.dds", "Res/Model/City01.Normal.bmp", std::make_shared<Landscape>());
					}
				}
				break;

			}
			case 0: {

				stageTimer = 30;

				game.KeyValue(0.02f);
				game.LoadMeshFromFile("Res/Model/SpaceSphere.fbx");
				game.LoadTextureFromFile("Res/Model/SpaceSphere.bmp");
				game.AddEntity(EntityGroupId_Background, glm::vec3(0),
					"SpaceSphere", "Res/Model/SpaceSphere.bmp", std::make_shared<SpaceSphereMain>(), "NonLighting");

				break;
			}
			}

			auto playerEntity = game.AddEntity(EntityGroupId_Player, glm::vec3(0, 0, 0),
				"Aircraft", "Res/Model/Player.dds",playerData);

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

		auto playertest = game.FindEntity(SearchPlayer);

		if (sceneTimer == 0 && playerData->RemainingPlayer() < 0) {

			if (sceneTimer == 0) sceneTimer = 3;
		}


		if (sceneTimer <= 0) {
			//スコア表示処理
			char str[16];
			snprintf(str, 16, "SCORE :%08.0f", game.UserVariable("score"));
			game.FontScale(glm::vec2(3));
			game.FontColor(glm::vec4(1, 0, 0, 1));
			game.AddString(glm::vec2(-0.95f, 0.95f), str);

			snprintf(str, 16, "P :%02.0f", glm::max(0.0, static_cast<double>(playerData->RemainingPlayer())));
			game.AddString(glm::vec2(-0.95f, 0.8f), str);
			
			static int bufferCount = 0;
			static float fpsBuffer[60];

			fpsBuffer[++bufferCount % 60] = delta;

			float fps = 0;
			for (int i = 0; i < 60; i++) {
				fps += fpsBuffer[i];
			}

			fps /= 60.0f;
			fps = 1 / fps;

			snprintf(str, 16, "P :%02.0f",fps);
			game.AddString(glm::vec2(-0.95f, 0.65f), str);


			//カメラ移動処理
			GameEngine::CameraData camera = game.Camera(0);
			float cameraMoveValue = fmod(static_cast<float>(stageTimer), 45.0f) * (glm::radians(360.0f) / 45.0f);
			camera.position.x = glm::cos(cameraMoveValue) * 5.0f;
			game.Camera(0, camera);
		}
		else {

			if ((sceneTimer -= delta) <= 0) {
				game.UpdateFunc(GameEnd());
			}
		}
	}
}
