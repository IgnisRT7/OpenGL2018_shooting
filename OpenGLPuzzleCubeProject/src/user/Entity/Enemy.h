/**
*	@file Entity.h
*/
#pragma once

#include "../../Entity.h"

namespace GameState {

	class EnemyBulletManager;
	using EnemyBulletManagerPtr = std::shared_ptr<EnemyBulletManager>;

	/// “G
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
		int enemyType = 0;	/// ˆÚ“®ƒ^ƒCƒv
		int hp = 2;			/// ‘Ì—Í

		bool isItemDrop = false;

		std::shared_ptr<EnemyBulletManager> bulletManager;	/// ’e‚ÌŠÇ—ƒNƒ‰ƒX
	};

	/// “GƒXƒ|ƒi[
	class EnemySpawner : public Entity::EntityDataBase {
	public:

		EnemySpawner() {}
		EnemySpawner(int max, float interval, int enemyType,int bulletType);
		void Initialize() override {}

		void Update(float delta) override;

		float spawnInterval = 0.5;	///< ƒXƒ|[ƒ“‚·‚éŠÔŠu
		float spawnMax = 5;			///< ƒXƒ|[ƒ“”
		float time = 0;				///< Œo‰ßŠÔ
		int launchIndex = -1;		///< oŒ‚‚µ‚Ä‚¢‚é“G‚Ì”
		int enemyType = -1;			///< oŒ‚‚·‚é“G‚Ìí—Ş
		int bulletType = -1;		///< ’e‚Ìí—Ş 
	};

	///“G‚Ì’e‚ÌŠÇ—ƒVƒXƒeƒ€
	class EnemyBulletManager {
	public:
		
		EnemyBulletManager(Entity::Entity& p, Entity::Entity* t = nullptr);

		virtual void Update(float delta);

	protected:
		float timer = 0;
		float shotInterval = 2;		/// ’e‚Ì”­ËŠÔŠu
		
		Entity::Entity& parent;		/// ’e‚Ì”­ËŒ³
		Entity::Entity* target;		/// ’e‚Ì’Ç”ö‘ÎÛ
	};

	class EnemyFiveWayBullet : public EnemyBulletManager {
	public:
		EnemyFiveWayBullet(Entity::Entity& p, Entity::Entity* t = nullptr) :EnemyBulletManager(p, t) {}

		void Update(float delta) override;
	};


}