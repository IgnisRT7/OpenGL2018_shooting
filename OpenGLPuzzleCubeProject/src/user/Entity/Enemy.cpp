/**
*	@file Entity.cpp
*/
#include "Enemy.h"
#include "../../GameEngine.h"
#include "../../GameState.h"
#include "../../../Res/Audio/testProject_acf.h"
#include "../../../Res/Audio/CueSheet_0.h"

#include "Effect.h"
#include "Item.h"
#include "Bullet.h"
#include "Player.h"


namespace GameState {

	EnemyBulletManager::EnemyBulletManager(Entity::Entity& p, Entity::Entity* t) :
	parent(p), target(t), timer(shotInterval){
		target = GameEngine::Instance().FindEntityData<Player>();
	}

	/**
	*	�G�̏���������
	*/
	void Toroid::Initialize() {
		entity->CastShadow(true);
		entity->CastStencil(true);	//TODO : �X�e���V���}�X�N�̃e�X�g�p
		entity->StencilColor(glm::vec4(1, 0, 1, 1));

		entity->Collision(collisionDataList[EntityGroupId_Enemy]);
		bulletManager = std::make_shared<EnemyBulletManager>(*entity);

		return;
	}

	/**
	*	�G�̍X�V����
	*
	*	@param delta	�o�ߎ���
	*/
	void Toroid::Update(float delta) {

		timer += delta;

		if (bulletManager)bulletManager->Update(delta);

		float rot = glm::angle(entity->Rotation());
		const glm::vec3 pos = entity->Position();

		GameEngine& game = GameEngine::Instance();

		auto e = game.FindEntityData<Player>();

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
		rot += glm::radians(180.0f) * delta;
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

		GameEngine::Instance().PlayAudio(1, CRI_CUESHEET_0_EXPLOSIVE);
	}

	/// �G�X�|�i�[�̃N���X��`

	/**
	*	�G�X�|�i�[�̍X�V����
	*
	*	@param delta	�o�ߎ���
	*/
	void EnemyLaunchType::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		time += delta;

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
	*
	*	@param delta	�o�ߎ���
	*/
	void EnemyBulletManager::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		timer -= delta;

		if (timer < 0) {
			//game.PlayAudio(1, CRI_CUESHEET_0_ENEMYSHOT);

			if (Entity::Entity* p = game.AddEntity(EntityGroupId_EnemyShot, parent.Position(),
				"NormalShot", "Res/Model/Player.dds", std::make_shared<Bullet>(
					parent.Velocity(), target), "NonLighting")) {

				p->CastStencil(true);
				p->StencilColor(glm::vec4(1, 0, 1, 1));
				timer = shotInterval;

			}
		}
	}

}