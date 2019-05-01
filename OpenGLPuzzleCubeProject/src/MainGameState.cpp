/**
*	@file MainGameState.cpp
*/
#include "GameState.h"
#include "GameEngine.h"
#include "../Res/Audio/SampleCueSheet.h"
//#include "ResourceName.h"
#include <algorithm>

namespace GameState {
	///衝突形状リスト
	static const Entity::CollisionData collisionDataList[] = {
		{ glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f,1.0f,1.0f) },
		{ glm::vec3(-0.5f,-0.5f,-1.0f), glm::vec3(0.5f,0.5f,1.0f) },
		{ glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f,1.0f,1.0f) },
		{ glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.25f,0.25f,0.25f) },
	};

	void UpdateLandscape(Entity::Entity& entity, double delta) {
		entity.Position(entity.Position() + glm::vec3(0, 0, -4.0f * delta));
	}

	///背景の更新
	void UpdateSpaceSphere1(Entity::Entity& entity, double delta) {

		glm::vec3 rotSpace = glm::eulerAngles(entity.Rotation());
		rotSpace.x += static_cast<float>(glm::radians(2.5) * delta);
		entity.Rotation(rotSpace);
	}

	/**
	*	敵の円盤の状態を更新する
	*/
	struct UpdateToroid {

		UpdateToroid(int typeID = 0) :enemyType(typeID){
		}

		void operator()(Entity::Entity& entity, double delta) {

			timer += delta;

			float rot = glm::angle(entity.Rotation());
			const glm::vec3 pos = entity.Position();

			GameEngine& game = GameEngine::Instance();
			

			// 移動処理
			switch (enemyType) {
			case 0:	/// デフォルトの処理まっすぐ進む

				entity.Velocity(glm::vec3(0, 0, -10));


				break;

			case 1:	/// 蛇行して下方向へ
			{
				float velX = glm::cos(timer * 2) * 10 - entity.Velocity().x;
				glm::vec3 vel = glm::vec3(velX, 0, -5);
				entity.Velocity(vel);

				break;
			}
			case 2: ///プレイヤーキャラへ軽追尾

				

				break;

			case 3:

			default:
				break;
			}
		
			// 円盤を回転させる.
			rot += glm::radians(180.0f) * static_cast<float>(delta);
			if (rot > glm::pi<float>() * 2.0f) {
				rot -= glm::pi<float>() * 2.0f;
			}
			entity.Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

			//画面外判定処理
			if (std::abs(pos.x) > 200.0f || std::abs(pos.z) > 200.0f) {
				GameEngine::Instance().RemoveEntity(&entity);
				std::cout << "Toroid is destroyed!" << std::endl;
				return;
			}

		}


		float timer = 0;
		int enemyType = 0;

	};

	struct EnemyLaunchType {

		void operator()(Entity::Entity& entity, float delta) {

			GameEngine& game = GameEngine::Instance();

			time += delta;

			if (time >= spawnMax * spawnInterval) {
				//スポーン終了

				std::cout << "EnemyLaunchType is Destroyed!" << std::endl;

				entity.Destroy();
				return;
			}

			if (launchIndex < static_cast<int>(time / spawnInterval)) {

				std::cout << "敵がスポーンしたよ" << std::endl;


				Entity::Entity* p = game.AddEntity(EntityGroupId_Enemy, entity.Position(),
					"Toroid", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", UpdateToroid(0));
				//game.AddEntity(EntityGroupId_Enemy, entity.Position(), "mesh","texture",UpdateToroid());

				p->Collision(collisionDataList[EntityGroupId_Enemy]);

				launchIndex++;
			}
		}

		float spawnInterval = 0.5;	//スポーンする間隔
		float spawnMax = 5;			//スポーン数
		float time;					//経過時間
		int launchIndex = -1;		//出撃している敵の数
		int enemyType = -1;			//出撃する敵の種類
	};


	

	/**
	*	自機の弾の更新
	*/
	struct UpdatePlayerShot {
		void operator()(Entity::Entity& entity, double delta) {

			if (!isInitialized) {
				entity.CastShadow(false);
			}

			const glm::vec3 pos = entity.Position();
			if (std::abs(pos.x) > 40 || pos.z < -4 || pos.z > 40) {
				entity.Destroy();
				return;
			}
		}

		bool isInitialized = false;
	};

	/**
	*	爆発の更新
	*/
	struct UpdateBlast {

		void operator()(Entity::Entity& entity, double delta) {

			if (!isInitialized) {
				entity.CastShadow(false);
			}

			timer += delta;

			if (timer >= 0.5) {
				entity.Destroy();
				return;
			}
			const float variation = static_cast<float>(timer * 4);	//変化量
			entity.Scale(glm::vec3(static_cast<float>(1 + variation)));	//徐々に拡大していく

																		//時間経過で色と透明度を変化させる
			static const glm::vec4 color[] = {
				glm::vec4(1.0f,1.0f,0.75f,1),
				glm::vec4(1.0f,0.5f,0.1f,1),
				glm::vec4(0.25f,0.1f,0.1f,0),
			};
			const glm::vec4 col0 = color[static_cast<int>(variation)];
			const glm::vec4 col1 = color[static_cast<int>(variation) + 1];
			const glm::vec4 newColor = glm::mix(col0, col1, std::fmod(variation, 1));
			entity.Color(newColor);

			//Y軸回転させる
			glm::vec3 euler = glm::eulerAngles(entity.Rotation());
			euler.y += glm::radians(60.0f) + static_cast<float>(delta);
			entity.Rotation(glm::quat(euler));
		}

		double timer = 0;
		bool isInitialized = false;
	};


	/**
	*	自機の更新
	*/
	struct UpdatePlayer {
		void operator()(Entity::Entity& entity, double delta) {

			static float timer = 0;
			timer += delta * 100;

			/*TODO : プレイヤーのダメージ時の点滅アクションテスト
			auto color = entity.Color();
			color.a = (glm::sin(timer) + 1) * 0.5;
			entity.Color(color);
			*/

			GameEngine& game = GameEngine::Instance();

			
			const GamePad gamepad = game.GetGamePad();			

			//移動量計算処理
			glm::vec3 vec;
			float rotZ = 0;
			if (gamepad.buttons & GamePad::DPAD_LEFT) {
				vec.x = 1;
				rotZ = -glm::radians(30.0f);
			}
			else if (gamepad.buttons & GamePad::DPAD_RIGHT) {
				vec.x = -1;
				rotZ = glm::radians(30.0f);
			}
			if (gamepad.buttons & GamePad::DPAD_UP) {
				vec.z = 1;
			}
			else if (gamepad.buttons & GamePad::DPAD_DOWN) {
				vec.z = -1;
			}
			if (vec.x || vec.z) {
				vec = glm::normalize(vec) * 5.0f;
			}

			glm::vec3 rt = glm::vec3(-25, -100, 1);
			glm::vec3 ld = glm::vec3(25, 100, 80)
				;
			entity.Velocity(vec);
			entity.Rotation(glm::quat(glm::vec3(0, 0, rotZ)));
			glm::vec3 pos = entity.Position();
			pos = glm::min(ld, glm::max(pos, rt));
			entity.Position(pos);

			//弾の発射処理
			if (gamepad.buttons & GamePad::A) {
				shotInterval -= delta;
				float tmp = std::abs(shotInterval);

				if (shotInterval <= 0) {

					glm::vec3 pos = entity.Position();
					pos.x -= 0.3f;

					game.PlayAudio(0, 0);
					for (int i = 0; i < 2; ++i) {
						if (Entity::Entity* p = game.AddEntity(EntityGroupId_PlayerShot, pos, "NormalShot", "Res/Model/Player.dds", UpdatePlayerShot())) {
							p->Velocity(glm::vec3(0, 0, 80));
							p->Collision(collisionDataList[EntityGroupId_PlayerShot]);
						}
						pos.x += 0.25f;
					}
					shotInterval = 0.1f - tmp;
				}

			}
			else {
				shotInterval = 0;
			}

		}
	private:
		double shotInterval = 0;
	};

	/**
	*	自機の弾と敵の衝突判定
	*/
	void PlayerShotAndEnemyCollisionHandler(Entity::Entity& lhs, Entity::Entity& rhs) {

		GameEngine& game = GameEngine::Instance();

		//爆発エフェクト
		if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, rhs.Position(), "Blast", "Res/Model/Toroid.dds", UpdateBlast())) {
			const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
			p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
			p->Color(glm::vec4(1.0f, 0.75f, 0.5f, 1.0f));
			game.UserVariable("score") += 100;
		}

		//爆発音
		game.PlayAudio(1, CRI_SAMPLECUESHEET_BOMB);
		lhs.Destroy();
		rhs.Destroy();
	}

	/**
	*	自機と敵の衝突判定
	*/
	void PlayyerAndEnemyCollisionHandler(Entity::Entity& lhs, Entity::Entity& rhs) {

		GameEngine& game = GameEngine::Instance();

		//爆発エフェクト
		if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, rhs.Position(), "Blast", "Res/Model/Toroid.dds", UpdateBlast())) {
			const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
			p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
			game.UserVariable("score") += 100;
		}

		//爆発音
		game.PlayAudio(1, CRI_SAMPLECUESHEET_BOMB);

		if (lhs.GroupId() == EntityGroupId_Player) {
			rhs.Destroy();
		}
		else {
			lhs.Destroy();
		}

		game.UserVariable("player") -= 1;

	}

	/**
	*	メインゲーム画面のコンストラクタ
	*/
	MainGame::MainGame() {

		GameEngine& game = GameEngine::Instance();
		game.ClearCollisionHandlerList();
		game.CollisionHandler(EntityGroupId_PlayerShot, EntityGroupId_Enemy, PlayerShotAndEnemyCollisionHandler);
		game.CollisionHandler(EntityGroupId_Player, EntityGroupId_Enemy, PlayyerAndEnemyCollisionHandler);
	}

	/**
	*	メインゲーム画面の更新
	*/
	void MainGame::operator()(double delta) {

		GameEngine& game = GameEngine::Instance();
		static const float stageTime = 60;

		if (stageTimer < 0) {

			++stageNo;
			stageTimer = stageTime;
			game.Camera(0, { glm::vec4(0,100,0,1),glm::vec3(0,0,10),glm::vec3(0,0,1) });
			game.Camera(1, { glm::vec4(0,20,-8,1),glm::vec3(0,0,12),glm::vec3(0,0,1) });
			game.GroupVisibility(EntityGroupId_Background, 0, true);
			game.GroupVisibility(EntityGroupId_Background, 1, false);

			game.GroupVisibility(EntityGroupId_Enemy, 0, false);
			game.GroupVisibility(EntityGroupId_Enemy, 0, true);

			game.GroupVisibility(EntityGroupId_Player, 0, false);
			game.GroupVisibility(EntityGroupId_Player, 0, true);

			game.GroupVisibility(EntityGroupId_Others, 0, false);
			game.GroupVisibility(EntityGroupId_Others, 0, true);

			game.AmbientLight(glm::vec4(0.05f, 0.1f, 0.2f, 1));
			game.Light(0, { glm::vec4(1,100,1,1),glm::vec4(12000,12000,12000,1) });
			game.KeyValue(0.24);

			//シャドウの設定
			GameEngine::ShadowParameter shadowParam;
			shadowParam.lightPos = glm::vec3(20, 50, 50);
			shadowParam.lightDir = glm::normalize(glm::vec3(-25, -50, 25));
			shadowParam.lightUp = glm::vec3(0, 0, 1);
			shadowParam.near = 10;
			shadowParam.far = 200;
			shadowParam.range = glm::vec2(300, 300);
			game.Shadow(shadowParam);

			game.RemoveAllEntity();
			game.ClearLevel();
			game.LoadMeshFromFile("Res/Model/Player.fbx");
			game.LoadMeshFromFile("Res/Model/Blast.fbx");
			game.LoadMeshFromFile("Res/Model/Toroid.fbx");
			game.LoadTextureFromFile("Res/Model/Player.dds");
			game.LoadTextureFromFile("Res/Model/Toroid.dds");
			game.LoadTextureFromFile("Res/Model/Toroid.Normal.bmp");




			switch (stageNo % 3) {
			case 1: {

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
							"Landscape01", "Res/Model/BG02.Diffuse.dds", "Res/Model/BG02.Normal.bmp", &UpdateLandscape);
					}
				}

				break;
			}
			case 2: {

				game.KeyValue(0.24f);
				game.LoadMeshFromFile("Res/Model/City01.fbx");
				game.LoadTextureFromFile("Res/Model/City01.Diffuse.dds");
				game.LoadTextureFromFile("Res/Model/City01.Normal.bmp");

				//背景の更新処理
				for (int z = 0; z < 5; ++z) {
					const float offsetZ = static_cast<float>(z * 40);

					for (int x = 0; x < 5; ++x) {
						const float offsetX = static_cast<float>(x * 40 - 80);
						game.AddEntity(EntityGroupId_Background, glm::vec3(offsetX, -10, offsetZ),
							"City01", "Res/Model/City01.Diffuse.dds", "Res/Model/City01.Normal.bmp", &UpdateLandscape);
						game.AddEntity(EntityGroupId_Background, glm::vec3(offsetX, -10, offsetX),
							"City01.Shadow", "Res/Model/City01.Diffuse.dds", "Res/Model/City01.Normal.bmp", &UpdateLandscape);
					}
				}
				break;

			}
			case 0: {

				game.KeyValue(0.02f);
				game.LoadMeshFromFile("Res/Model/SpaceSphere.fbx");
				game.LoadTextureFromFile("Res/Model/SpaceSphere.bmp");
				game.AddEntity(EntityGroupId_Background, glm::vec3(0),
					"SpaceSphere", "Res/Model/SpaceSphere.bmp", &UpdateSpaceSphere1, "NonLighting");

				break;
			}
			}


			auto pPlayer = game.AddEntity(EntityGroupId_Player, glm::vec3(0, 0, 0),
				"Aircraft", "Res/Model/Player.dds", UpdatePlayer());
			pPlayer->Collision(collisionDataList[EntityGroupId_Player]);

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

			//for (int i = rndAddingCount(game.Rand()); i > 0; --i) {
				const glm::vec3 pos(distributerX(game.Rand()), 0, distributerZ(game.Rand()));

				if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, pos,
					"Res/Model/Toroid.fbx", "Res/Model/Player.dds", EnemyLaunchType())) {

					//p->Velocity(glm::vec3(pos.x < 0 ? 0.5f : -0.5f, 0, -1.0f));
					p->Collision(collisionDataList[EntityGroupId_Others]);
				}
			//}

			interval = 5;// rndInterval(game.Rand());

		}

		//スコア表示処理
		char str[16];
		snprintf(str, 16, "%08.0f", game.UserVariable("score"));
		game.FontScale(glm::vec2(1.5));
		game.FontColor(glm::vec4(1, 1, 1, 1));
		game.AddString(glm::vec2(-0.3f, 1.0f), str);

		snprintf(str, 16, "%02.0f", game.UserVariable("player"));
		std::string player = std::string("p - 0") + std::to_string((int)game.UserVariable("player"));
		game.AddString(glm::vec2(-0.9, 1.0f), player.c_str());

		//カメラ移動処理
		GameEngine::CameraData camera = game.Camera(0);
		float cameraMoveValue = fmod(static_cast<float>(stageTimer), 45.0f) * (glm::radians(360.0f) / 45.0f);
		camera.position.x = glm::cos(cameraMoveValue) * 5.0f;
		game.Camera(0, camera);
	}


}
