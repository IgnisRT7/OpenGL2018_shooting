/**
*	@file Player.cpp
*/

#include "Player.h"
#include "../../Engine/GameEngine.h"

#include "Bullet.h"
#include "Item.h"
#include "Enemy.h"
#include "Effect.h"

#include "../../../Res/Audio/testProject_acf.h"
#include "../../../Res/Audio/CueSheet_0.h"

namespace GameState {

	/**
	*	�X�e�[�W�J�n���̉��o�p�ݒ菈��
	*/
	void Player::StartMoveSet(){

		startMovValue = 100;
		entity->Position(glm::vec3(0, 0, -screenHalfH));
		entity->Velocity(glm::vec3(0, 0, 15));
		isStartingMove = true;
	}

	/**
	*	�X�e�[�W�J�n��ɍs���鏈��
	*/
	void Player::StartMove(float delta) {

		if (entity->Position().z >= -screenHalfH * 0.5f) {

			isStartingMove = false;
			entity->Velocity(glm::vec3(0, 0, 0));
		}
	}

	/**
	*	�e�̔��ˏ���
	*/
	void Player::ShotBullet() {

		GameEngine& game = GameEngine::Instance();

		glm::vec3 pos = entity->Position();
		float bulletInterval = 1.0f;
		int bulletHalfIntrval = multiShotNum % 2 == 0 ? (int)(bulletInterval / 2) : 0;

		glm::vec3 leftPos = glm::vec3(pos.x - bulletInterval * (multiShotNum - 1) / 2, pos.y, pos.z);

		for (int i = 0; i < multiShotNum; ++i) {

			auto b = std::make_shared<Bullet>();

			if (Entity::Entity* p = game.AddEntity(EntityGroupId_PlayerShot, leftPos + glm::vec3(i * bulletInterval, 0, 0),
				"NormalShot", "Res/Model/Player.dds", b, "NonLighting")) {

				b->Velocity(glm::vec3(0, 0, 100));
				p->Collision(collisionDataList[EntityGroupId_PlayerShot]);
				p->Scale(glm::vec3(1.5f));
			}
			pos.x += 0.25f;
		}

		game.PlayAudio(0, CRI_CUESHEET_0_SHOT);
	}

	/**
	*	����������
	*/
	void Player::Initialize() {

		if (!initialized) {
			initialized = true;
			entity->Collision(collisionDataList[EntityGroupId_Player]);
		}

		entity->Scale(glm::vec3(1.5f));
		entity->CastShadow(true);
	}

	/**
	*	�X�V����
	*
	*	@param delta	�o�ߎ���
	*/
	void Player::Update(float delta) {

		if (remainingPlayer < 0) {
			return;
		}

		timer += delta;

		//�_�ŏ���
		if (damageTimer > 0) {
			const float speedMul = 30.0f;
			damageTimer -= delta;

			float colorAlpha = damageTimer <= 0 ? 1 : glm::max(0.0f, (glm::sin(damageTimer*speedMul)));

			entity->Color(glm::vec4(1, 1, 1, colorAlpha));
		}


		if (isStartingMove) {
			StartMove(delta);
		}
		else {

			GameEngine& game = GameEngine::Instance();

			const GamePad gamepad = game.GetGamePad();

			//�ړ��ʌv�Z����
			glm::vec3 vec;
			float rotZ = 0;
			if (gamepad.buttons & GamePad::DPAD_LEFT) {
				vec.x = 1;
				rotZ = -glm::radians(30.0f);
			}
			else if (gamepad.buttons & GamePad::DPAD_RIGHT) {
				vec.x = -1;
				rotZ = glm::radians(30.0f);
			}
			if (gamepad.buttons & GamePad::DPAD_UP) {
				vec.z = 1;
			}
			else if (gamepad.buttons & GamePad::DPAD_DOWN) {
				vec.z = -1;
			}
			if (vec.x || vec.z) {
				vec = glm::normalize(vec) * moveSpeed * moveMultiply;
			}
			entity->Velocity(vec);

			//�ړ���������
			glm::vec3 pos = entity->Position();
			pos = glm::min(moveBox[1], glm::max(pos, moveBox[0]));
			entity->Position(pos);


			entity->Rotation(glm::quat(glm::vec3(0, 0, rotZ)));

			//�e�̔��ˏ���
			if (gamepad.buttons & GamePad::A) {
				if ((shotInterval -= delta) <= 0) {

					ShotBullet();
					shotInterval = 0.1f - std::abs(shotInterval);
				}
			}
			else {
				shotInterval = 0;
			}


		}
	}

	/**
	*	�_���[�W����
	*/
	void Player::Damage(float p) {

		if (remainingPlayer < 0) {
			return;
		}

		if (damageTimer <= 0) {

			GameEngine& game = GameEngine::Instance();

			//�����G�t�F�N�g
			if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, entity->Position(), "Blast", "Res/Model/Toroid.dds", std::make_shared<Blast>())) {
				const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
				p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
				p->Color(glm::vec4(1.0f, 0.75f, 0.5f, 1.0f));
				p->CastStencil(true);
				p->StencilColor(glm::vec4(0, 1, 0, 1));
				game.UserVariable("score") += 100;
			}

			//������
			//game.PlayAudio(1, CRI_CUESHEET_0_SHOT);

			if (--remainingPlayer < 0) {

				entity->CastShadow(false);  
				entity->Color(glm::vec4(0, 0, 0, 0));
				entity->Position(glm::vec3(500));
				return;
			}
			
			StartMoveSet();
			damageTimer = 4.0f;
		}
	}

	/**
	*	�Փ˔��菈��
	*/
	void Player::CollisionEnter(Entity::Entity& entity) {

		if (remainingPlayer >= 0) {

			if (auto i = entity.CastTo<Item>()) {
				//�A�C�e���̌��ʂ��󂯂�

				if (i->ItemType() == 1) {
					moveMultiply = glm::min(1.5f, moveMultiply + 0.1f);
				}
				else {

					multiShotNum = glm::min(multiShotNum + 1, 5); 
				}
			}
			if (auto e = entity.CastTo<Toroid>()) {
				//�G�Ƀ_���[�W��^����

				e->Damage(1);
				GameEngine::Instance().PlayAudio(1, CRI_CUESHEET_0_EXPLOSIVE);
			}
		}
	}



	/**
	*	����������
	*/
	void PlayerForProduction::Initialize(){

		entity->Position(glm::vec3(0, 0, -20));
	}
}