/**
*	@file Entity.h
*/
#pragma once

#include "../../Entity.h"

namespace GameState {

	class EnemyBulletManager;

	/// �G
	class Toroid : public Entity::EntityDataBase {
	public:

		Toroid(int typeID = 0, bool itemDrop = false) :
			enemyType(typeID), isItemDrop(itemDrop) {}

		void Initialize() override;

		void Update(double delta) override;

		void Damage(float) override;

		void CollisionEnter(Entity::Entity& e) override;

	private:

		float timer = 0;
		int enemyType = 0;
		int hp = 2;

		bool isItemDrop = false;

		std::shared_ptr<EnemyBulletManager> bulletManager;
	};

	/// �G�X�|�i�[
	class EnemyLaunchType : public Entity::EntityDataBase {
	public:

		void Initialize() override {}

		void Update(double delta) override;

		float spawnInterval = 0.5;	//�X�|�[������Ԋu
		float spawnMax = 5;			//�X�|�[����
		float time;					//�o�ߎ���
		int launchIndex = -1;		//�o�����Ă���G�̐�
		int enemyType = -1;			//�o������G�̎��
	};

	///�G�̒e�̊Ǘ��V�X�e��
	class EnemyBulletManager {
	public:
		
		EnemyBulletManager(Entity::Entity& p, Entity::Entity* t = nullptr) :
			parent(p),target(t),timer(shotInterval){}

		void Update(double delta);


	private:
		float timer = 0;
		float shotInterval = 2;
		
		Entity::Entity& parent;
		Entity::Entity* target;
	};



}