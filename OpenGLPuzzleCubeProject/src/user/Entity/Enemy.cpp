/**
*	@file Entity.cpp
*/
#include "Enemy.h"
#include "../../GameEngine.h"
#include "../../GameState.h"
#include "../../../Res/Audio/testProject_acf.h"
#include "../../../Res/Audio/CueSheet_0.h"
#include "../EnemyLaunchController.h"
#include "../GameMainScene.h"

#include "Effect.h"
#include "Item.h"
#include "Bullet.h"
#include "Player.h"
#include <memory>

namespace GameState {



	/**
	*	�G�̏���������
	*/
	void Toroid::Initialize() {
		entity->CastShadow(true);
		entity->CastStencil(true);	//TODO : �X�e���V���}�X�N�̃e�X�g�p
		entity->StencilColor(glm::vec4(1, 0, 1, 1));
		entity->Scale(glm::vec3(1.5));

		entity->Collision(collisionDataList[EntityGroupId_Enemy]);

		return;
	}

	/**
	*	�G�̍X�V����
	*
	*	@param delta	�o�ߎ���
	*/
	void Toroid::Update(float delta) {

		timer += delta;

		//�e�Ǘ��V�X�e���̍X�V
		if (bulletManager) {
			bulletManager->Update(delta);
		}

		//�ړ��Ǘ��V�X�e���̍X�V
		if (moveController) {
			moveController->Update(*entity, delta);
		}

		// �~�Ղ���]������.
		float rot = glm::angle(entity->Rotation());
		rot += glm::radians(180.0f) * delta;
		if (rot > glm::pi<float>() * 2.0f) {
			rot -= glm::pi<float>() * 2.0f;
		}
		entity->Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		//��ʊO���菈��
		if (isDestroyByScreenOut) {
			const glm::vec3 pos = entity->Position();
			if (std::abs(pos.x) > screenHalfW * 1.2 || std::abs(pos.z) > screenHalfH * 1.2) {

				std::cout << "enemy of outrange" << std::endl;
				GameEngine::Instance().RemoveEntity(entity);
				return;
			}
		}
	}

	/**
	*	�_���[�W����
	*
	*	@param p	�_���[�W��
	*/
	void Toroid::Damage(float p) {

		if (--hp <= 0) {

			GameEngine& game = GameEngine::Instance();

			//�����G�t�F�N�g
			if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, entity->Position(), "Blast", "Res/Model/Toroid.dds", std::make_shared<Blast>())) {
				const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
				p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
				p->Color(glm::vec4(1.0f, 0.75f, 0.5f, 1.0f));
				game.UserVariable("score") += 100;
			}

			if (isItemDrop) {
				//�A�C�e���h���b�v����

				int itemID = rand() % 2;

				//�A�C�e��
				std::string texName = itemID ? "Res/Model/ItemBoxSpeed.dds" : "Res/Model/ItemBoxBullet.dds";

				if (Entity::Entity* p = game.AddEntity(EntityGroupId_Item, entity->Position(), "ItemBox", texName.c_str(), std::make_shared<Item>(itemID))) {
					p->Collision(collisionDataList[EntityGroupId_Item]);
				}
			}

