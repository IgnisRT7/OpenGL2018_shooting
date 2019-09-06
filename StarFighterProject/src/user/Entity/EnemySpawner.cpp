/**
*	@file EnemySpawner.cpp
*/

#include "EnemySpawner.h"
#include "../../../Res/Resource.h"

namespace Application {

	EnemySpawner::EnemySpawner(int max, float interval, int eType, int mType, int bType, int health) :
		spawnMax(max), spawnInterval(interval), enemyType(eType),
		moveType(mType), bulletType(bType), health(health) {
	}

	void EnemySpawner::Initialize() {
		playerEntity = GameEngine::Instance().FindEntityData<Player>();
	}

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

	void EnemySpawner::SpawnEnemy() {

		GameEngine& game = GameEngine::Instance();

		bool isItemDrop = spawnMax == (launchIndex + 1);	///最後に出撃する敵のみアイテムドロップする

		if (enemyType == 5) {
			//boss

			auto& b = std::make_shared<BossEnemy>();
			b->MoveController(MakeMoveControllerByMoveType(-1, false));
			b->Target(playerEntity);

			Entity::Entity* p = game.AddEntity(GameState::EntityGroupId_Enemy, entity->Position(),
				"MotherShip", Resource::tex_toroid, Resource::tex_toroid_normal, b);
		}
		else {

			//敵本体の作成
			auto& t = std::make_shared<Toroid>(0, health, isItemDrop);

			//移動タイプに基づいて移動データを設定する
			t->MoveController(MakeMoveControllerByMoveType(moveType, entity->Position().x < 0));

			//スポナーの位置からスポーン地点の設定
			glm::vec3 pos = entity->Position();
			if (moveType == 2) {
				pos.z -= launchIndex * 5.0f;
			}

			Entity::Entity* p = game.AddEntity(GameState::EntityGroupId_Enemy, pos,
				"Toroid", Resource::tex_toroid, Resource::tex_toroid_normal, t);

			//弾のタイプに基づいて弾生成機を作成
			if (bulletType != -1) {

				switch (bulletType) {
				case 1:
					t->BulletGenerator(std::make_shared<NormalShot>(*p, GameState::EntityGroupId_EnemyShot, playerEntity));
					break;
				case 2:
					t->BulletGenerator(std::make_shared<MultiWayShot>(*p, GameState::EntityGroupId_EnemyShot, nullptr));
					t->BulletGenerator()->Color(glm::vec4(0.3f, 1.0f, 0.5f, 1.0f));
					break;
				case 5:
					t->BulletGenerator(std::make_shared<CircleShot>(*p, GameState::EntityGroupId_EnemyShot));
					t->BulletGenerator()->Color(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
				default:
					break;
				}
			}
			t->Target(playerEntity);

		}

	}

}