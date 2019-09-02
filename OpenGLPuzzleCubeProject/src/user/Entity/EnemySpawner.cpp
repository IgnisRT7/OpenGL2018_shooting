/**
*	@file EnemySpawner.cpp
*/

#include "EnemySpawner.h"

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
		auto& t = std::make_shared<Toroid>(0, health, isItemDrop);

		//移動タイプに基づいて移動データを設定する
		t->MoveController(MakeMoveControllerByMoveType(moveType, entity->Position().x < 0));

		//スポナーの位置からスポーン地点の設定
		glm::vec3 pos = entity->Position();
		if (moveType == 2) {
			pos.z -= launchIndex * 5.0f;
		}

		Entity::Entity* p = game.AddEntity(EntityGroupId_Enemy, pos,
			"Toroid", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", t);

		//弾のタイプに基づいて弾生成機を作成
		if (bulletType != -1) {

			switch (bulletType) {
			case 1:
				t->BulletGenerator(std::make_shared<NormalShot>(*p, EntityGroupId_EnemyShot, playerEntity));
				break;
			case 2:
				t->BulletGenerator(std::make_shared<MultiWayShot>(*p, EntityGroupId_EnemyShot, nullptr));
				t->BulletGenerator()->Color(glm::vec4(0.3f, 1.0f, 0.5f, 1.0f));
				break;
			case 5:
				t->BulletGenerator(std::make_shared<CircleShot>(*p, EntityGroupId_EnemyShot));
				t->BulletGenerator()->Color(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
			default:
				break;
			}
		}
		t->Target(playerEntity);

	}



}
