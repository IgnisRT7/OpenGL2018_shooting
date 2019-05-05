/**
*	@file GameMainScene.cpp
*/
#include "GameMainScene.h"
#include "../GameState.h"
#include "../../Res/Audio/SampleSound_acf.h"
#include "../../Res/Audio/SampleCueSheet.h"
#include "../GameEngine.h"

#include "Entity/Bullet.h"

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
	void SpaceSphereMain::Update(double delta){

		glm::vec3 rotSpace = glm::eulerAngles(entity->Rotation());
		rotSpace.x += static_cast<float>(glm::radians(2.5) * delta);
		entity->Rotation(rotSpace);
	}

	/// 爆発エフェクトクラス定義

	/**
	*	初期化処理
	*/
	void Blast::Initialize() {

		entity->CastShadow(false);
	}

	/**
	*	更新処理
	*/
	void Blast::Update(double delta) {

		timer += delta;

		if (timer >= 0.3) {
			entity->Destroy();
			return;
		}
		const float variation = static_cast<float>(timer * 4);	//変化量
		entity->Scale(glm::vec3(static_cast<float>(1 + variation)));	//徐々に拡大していく

																	//時間経過で色と透明度を変化させる
		static const glm::vec4 color[] = {
			glm::vec4(1.0f,1.0f,0.75f,1),
			glm::vec4(1.0f,0.5f,0.1f,1),
			glm::vec4(0.25f,0.1f,0.1f,0),
		};
		const glm::vec4 col0 = color[static_cast<int>(variation)];
		const glm::vec4 col1 = color[static_cast<int>(variation) + 1];
		const glm::vec4 newColor = glm::mix(col0, col1, std::fmod(variation, 1));
		entity->Color(newColor);

		//Y軸回転させる
		glm::vec3 euler = glm::eulerAngles(entity->Rotation());
		euler.y += glm::radians(60.0f) + static_cast<float>(delta);
		entity->Rotation(glm::quat(euler));
	}

	/// 自機弾のクラス定義

	/**
	*	自機の弾の初期化処理
	*
	void PlayerShot::Initialize() {

		entity->CastShadow(false);

		entity->Velocity(glm::vec3(0, 0, 40));
	}

	/**
	*	自機の弾の更新処理
	*
	void PlayerShot::Update(double delta) {

		const glm::vec3 pos = entity->Position();
		if (std::abs(pos.x) > 40 || pos.z < -4 || pos.z > 40) {
			entity->Destroy();
			return;
		}
	}

	void PlayerShot::CollisionEnter(Entity::Entity& e) {

		entity->Destroy();
	}*/

	/// アイテムクラスの定義

	/**
	*	アイテムの初期化処理
	*/
	void Item::Initialize() {
		//entity->Scale({ 50,50,50 });
		entity->Velocity({ 0,0,-5 });
		entity->Color({ 1,0,0,1 });
	}

	/**
	*	アイテムの更新処理
	*/
	void Item::Update(double delta) {

		glm::vec3 pos = entity->Position();

		//画面外判定処理
		if (std::abs(pos.x) > 200.0f || std::abs(pos.z) > 200.0f) {
			GameEngine::Instance().RemoveEntity(entity);
			return;
		}
	}

	/**
	*	アイテムの衝突判定処理
	*/
	void Item::CollisionEnter(Entity::Entity& e) {

		auto p = e.EntityData();

		entity->Destroy();

	}


	/// プレイヤークラスの定義

	/**
	*	ステージ開始後に行われる処理
	*
	void Player::StartMove(double delta) {

		if (entity->Position().z >= 0) {
			
			isStartingMove = false;
			entity->Velocity(glm::vec3(0, 0, 0));
		}
	}

	void Player::Initialize() {

		entity->Position(glm::vec3(0, 0, -8));
		entity->Velocity(glm::vec3(0, 0, 15));
		startMovValue = 100;
		isStartingMove = true;
	}

	void Player::Update(double delta) {

		timer += static_cast<float>(delta);

		//点滅処理
		if (damageTimer > 0) {
			const float speedMul = 30.0f;
			damageTimer -= static_cast<float>(delta);

			float colorAlpha = damageTimer <= 0 ? 1 : glm::max(0.0f,(glm::sin(damageTimer*speedMul)));

			entity->Color(glm::vec4(1, 1, 1, colorAlpha));
		}
		

		if (isStartingMove) {
			StartMove(delta);
		}
		else {

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
				vec = glm::normalize(vec) * moveSpeed;
			}

			/**
			*	画面輝度の設定
			*
			if (gamepad.buttonDown & GamePad::B ) {
			
				game.KeyValue(glm::min(1.0, game.KeyValue() + delta));
			}
			else if (gamepad.buttonDown & GamePad::X) {

				game.KeyValue(glm::max(0.0, game.KeyValue() - delta));
			}


			entity->Velocity(vec);
			glm::vec3 pos = entity->Position();
			pos = glm::min(moveBox[1], glm::max(pos, moveBox[0]));
			entity->Position(pos);


			entity->Rotation(glm::quat(glm::vec3(0, 0, rotZ)));

			//弾の発射処理
			if (gamepad.buttons & GamePad::A) {
				shotInterval -= delta;
				float tmp = static_cast<float>(std::abs(shotInterval));

				if (shotInterval <= 0) {

					glm::vec3 pos = entity->Position();
					float bulletInterval = 1.0f;
					glm::vec3 leftPos = glm::vec3(pos.x - (multiShotNum - 1) / 2 * bulletInterval, pos.y, pos.z);

					game.PlayAudio(0, 0);
					for (int i = 0; i < multiShotNum; ++i) {

						if (Entity::Entity* p = game.AddEntity(EntityGroupId_PlayerShot, leftPos + glm::vec3(i*bulletInterval, 0, 0),
							"NormalShot", "Res/Model/Player.dds", std::make_shared<PlayerShot>(), "NonLighting")) {

							p->Velocity(glm::vec3(0, 0, 200));
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
	}

	/**
	*	プレイヤーの衝突判定処理
	*
	void Player::CollisionEnter(Entity::Entity& entity) {

		if (auto i = entity.CastTo<Item>()) {

			if (i->ItemType() == 1) {
				moveSpeed = glm::min(10.0f, moveSpeed + 2);
			}
			else {

				multiShotNum = glm::min(5, multiShotNum + 1);

			}
		}
		else if (entity.CastTo<Toroid>()) {

			if (damageTimer <= 0) {

				Initialize();
				damageTimer = 4.0f;
			}

		}
	}*/

	/// 敵クラスの定義

	/**
	*	敵の初期化処理
	*/
	void Toroid::Initialize() {
		entity->CastShadow(false);
		entity->CastStencil(true);	//TODO : ステンシルマスクのテスト用

		//auto p = GameEngine::Instance().FindEntityData<Player>();
	}

	/**
	*	敵の更新処理
	*/
	void Toroid::Update(double delta) {

		timer += static_cast<float>(delta);

		float rot = glm::angle(entity->Rotation());
		const glm::vec3 pos = entity->Position();

		GameEngine& game = GameEngine::Instance();


		// 移動処理
		switch (enemyType) {
		case 0:	/// デフォルトの処理まっすぐ進む

			entity->Velocity(glm::vec3(0, 0, -10));


			break;

		case 1:	/// 蛇行して下方向へ
		{
			float velX = glm::cos(timer * 2) * 10 - entity->Velocity().x;
			glm::vec3 vel = glm::vec3(velX, 0, -5);
			entity->Velocity(vel);

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
		entity->Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		//画面外判定処理
		if (std::abs(pos.x) > 200.0f || std::abs(pos.z) > 200.0f) {
			GameEngine::Instance().RemoveEntity(entity);
			return;
		}
	}

	void Toroid::CollisionEnter(Entity::Entity& e) {

		if (--hp <= 0) {

			GameEngine& game = GameEngine::Instance();

			//爆発エフェクト
			if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, entity->Position(), "Blast", "Res/Model/Toroid.dds", std::make_shared<Blast>())) {
				const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
				p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
				p->Color(glm::vec4(1.0f, 0.75f, 0.5f, 1.0f));
				game.UserVariable("score") += 100;
			}
		
			if (isItemDrop) {
				
				int itemID = rand()  % 2;

				//アイテム
				if (Entity::Entity* p = game.AddEntity(EntityGroupId_Item, entity->Position(), "ItemBox", "Res/Model/ItemBox.dds", std::make_shared<Item>(itemID))) {//"ItemBox", "Res/Model/ItemBox.dds", std::make_shared<Item>())) {
					p->Collision(collisionDataList[EntityGroupId_Item]);
				}
			}

			//爆発音
			game.PlayAudio(1, CRI_SAMPLECUESHEET_BOMB);

			entity->Destroy();
		}

	}

	/// 敵スポナーのクラス定義

	/**
	*	敵スポナーの更新処理
	*/
	void EnemyLaunchType::Update(double delta) {

		GameEngine& game = GameEngine::Instance();

		time += static_cast<float>(delta);

		if (time >= spawnMax * spawnInterval) {
			//スポーン終了

			entity->Destroy();
			return;
		}

		//敵の出撃処理
		if (launchIndex < static_cast<int>(time / spawnInterval)) {

			bool isItemDrop = spawnMax == (launchIndex + 2);

			Entity::Entity* p = game.AddEntity(EntityGroupId_Enemy, entity->Position(),
				"Toroid", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", std::make_shared<Toroid>(0,isItemDrop));

			p->Collision(collisionDataList[EntityGroupId_Enemy]);

			launchIndex++;
		}
	}


	///メインゲーム画面のクラス定義

	/**
	*	メインゲーム画面のコンストラクタ
	*/
	MainGame::MainGame() {

		GameEngine& game = GameEngine::Instance();
		game.ClearCollisionHandlerList();
		game.CollisionHandler(EntityGroupId_PlayerShot, EntityGroupId_Enemy, nullptr);
		game.CollisionHandler(EntityGroupId_Player, EntityGroupId_Enemy,nullptr);
		game.CollisionHandler(EntityGroupId_Player, EntityGroupId_Item, nullptr);
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
			game.LoadTextureFromFile("Res/Model/ItemBox.dds");

			switch (stageNo % 3) {
			case 1: {

				stageTimer = 10;

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

				//背景の更新処理
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
				"Aircraft", "Res/Model/Player.dds", std::make_shared<Player>());
			playerEntity->Collision(collisionDataList[EntityGroupId_Player]);

			playerData = playerEntity->CastTo<Player>();

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

			if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, glm::vec3(0, 0, 50),
				"Res/Model/Toroid.fbx", "Res/Model/Player.dds", std::make_shared<EnemyLaunchType>())) {

				//p->Velocity(glm::vec3(pos.x < 0 ? 0.5f : -0.5f, 0, -1.0f));
				p->Collision(collisionDataList[EntityGroupId_Others]);
			}

			interval = 5;

		}

		//スコア表示処理
		char str[16];
		snprintf(str, 16, "%08.0f", game.UserVariable("score"));
		game.FontScale(glm::vec2(3.0f));
		game.FontColor(glm::vec4(1, 0, 0, 1));
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
