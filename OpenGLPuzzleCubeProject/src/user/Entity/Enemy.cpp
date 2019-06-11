/**
*	@file Entity.cpp
*/
#include "Enemy.h"
#include "../../GameEngine.h"
#include "../../GameState.h"
#include "../../../Res/Audio/testProject_acf.h"
#include "../../../Res/Audio/CueSheet_0.h"
#include "../EnemyLaunchController.h"

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
		entity->Scale(glm::vec3(1.5f));

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
		if (bulletManager) {
			bulletManager->Update(delta);
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
		const glm::vec3 pos = entity->Position();
		if (std::abs(pos.x) > screenHalfW * 1.2 || std::abs(pos.z) > screenHalfH * 1.2) {

			std::cout << "enemy of outrange" << std::endl;
			GameEngine::Instance().RemoveEntity(entity);
			return;
		}
		//std::cout << "pos: " << entity->Position().x << "," << entity->Position().y << "," << entity->Position().z << std::endl;
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

		if (bulletManager) {
 			bulletManager->Target(t);
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
	EnemySpawner::EnemySpawner(int max, float interval, int eType,int mType, int bType) :
		spawnMax(max), spawnInterval(interval), enemyType(eType),moveType(mType), bulletType(bType) {
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

		//敵本体の作成
		auto& t = std::make_shared<Toroid>(0, isItemDrop);
		
		t->MoveController(MakeMoveControllerByMoveType(moveType,entity->Position().x < 0));

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
				t->BulletManager(std::make_shared<BulletManager>(*p,EntityGroupId_EnemyShot, playerEntity));
				break;
			case 5:
				t->BulletManager(std::make_shared<CircleShot>(*p, EntityGroupId_EnemyShot));
			default:
				break;
			}
		}
		t->Target(playerEntity);

	}
}
