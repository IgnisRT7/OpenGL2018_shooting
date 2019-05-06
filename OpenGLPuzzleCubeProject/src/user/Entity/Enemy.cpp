/**
*	@file Entity.cpp
*/
#include "Enemy.h"
#include "../../GameEngine.h"
#include "../../GameState.h"
#include "../../../Res/Audio/SampleSound_acf.h"
#include "../../../Res/Audio/SampleCueSheet.h"

#include "Effect.h"
#include "Item.h"
#include "Bullet.h"
#include "Player.h"

namespace GameState {


	/**
	*	�G�̏���������
	*/
	void Toroid::Initialize() {
		entity->CastShadow(false);
		entity->CastStencil(true);	//TODO : �X�e���V���}�X�N�̃e�X�g�p

		bulletManager = std::make_shared<EnemyBulletManager>(*entity);
	}

	/**
	*	�G�̍X�V����
	*/
	void Toroid::Update(double delta) {

		timer += static_cast<float>(delta);

		if (bulletManager)bulletManager->Update(delta);

		float rot = glm::angle(entity->Rotation());
		const glm::vec3 pos = entity->Position();

		GameEngine& game = GameEngine::Instance();


		// �ړ�����
		switch (enemyType) {
		case 0:	/// �f�t�H���g�̏����܂������i��

			entity->Velocity(glm::vec3(0, 0, -10));


			break;

		case 1:	/// �֍s���ĉ�������
		{
			float velX = glm::cos(timer * 2) * 10 - entity->Velocity().x;
			glm::vec3 vel = glm::vec3(velX, 0, -5);
			entity->Velocity(vel);

			break;
		}
		case 2: ///�v���C���[�L�����֌y�ǔ�

			


			break;

		case 3:

		default:
			break;
		}

		// �~�Ղ���]������.
		rot += glm::radians(180.0f) * static_cast<float>(delta);
		if (rot > glm::pi<float>() * 2.0f) {
			rot -= glm::pi<float>() * 2.0f;
		}
		entity->Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		//��ʊO���菈��
		if (std::abs(pos.x) > 200.0f || std::abs(pos.z) > 200.0f) {
			GameEngine::Instance().RemoveEntity(entity);
			return;
		}
	}

	/**
	*	�_���[�W����
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

				int itemID = rand() % 2;

				//�A�C�e��
				if (Entity::Entity* p = game.AddEntity(EntityGroupId_Item, entity->Position(), "ItemBox", "Res/Model/ItemBox.dds", std::make_shared<Item>(itemID), "NonLighting")) {
					p->Collision(collisionDataList[EntityGroupId_Item]);
				}
			}

			//������
			game.PlayAudio(1, CRI_SAMPLECUESHEET_BOMB);

			entity->Destroy();
		}
	}

	/**
	*	�Փ˔��菈��
	*/
	void Toroid::CollisionEnter(Entity::Entity& e) {
	}

	/// �G�X�|�i�[�̃N���X��`

	/**
	*	�G�X�|�i�[�̍X�V����
	*/
	void EnemyLaunchType::Update(double delta) {

		GameEngine& game = GameEngine::Instance();

		time += static_cast<float>(delta);

		if (time >= spawnMax * spawnInterval) {
			//�X�|�[���I��

			entity->Destroy();
			return;
		}

		//�G�̏o������
		if (launchIndex < static_cast<int>(time / spawnInterval)) {

			bool isItemDrop = spawnMax == (launchIndex + 2);	///�Ō�ɏo������G�̂݃A�C�e���h���b�v����

			Entity::Entity* p = game.AddEntity(EntityGroupId_Enemy, entity->Position(),
				"Toroid", "Res/Model/Toroid.dds", "Res/Model/Toroid.Normal.bmp", std::make_shared<Toroid>(0, isItemDrop));

			p->Collision(collisionDataList[EntityGroupId_Enemy]);

			launchIndex++;
		}
	}

	/**
	*	�X�V����
	*/
	void EnemyBulletManager::Update(double delta) {

		GameEngine& game = GameEngine::Instance();



		timer -= static_cast<float>(delta);

		if (timer < 0) {

			if (Entity::Entity* p = game.AddEntity(EntityGroupId_EnemyShot, parent.Position(),
				"NormalShot", "Res/Model/Player.dds", std::make_shared<Bullet>(parent.Velocity() * 1.1f), "NonLighting")) {

				timer = shotInterval;
			}
		}
	}

}