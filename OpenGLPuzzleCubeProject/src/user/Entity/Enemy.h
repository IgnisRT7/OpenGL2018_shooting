/**
*	@file Enemy.h
*/
#pragma once

#include "../../Engine/Entity.h"
#include "../BulletGenerator.h"
#include "../EnemyMoveController.h"

namespace GameState {

	/// 敵
	class Toroid : public Entity::EntityDataBase {
	public:

		Toroid(int typeID = 0,int health = 2, bool itemDrop = false) :
			enemyType(typeID), isItemDrop(itemDrop),hp(health) {}

		void Initialize() override;
		void Update(float delta) override;
		void Damage(float) override;
		void CollisionEnter(Entity::Entity& e) override;

		void MoveController(MoveControllerPtr m) { moveController = m; }
		void BulletGenerator(BulletGeneratorPtr b) { bulletGenerator = b; }
		BulletGeneratorPtr BulletGenerator() const { return bulletGenerator; }
		void DestroyByScreenOut(bool v) { isDestroyByScreenOut = v; }
		void Target(Entity::Entity* t);
		void BulletColor(glm::vec4 c) { bulletColor = c; }
		Entity::Entity* Entity() { return entity; }

	private:

		float timer = 0;	/// 
		int enemyType = 0;	/// 移動タイプ
		int hp = 2;			/// 体力

		bool isItemDrop = false;
		bool isDestroyByScreenOut = true;
		glm::vec4 bulletColor = glm::vec4(1);

		BulletGeneratorPtr bulletGenerator;	///< 弾の生成管理クラス
		MoveControllerPtr moveController;	///< 移動管理クラス
		
		Entity::Entity* playerEntity;
	};
	
	//ボス用敵
	class BossEnemy : public Entity::EntityDataBase {
	public:

		void Initialize() override;
		void Update(float delta) override;
		void Damage(float) override;
		void CollisionEnter(Entity::Entity& e) override;

		void UpdateTurret();
		void MoveController(MoveControllerPtr m) { moveController = m; }
		void Target(Entity::Entity* t);

	private:

		float timer = 0;	/// 
		int hp = 0;		/// 体力
		const int maxHp = 1500;

		MoveControllerPtr moveController;
		std::vector<std::shared_ptr<Toroid> > turrets;

		Entity::Entity* playerEntity;
	};

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