/**
*	@file Entity.h
*/
#pragma once

#include "../../Entity.h"

namespace GameState {

	class EnemyBulletManager;
	using EnemyBulletManagerPtr = std::shared_ptr<EnemyBulletManager>;

	/// 敵
	class Toroid : public Entity::EntityDataBase {
	public:

		Toroid(int typeID = 0, bool itemDrop = false) :
			enemyType(typeID), isItemDrop(itemDrop) {}

		void Initialize() override;
		void Update(float delta) override;
		void Damage(float) override;
		void CollisionEnter(Entity::Entity& e) override;

		void BulletManager(EnemyBulletManagerPtr b) { bulletManager = b; }

	private:

		float timer = 0;	/// 
		int enemyType = 0;	/// 移動タイプ
		int hp = 2;			/// 体力

		bool isItemDrop = false;

		std::shared_ptr<EnemyBulletManager> bulletManager;	/// 弾の管理クラス
	};

	/// 敵スポナー
	class EnemySpawner : public Entity::EntityDataBase {
	public:

		EnemySpawner() {}
		EnemySpawner(int max, float interval, int enemyType,int bulletType);
		void Initialize() override {}

		void Update(float delta) override;

		float spawnInterval = 0.5;	///< スポーンする間隔
		float spawnMax = 5;			///< スポーン数
		float time = 0;				///< 経過時間
		int launchIndex = -1;		///< 出撃している敵の数
		int enemyType = -1;			///< 出撃する敵の種類
		int bulletType = -1;		///< 弾の種類 
	};

	///敵の弾の管理システム
	class EnemyBulletManager {
	public:
		
		EnemyBulletManager(Entity::Entity& p, Entity::Entity* t = nullptr);

		virtual void Update(float delta);

	protected:
		float timer = 0;
		float shotInterval = 2;		/// 弾の発射間隔
		
		Entity::Entity& parent;		/// 弾の発射元
		Entity::Entity* target;		/// 弾の追尾対象
	};

	class EnemyFiveWayBullet : public EnemyBulletManager {
	public:
		EnemyFiveWayBullet(Entity::Entity& p, Entity::Entity* t = nullptr) :EnemyBulletManager(p, t) {}

		void Update(float delta) override;
	};


}