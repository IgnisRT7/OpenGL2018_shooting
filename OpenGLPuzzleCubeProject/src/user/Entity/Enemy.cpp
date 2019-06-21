/**
*	@file Entity.cpp
*/
#include "Enemy.h"
#include "../../Engine/GameEngine.h"
#include "../../GameState.h"
#include "../../../Res/Audio/testProject_acf.h"
#include "../../../Res/Audio/CueSheet_0.h"
#include "../EnemyLaunchController.h"
#include "../GameMainScene.h"

#include "Effect.h"
#include "Item.h"
#include "Bullet.h"
#include "Player.h"
#include <memory>

namespace GameState {



	/**
	*	敵の初期化処理
	*/
	void Toroid::Initialize() {
		entity->CastShadow(true);
		entity->CastStencil(true);	//TODO : ステンシルマスクのテスト用
		entity->StencilColor(glm::vec4(1, 0, 1, 1));
		entity->Scale(glm::vec3(1.5));

		entity->Collision(collisionDataList[EntityGroupId_Enemy]);

		return;
	}

	/**
	*	敵の更新処理
	*
	*	@param delta	経過時間
	*/
	void Toroid::Update(float delta) {

		timer += delta;

		//弾管理システムの更新
		if (bulletGenerator) {
			//bulletGenerator->Color(bulletColor);
			bulletGenerator->Update(delta);
		}

		//移動管理システムの更新
		if (moveController) {
			moveController->Update(*entity, delta);
		}

		// 円盤を回転させる.
		float rot = glm::angle(entity->Rotation());
		rot += glm::radians(180.0f) * delta;
		if (rot > glm::pi<float>() * 2.0f) {
			rot -= glm::pi<float>() * 2.0f;
		}
		entity->Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		//画面外判定処理
		if (isDestroyByScreenOut) {
			const glm::vec3 pos = entity->Position();
			if (std::abs(pos.x) > screenHalfW * 1.2 || std::abs(pos.z) > screenHalfH * 1.2) {

				std::cout << "enemy of outrange" << std::endl;
				GameEngine::Instance().RemoveEntity(entity);
				return;
			}
		}
	}

	/**
	*	ダメージ処理
	*
	*	@param p	ダメージ量
	*/
	void Toroid::Damage(float p) {

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
				//アイテムドロップ処理

				int itemID = rand() % 2;

				//アイテム
				std::string texName = itemID ? "Res/Model/ItemBoxSpeed.dds" : "Res/Model/ItemBoxBullet.dds";

				if (Entity::Entity* p = game.AddEntity(EntityGroupId_Item, entity->Position(), "ItemBox", texName.c_str(), std::make_shared<Item>(itemID))) {
					p->Collision(collisionDataList[EntityGroupId_Item]);
				}
			}

