/**
*	@file Player.cpp
*/

#include "Player.h"
#include "../../GameEngine.h"
#include "../../GameState.h"

#include "Bullet.h"
#include "Item.h"
#include "Enemy.h"

namespace GameState {

	/**
	*	�X�e�[�W�J�n��ɍs���鏈��
	*/
	void Player::StartMove(double delta) {

		if (entity->Position().z >= 0) {

			isStartingMove = false;
			entity->Velocity(glm::vec3(0, 0, 0));
		}
	}

	void Player::Initialize() {

		entity->Position(glm::vec3(0, 0, -8));
		entity->Velocity(glm::vec3(0, 0, 15));
		startMovValue = 100;
		isStartingMove = true;
	}

	void Player::Update(double delta) {

		timer += static_cast<float>(delta);

		//�_�ŏ���
		if (damageTimer > 0) {
			const float speedMul = 30.0f;
			damageTimer -= static_cast<float>(delta);

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
				vec = glm::normalize(vec) * moveSpeed;
			}

			/**
			*	��ʋP�x�̐ݒ�
			*/
			if (gamepad.buttonDown & GamePad::B) {

				game.KeyValue(glm::min(1.0, game.KeyValue() + delta));
			}
			else if (gamepad.buttonDown & GamePad::X) {

				game.KeyValue(glm::max(0.0, game.KeyValue() - delta));
			}


			entity->Velocity(vec);
			glm::vec3 pos = entity->Position();
			pos = glm::min(moveBox[1], glm::max(pos, moveBox[0]));
			entity->Position(pos);


			entity->Rotation(glm::quat(glm::vec3(0, 0, rotZ)));

			//�e�̔��ˏ���
			if (gamepad.buttons & GamePad::A) {
				shotInterval -= delta;
				float tmp = static_cast<float>(std::abs(shotInterval));

				if (shotInterval <= 0) {

					glm::vec3 pos = entity->Position();
					float bulletInterval = 1.0f;
					glm::vec3 leftPos = glm::vec3(pos.x - (multiShotNum - 1) / 2 * bulletInterval, pos.y, pos.z);

					game.PlayAudio(0, 0);
					for (int i = 0; i < multiShotNum; ++i) {

						if (Entity::Entity* p = game.AddEntity(EntityGroupId_PlayerShot, leftPos + glm::vec3(i*bulletInterval, 0, 0),
							"NormalShot", "Res/Model/Player.dds", std::make_shared<PlayerShot>(), "NonLighting")) {

							p->Velocity(glm::vec3(0, 0, 200));
							p->Collision(collisionDataList[EntityGroupId_PlayerShot]);
						}
						pos.x += 0.25f;
					}
					shotInterval = 0.1f - tmp;
				}

			}
			else {
				shotInterval = 0;
			}
		}
	}

	/**
	*	�v���C���[�̏Փ˔��菈��
	*/
	void Player::CollisionEnter(Entity::Entity& entity) {

		if (auto i = entity.CastTo<Item>()) {

			if (i->ItemType() == 1) {
				moveSpeed = glm::min(10.0f, moveSpeed + 2);
			}
			else {

				multiShotNum = glm::min(5, multiShotNum + 1);

			}
		}
		else if (entity.CastTo<Toroid>()) {

			if (damageTimer <= 0) {

				Initialize();
				damageTimer = 4.0f;
			}

		}
	}



}