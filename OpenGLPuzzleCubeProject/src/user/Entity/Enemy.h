/**
*	@file Entity.h
*/
#pragma once

#include "../../Entity.h"
#include "../BulletManager.h"

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

		void BulletManager(EnemyBulletManagerPtr b) { bulletManager = b; }
		void Target(Entity::Entity* t);

	private:

		float timer = 0;	/// 
		int enemyType = 0;	/// �ړ��^�C�v
		int hp = 2;			/// �̗�

		bool isItemDrop = false;

		EnemyBulletManagerPtr bulletManager;	/// �e�̊Ǘ��N���X
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
	};





}