			entity->Destroy();
		}
	}

	/**
	*	衝突判定処理
	*
	*	@param e	衝突してきたエンティティ
	*/
	void Toroid::CollisionEnter(Entity::Entity& e) {
		e.EntityData()->Damage(1);

		//GameEngine::Instance().PlayAudio(1, CRI_CUESHEET_0_EXPLOSIVE);
	}

	/**
	*	ターゲットの設定
	*/
	void Toroid::Target(Entity::Entity * t) {

		playerEntity = t;

		if (bulletGenerator) {
			bulletGenerator->Target(t);
		}
	}

	/**
	*	初期化処理
	*/
	void BossEnemy::Initialize() {

		GameEngine& game = GameEngine::Instance();

		entity->Scale(glm::vec3(11));
		entity->Collision({ { -15,-20,-10},{15,20,10} });

		for (int i = 0; i < 8; i++) {

			auto t = std::make_shared<Toroid>();
			t->DestroyByScreenOut(false);

			if (auto p = game.AddEntity(EntityGroupId_Others, glm::vec3(-60),
				"", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", t)) {

				turrets.push_back(t);

				//タレット用弾作成
				BulletGeneratorPtr b;
				if ((i % 2) == 0) {
					b = std::make_shared<MultiWayShot>(*p, EntityGroupId_EnemyShot, nullptr, 15, 3);
				}
				else {
					b = std::make_shared<NormalShot>(*p, EntityGroupId_EnemyShot, playerEntity);
				}

				t->BulletGenerator(b);
				b->Color(glm::vec4(0, 0.2, 1, 1));
			}
		}


	}

	/**
	*	更新処理
	*
	*	@param delta	経過時間
	*/
	void BossEnemy::Update(float delta) {

		timer += delta;

		//移動管理システムの更新
		if (moveController) {
			moveController->Update(*entity, delta);
		}

		// 円盤を回転させる.
		float rot = glm::angle(entity->Rotation());
		rot += glm::radians(30.0f) * delta;
		if (rot > glm::pi<float>() * 2.0f) {
			rot -= glm::pi<float>() * 2.0f;
		}
		entity->Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		//タレットの位置情報更新処理
		UpdateTurret();
	}

	/**
	*	ダメージ処理
	*
	*	@param p	ダメージ量
	*/
	void BossEnemy::Damage(float p) {

		if (--hp <= 0) {

			GameEngine& game = GameEngine::Instance();

			//爆発エフェクト
			if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, entity->Position(), "Blast", "Res/Model/Toroid.dds", std::make_shared<Blast>())) {
				const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
				p->Scale(glm::vec3(10.0f));
				p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
				p->Color(glm::vec4(1.0f, 0.75f, 0.5f, 1.0f));
				game.UserVariable("score") += 100;

				game.PlayAudio(0, CRI_CUESHEET_0_EXPLOSIVE);

				for (auto turret : turrets) {
					turret->Entity()->Destroy();
				}
				dynamic_cast<MainGame*>(&SceneStack::Instance().Current())->StageClear(5);
			}

			entity->Destroy();
		}
		else {

			if (static_cast<float>(hp) / maxHp > 0.5) {
				for (auto turret : turrets) {

					turret->BulletGenerator()->BulletSpeed(20.0);
					turret->BulletGenerator()->ShotInterval(2.0f);
 					turret->BulletGenerator()->Color(glm::vec4(0, 0.2, 1, 1));
				}
			}
			else if(static_cast<float>(hp) / maxHp > 0.3){
				for (auto turret : turrets) {

					turret->BulletGenerator()->BulletSpeed(15.0);
					turret->BulletGenerator()->ShotInterval(1.0f);
					turret->BulletGenerator()->Color(glm::vec4(1, 1, 0, 1));
				}
			}
			else {
				for (auto turret : turrets) {
					std::shared_ptr<MultiWayShot> b = std::dynamic_pointer_cast<MultiWayShot>(turret->BulletGenerator());

					turret->BulletGenerator()->BulletSpeed(15.0);
					
					if (b) {
						b->MaxBulletNum(5);
						b->ShotInterval(0.5f);
					}
					else {
						turret->BulletGenerator()->ShotInterval(0.7);
					}
					turret->BulletGenerator()->Color(glm::vec4(1, 0, 0, 1));
				}
			}

		}
	}

	void BossEnemy::CollisionEnter(Entity::Entity &e) {

		e.EntityData()->Damage(1);
	}

	/**
	*	タレットの更新処理
	*/
	void BossEnemy::UpdateTurret() {

		glm::quat baseRot = entity->Rotation();
		glm::vec3 centerPos = entity->Position();
		glm::vec3 forward = glm::vec3(-1, 0, 0);

		for (int i = 0; i < 8; i++) {

			const float perRad = glm::pi<float>() * 2.0f * (1.0f / 8.0f);
			float rot = glm::pi<float>() * 2.0f * 0.125f * i;
			float offset = 0.15f;
			glm::quat tmp = glm::angleAxis(rot + offset, glm::vec3(0, 1, 0));

			tmp = tmp * baseRot;

			glm::vec3 vec = tmp * (forward * 17.0f);

			glm::vec3 offsetPos = glm::vec3(0, 5, 0);

			turrets[i]->Entity()->Position(centerPos + vec + offsetPos);
			turrets[i]->BulletGenerator()->Rotation(tmp);
		}
	}

	/**
	*	ターゲットの設定
	*
	*	@param t	ターゲットのエンティティ
	*/
	void BossEnemy::Target(Entity::Entity* t) {

		playerEntity = t;
		for (auto turret : turrets) {
			if (turret) {
				turret->Target(t);
			}
		}
	}

	/// 敵スポナーのクラス定義

	/**
	*	コンストラクタ
	*
	*	@param max	スポーン数
	*	@param interval	出撃間隔
	*	@param eType	敵のタイプ
	*	@param mType	移動タイプ
	*	@param bType	弾のタイプ
	*/
	EnemySpawner::EnemySpawner(int max, float interval, int eType, int mType, int bType) :
		spawnMax(max), spawnInterval(interval), enemyType(eType), moveType(mType), bulletType(bType) {
	}

	/**
	*	初期化処理
	*/
	void EnemySpawner::Initialize() {
		playerEntity = GameEngine::Instance().FindEntityData<Player>();
	}

	/**
	*	敵スポナーの更新処理
	*
	*	@param delta	経過時間
	*/
	void EnemySpawner::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		time += delta;

		//敵の出撃処理
		if (launchIndex < static_cast<int>(time / spawnInterval)) {
			SpawnEnemy();
			launchIndex++;
		}

		if (time >= spawnMax * spawnInterval) {
			//スポーン終了

			entity->Destroy();
			return;
		}
	}

	/**
	*	スポーン処理
	*/
	void EnemySpawner::SpawnEnemy() {

		GameEngine& game = GameEngine::Instance();

		bool isItemDrop = spawnMax == (launchIndex + 1);	///最後に出撃する敵のみアイテムドロップする

		if (enemyType == 5) {
			//boss

			auto& b = std::make_shared<BossEnemy>();
			b->MoveController(MakeMoveControllerByMoveType(-1, false));
			b->Target(playerEntity);

			Entity::Entity* p = game.AddEntity(EntityGroupId_Enemy, entity->Position(),
				"MotherShip", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", b);

		}
		else {

			//敵本体の作成
			auto& t = std::make_shared<Toroid>(0, isItemDrop);

			t->MoveController(MakeMoveControllerByMoveType(moveType, entity->Position().x < 0));

			glm::vec3 pos = entity->Position();
			if (moveType == 2) {
				pos.z -= launchIndex * 5.f;
			}

			Entity::Entity* p = game.AddEntity(EntityGroupId_Enemy, pos,
				"Toroid", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", t);

			//弾管理の作成
			if (bulletType != -1) {

				switch (bulletType) {
				case 1:
					t->BulletGenerator(std::make_shared<NormalShot>(*p, EntityGroupId_EnemyShot, playerEntity));
					break;
				case 5:
					t->BulletGenerator(std::make_shared<CircleShot>(*p, EntityGroupId_EnemyShot));
				default:
					break;
				}
			}
			t->Target(playerEntity);

		}



	}


}
