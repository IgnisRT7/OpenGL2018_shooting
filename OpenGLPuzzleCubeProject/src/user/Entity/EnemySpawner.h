/**
*	@file	EnemySpawner
*	@brief	�͑��P�ʂł̓G�̃X�|�i�[�Ǘ�
*	@author	Takuya Yokoyama
*/

#pragma once

#include "../../Engine/Entity.h"
#include "../../Engine/GameEngine.h"
#include "Player.h"
#include "Enemy.h"
#include "../EnemyLaunchController.h"

namespace Application {

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