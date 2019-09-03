/**
*	@file EnemySpawner.cpp
*/

#include "EnemySpawner.h"

namespace Application {

	/**
*	�R���X�g���N�^
*
*	@param max	�X�|�[����
*	@param interval	�o���Ԋu
*	@param eType	�G�̃^�C�v
*	@param mType	�ړ��^�C�v
*	@param bType	�e�̃^�C�v
*/
	EnemySpawner::EnemySpawner(int max, float interval, int eType, int mType, int bType, int health) :
		spawnMax(max), spawnInterval(interval), enemyType(eType),
		moveType(mType), bulletType(bType), health(health) {
	}

	/**
	*	����������
	*/
	void EnemySpawner::Initialize() {
		playerEntity = GameEngine::Instance().FindEntityData<Player>();
	}

	/**
	*	�G�X�|�i�[�̍X�V����
	*
	*	@param delta	�o�ߎ���
	*/
	void EnemySpawner::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		time += delta;

		//�G�̏o������
		if (launchIndex < static_cast<int>(time / spawnInterval)) {
			SpawnEnemy();
			launchIndex++;
		}

		if (time >= spawnMax * spawnInterval) {
			//�X�|�[���I��

			entity->Destroy();
			return;
		}
	}

	/**
	*	�X�|�[������
	*/
	void EnemySpawner::SpawnEnemy() {

		GameEngine& game = GameEngine::Instance();

		bool isItemDrop = spawnMax == (launchIndex + 1);	///�Ō�ɏo������G�̂݃A�C�e���h���b�v����

		if (enemyType == 5) {
			//boss

			auto& b = std::make_shared<BossEnemy>();
			b->MoveController(MakeMoveControllerByMoveType(-1, false));
			b->Target(playerEntity);

			Entity::Entity* p = game.AddEntity(GameState::EntityGroupId_Enemy, entity->Position(),
				"MotherShip", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", b);
		}
		else {

			//�G�{�̂̍쐬
			auto& t = std::make_shared<Toroid>(0, health, isItemDrop);

			//�ړ��^�C�v�Ɋ�Â��Ĉړ��f�[�^��ݒ肷��
			t->MoveController(MakeMoveControllerByMoveType(moveType, entity->Position().x < 0));

			//�X�|�i�[�̈ʒu����X�|�[���n�_�̐ݒ�
			glm::vec3 pos = entity->Position();
			if (moveType == 2) {
				pos.z -= launchIndex * 5.0f;
			}

			Entity::Entity* p = game.AddEntity(GameState::EntityGroupId_Enemy, pos,
				"Toroid", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", t);

			//�e�̃^�C�v�Ɋ�Â��Ēe�����@���쐬
			if (bulletType != -1) {

				switch (bulletType) {
				case 1:
					t->BulletGenerator(std::make_shared<NormalShot>(*p, GameState::EntityGroupId_EnemyShot, playerEntity));
					break;
				case 2:
					t->BulletGenerator(std::make_shared<MultiWayShot>(*p, GameState::EntityGroupId_EnemyShot, nullptr));
					t->BulletGenerator()->Color(glm::vec4(0.3f, 1.0f, 0.5f, 1.0f));
					break;
				case 5:
					t->BulletGenerator(std::make_shared<CircleShot>(*p, GameState::EntityGroupId_EnemyShot));
					t->BulletGenerator()->Color(glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
				default:
					break;
				}
			}
			t->Target(playerEntity);

		}

	}

}