/**
*	@file Entity.h
*/
#pragma once

#include "../../Entity.h"

namespace GameState {

	class EnemyBulletManager;
	using EnemyBulletManagerPtr = std::shared_ptr<EnemyBulletManager>;

	/// �G
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
		int enemyType = 0;	/// �ړ��^�C�v
		int hp = 2;			/// �̗�

		bool isItemDrop = false;

		std::shared_ptr<EnemyBulletManager> bulletManager;	/// �e�̊Ǘ��N���X
	};

	/// �G�X�|�i�[
	class EnemySpawner : public Entity::EntityDataBase {
	public:

		EnemySpawner() {}
		EnemySpawner(int max, float interval, int enemyType,int bulletType);
		void Initialize() override {}

		void Update(float delta) override;

		float spawnInterval = 0.5;	///< �X�|�[������Ԋu
		float spawnMax = 5;			///< �X�|�[����
		float time = 0;				///< �o�ߎ���
		int launchIndex = -1;		///< �o�����Ă���G�̐�
		int enemyType = -1;			///< �o������G�̎��
		int bulletType = -1;		///< �e�̎�� 
	};

	///�G�̒e�̊Ǘ��V�X�e��
	class EnemyBulletManager {
	public:
		
		EnemyBulletManager(Entity::Entity& p, Entity::Entity* t = nullptr);

		virtual void Update(float delta);

	protected:
		float timer = 0;
		float shotInterval = 2;		/// �e�̔��ˊԊu
		
		Entity::Entity& parent;		/// �e�̔��ˌ�
		Entity::Entity* target;		/// �e�̒ǔ��Ώ�
	};

	class EnemyFiveWayBullet : public EnemyBulletManager {
	public:
		EnemyFiveWayBullet(Entity::Entity& p, Entity::Entity* t = nullptr) :EnemyBulletManager(p, t) {}

		void Update(float delta) override;
	};


}