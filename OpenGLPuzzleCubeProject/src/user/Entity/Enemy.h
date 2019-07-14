/**
*	@file Enemy.h
*/
#pragma once

#include "../../Engine/Entity.h"
#include "../BulletGenerator.h"
#include "../EnemyMoveController.h"

namespace GameState {

	/// �G
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
		int enemyType = 0;	/// �ړ��^�C�v
		int hp = 2;			/// �̗�

		bool isItemDrop = false;
		bool isDestroyByScreenOut = true;
		glm::vec4 bulletColor = glm::vec4(1);

		BulletGeneratorPtr bulletGenerator;	///< �e�̐����Ǘ��N���X
		MoveControllerPtr moveController;	///< �ړ��Ǘ��N���X
		
		Entity::Entity* playerEntity;
	};
	
	//�{�X�p�G
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
		int hp = 0;		/// �̗�
		const int maxHp = 1500;

		MoveControllerPtr moveController;
		std::vector<std::shared_ptr<Toroid> > turrets;

		Entity::Entity* playerEntity;
	};

	/// �G�X�|�i�[
	class EnemySpawner : public Entity::EntityDataBase {
	public:

		EnemySpawner() {}
		EnemySpawner(int max, float interval, int enemyType, int moveType, int bulletType, int health);
		void Initialize() override;
		void Update(float delta) override;

		void SpawnEnemy();

	private:

		float spawnInterval = 0.5;	///< �X�|�[������Ԋu
		float time = 0;				///< �o�ߎ���
		int spawnMax = 5;			///< �X�|�[����
		int launchIndex = 0;		///< �o�����Ă���G�̐�
		int enemyType = -1;			///< �o������G�̎��
		int moveType = -1;
		int bulletType = -1;		///< �e�̎�� 
		int health = 1;				///< �̗�

		Entity::Entity* playerEntity;
	};





}