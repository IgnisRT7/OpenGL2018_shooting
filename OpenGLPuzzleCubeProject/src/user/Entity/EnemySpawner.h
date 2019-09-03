/**
*	@file	EnemySpawner
*	@brief	艦隊単位での敵のスポナー管理
*	@author	Takuya Yokoyama
*/

#pragma once

#include "../../Engine/Entity.h"
#include "../../Engine/GameEngine.h"
#include "Player.h"
#include "Enemy.h"
#include "../EnemyLaunchController.h"

namespace Application {

	/// 敵スポナー
	class EnemySpawner : public Entity::EntityDataBase {
	public:

		EnemySpawner() {}
		EnemySpawner(int max, float interval, int enemyType, int moveType, int bulletType, int health);
		void Initialize() override;
		void Update(float delta) override;

		void SpawnEnemy();

	private:

		float spawnInterval = 0.5;	///< スポーンする間隔
		float time = 0;				///< 経過時間
		int spawnMax = 5;			///< スポーン数
		int launchIndex = 0;		///< 出撃している敵の数
		int enemyType = -1;			///< 出撃する敵の種類
		int moveType = -1;
		int bulletType = -1;		///< 弾の種類 
		int health = 1;				///< 体力

		Entity::Entity* playerEntity;
	};

}