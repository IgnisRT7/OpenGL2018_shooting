/**
*	@file Entity.h
*/
#pragma once

#include "../../Entity.h"
#include "../BulletManager.h"
#include "../EnemyMoveController.h"

namespace GameState {

	/// 敵
	class Toroid : public Entity::EntityDataBase {
	public:

		Toroid(int typeID = 0, bool itemDrop = false) :
			enemyType(typeID), isItemDrop(itemDrop) {}

		void Initialize() override;
		void Update(float delta) override;
		void Damage(float) override;
		void CollisionEnter(Entity::Entity& e) override;

		void MoveController(MoveControllerPtr m) { moveController = m; }
		void BulletManager(BulletManagerPtr b) { bulletManager = b; }
		void Target(Entity::Entity* t);

	private:

		float timer = 0;	/// 
		int enemyType = 0;	/// 移動タイプ
		int hp = 2;			/// 体力

		bool isItemDrop = false;

		BulletManagerPtr bulletManager;		///< 弾の管理クラス
		MoveControllerPtr moveController;	///< 移動管理クラス
		
		Entity::Entity* playerEntity;
	};

	class BossEnemy : public Entity::EntityDataBase {
	public:

		void Initialize() override;
		void Update(float delta) override;
		void Damage(float) override;
		void CollisionEnter(Entity::Entity& e) override;

	private:

		std::vector<BulletManagerPtr> bulletsManager;
	};

	/// 敵スポナー
	class EnemySpawner : public Entity::EntityDataBase {
	public:

		EnemySpawner() {}
		EnemySpawner(int max, float interval, int enemyType,int bulletType);
		void Initialize() override;
		void Update(float delta) override;

		void SpawnEnemy();

	private:

		float spawnInterval = 0.5;	///< スポーンする間隔
		float spawnMax = 5;			///< スポーン数
		float time = 0;				///< 経過時間
		int launchIndex = -1;		///< 出撃している敵の数
		int enemyType = -1;			///< 出撃する敵の種類
		int bulletType = -1;		///< 弾の種類 

		Entity::Entity* playerEntity;
	};





}