			entity->Destroy();
		}
	}

	/**
	*	�Փ˔��菈��
	*
	*	@param e	�Փ˂��Ă����G���e�B�e�B
	*/
	void Toroid::CollisionEnter(Entity::Entity& e) {
		e.EntityData()->Damage(1);

		//GameEngine::Instance().PlayAudio(1, CRI_CUESHEET_0_EXPLOSIVE);
	}

	/**
	*	�^�[�Q�b�g�̐ݒ�
	*/
	void Toroid::Target(Entity::Entity * t) {

		playerEntity = t;

		if (bulletManager) {
 			bulletManager->Target(t);
		}
	}

	/**
	*	����������
	*/
	void BossEnemy::Initialize() {

		GameEngine& game = GameEngine::Instance();

		entity->Scale(glm::vec3(10));
		entity->Collision({ { -15,-20,-10},{15,20,10} });

		for (int i = 0; i < 8; i++) {

			auto t = std::make_shared<Toroid>();
			t->DestroyByScreenOut(false);

			if (auto p = game.AddEntity(EntityGroupId_Others, glm::vec3(-60),
				"Toroid", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", t, "NonLighting")) {

				turrets.push_back(t);
				
				//�^���b�g�p�e�쐬
				BulletManagerPtr b;
				if ((i % 2) == 0) {
					b = std::make_shared<MultiWayShot>(*p, EntityGroupId_EnemyShot, nullptr, 15, 3);
				}
				else {
					b = std::make_shared<BulletManager>(*p, EntityGroupId_EnemyShot, playerEntity);
				}

				t->BulletManager(b);
			}
		}


	}

	/**
	*	�X�V����
	*
	*	@param delta	�o�ߎ���
	*/
	void BossEnemy::Update(float delta){

		timer += delta;

		//�ړ��Ǘ��V�X�e���̍X�V
		if (moveController) {
			moveController->Update(*entity, delta);
		}

		// �~�Ղ���]������.
		float rot = glm::angle(entity->Rotation());
		rot += glm::radians(30.0f) * delta;
		if (rot > glm::pi<float>() * 2.0f) {
			rot -= glm::pi<float>() * 2.0f;
		}
		entity->Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		//�^���b�g�̈ʒu���X�V����
		UpdateTurret();
	}

	/**
	*	�_���[�W����
	*
	*	@param p	�_���[�W��
	*/
	void BossEnemy::Damage(float p){

		if (--hp <= 0) {

			GameEngine& game = GameEngine::Instance();

			//�����G�t�F�N�g
			if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, entity->Position(), "Blast", "Res/Model/Toroid.dds", std::make_shared<Blast>())) {
				const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
				p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
				p->Color(glm::vec4(1.0f, 0.75f, 0.5f, 1.0f));
				game.UserVariable("score") += 100;

				game.PlayAudio(0, CRI_CUESHEET_0_EXPLOSIVE);

				for (auto turret : turrets) {
					turret->Entity()->Destroy();
				}
				dynamic_cast<MainGame*>(&SceneStack::Instance().Current())->StageClear(5);
			}

			entity->Destroy();
		}
	}

	void BossEnemy::CollisionEnter(Entity::Entity &e){

		e.EntityData()->Damage(1);
	}

	/**
	*	�^���b�g�̈ʒu���X�V����
	*/
	void BossEnemy::UpdateTurret() {

		glm::quat baseRot = entity->Rotation();
		glm::vec3 centerPos = entity->Position();
		glm::vec3 forward = glm::vec3(-1, 0, 0) ;

		for (int i = 0; i < 8; i++) {

			const float perRad = glm::pi<float>() * 2.0f * (1.0f / 8.0f);
			float rot = glm::pi<float>() * 2.0f * 0.125 * i;
			glm::quat tmp = glm::angleAxis(rot, glm::vec3(0, 1, 0));

			tmp = tmp * baseRot;

			glm::vec3 vec = tmp * (forward * 15.0f);

			turrets[i]->Entity()->Position(centerPos + vec);
		}
	}

	/**
	*	�^�[�Q�b�g�̐ݒ�
	*
	*	@param t	�^�[�Q�b�g�̃G���e�B�e�B
	*/
	void BossEnemy::Target(Entity::Entity* t){

		playerEntity = t;
		for (auto turret : turrets) {
			if (turret) {
				turret->Target(t);
			}
		}
	}

	/// �G�X�|�i�[�̃N���X��`

	/**
	*	�R���X�g���N�^
	*
	*	@param max	�X�|�[����
	*	@param interval	�o���Ԋu
	*	@param eType	�G�̃^�C�v
	*	@param mType	�ړ��^�C�v
	*	@param bType	�e�̃^�C�v
	*/
	EnemySpawner::EnemySpawner(int max, float interval, int eType,int mType, int bType) :
		spawnMax(max), spawnInterval(interval), enemyType(eType),moveType(mType), bulletType(bType) {
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

			Entity::Entity* p = game.AddEntity(EntityGroupId_Enemy, entity->Position(),
				"MotherShip", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", b, "NonLighting");

		}
		else {

			//�G�{�̂̍쐬
			auto& t = std::make_shared<Toroid>(0, isItemDrop);

			t->MoveController(MakeMoveControllerByMoveType(moveType, entity->Position().x < 0));

			glm::vec3 pos = entity->Position();
			if (moveType == 2) {
				pos.z -= launchIndex * 5.f;
			}

			Entity::Entity* p = game.AddEntity(EntityGroupId_Enemy, pos,
				"Toroid", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", t);

			//�e�Ǘ��̍쐬
			if (bulletType != -1) {

				switch (bulletType) {
				case 1:
					t->BulletManager(std::make_shared<BulletManager>(*p, EntityGroupId_EnemyShot, playerEntity));
					break;
				case 5:
					t->BulletManager(std::make_shared<CircleShot>(*p, EntityGroupId_EnemyShot));
				default:
					break;
				}
			}
			t->Target(playerEntity);

		}



	}


}
