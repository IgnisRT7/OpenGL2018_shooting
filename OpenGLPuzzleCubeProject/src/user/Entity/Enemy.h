/**
*	@file Entity.h
*/
#pragma once

#include "../../Entity.h"
#include "../BulletManager.h"
#include "../EnemyMoveController.h"

namespace GameState {

	/// �G
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
		int enemyType = 0;	/// �ړ��^�C�v
		int hp = 2;			/// �̗�

		bool isItemDrop = false;

		BulletManagerPtr bulletManager;		///< �e�̊Ǘ��N���X
		MoveControllerPtr moveController;	///< �ړ��Ǘ��N���X
		
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

	/// �G�X�|�i�[
	class EnemySpawner : public Entity::EntityDataBase {
	public:

		EnemySpawner() {}
		EnemySpawner(int max, float interval, int enemyType,int bulletType);
		void Initialize() override;
		void Update(float delta) override;

		void SpawnEnemy();

	private:

		float spawnInterval = 0.5;	///< �X�|�[������Ԋu
		float spawnMax = 5;			///< �X�|�[����
		float time = 0;				///< �o�ߎ���
		int launchIndex = -1;		///< �o�����Ă���G�̐�
		int enemyType = -1;			///< �o������G�̎��
		int bulletType = -1;		///< �e�̎�� 

		Entity::Entity* playerEntity;
	};





}