/**
*	@file Player.cpp
*/

#include "Player.h"
#include "../../GameEngine.h"
#include "../../GameState.h"

#include "Bullet.h"
#include "Item.h"
#include "Enemy.h"
#include "Effect.h"

#include "../../../Res/Audio/SampleSound_acf.h"
#include "../../../Res/Audio/SampleCueSheet.h"

namespace GameState {

	/**
	*	ステージ開始後に行われる処理
	*/
	void Player::StartMove(double delta) {

		if (entity->Position().z >= 0) {

			isStartingMove = false;
			entity->Velocity(glm::vec3(0, 0, 0));
		}
	}

	void Player::Initialize() {

		entity->Collision(collisionDataList[EntityGroupId_Player]);
		entity->Position(glm::vec3(0, 0, -8));
		entity->Velocity(glm::vec3(0, 0, 15));
		entity->CastStencil(true);
		entity->CastShadow(true);
		entity->StencilColor(glm::vec4(0, 1, 0, 1));
		startMovValue = 100;
		isStartingMove = true;
	}

	void Player::Update(double delta) {

		timer += static_cast<float>(delta);

		//点滅処理
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

			//移動量計算処理
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
			*	画面輝度の設定
			*
			if (gamepad.buttonDown & GamePad::B) {

				game.KeyValue(glm::min(1.0, game.KeyValue() + delta));
			}
			else if (gamepad.buttonDown & GamePad::X) {

				game.KeyValue(glm::max(0.0, game.KeyValue() - delta));
			}*/


			entity->Velocity(vec);
			glm::vec3 pos = entity->Position();
			pos = glm::min(moveBox[1], glm::max(pos, moveBox[0]));
			entity->Position(pos);


			entity->Rotation(glm::quat(glm::vec3(0, 0, rotZ)));

			//弾の発射処理
			if (gamepad.buttons & GamePad::A) {
				if ((shotInterval -= delta) <= 0) {

					ShotBullet();
					shotInterval = 0.1f - static_cast<float>(std::abs(shotInterval));
				}
			}
			else {
				shotInterval = 0;
			}
		}
	}

	/**
	*	ダメージ処理
	*/
	void Player::Damage(float p) {

		if (damageTimer <= 0) {

			GameEngine& game = GameEngine::Instance();

			//爆発エフェクト
			if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, entity->Position(), "Blast", "Res/Model/Toroid.dds", std::make_shared<Blast>())) {
				const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
				p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
				p->Color(glm::vec4(1.0f, 0.75f, 0.5f, 1.0f));
				p->CastStencil(true);
				p->StencilColor(glm::vec4(0, 1, 0, 1));
				game.UserVariable("score") += 100;
			}

			//爆発音
			game.PlayAudio(1, CRI_SAMPLECUESHEET_BOMB);

			if (--remainingPlayer < 0) {

				game.RemoveEntity(entity);
				return;
			}
			
			Initialize();
			damageTimer = 4.0f;
		}
	}

	/**
	*	衝突判定処理
	*/
	void Player::CollisionEnter(Entity::Entity& entity) {

		if (auto i = entity.CastTo<Item>()) {
			//アイテムの効果を受ける

			if (i->ItemType() == 1) {
				moveSpeed = glm::min(10.0f, moveSpeed + 2);
			}
			else {

				multiShotNum = glm::min(5, multiShotNum + 1);

			}
		}
		if (auto e = entity.CastTo<Toroid>()) {
			//敵にダメージを与える

			e->Damage(1);
			Damage(1);
		}
	}

	/**
	*	弾の発射処理
	*/
	void Player::ShotBullet() {

		GameEngine& game = GameEngine::Instance();

		glm::vec3 pos = entity->Position();
		float bulletInterval = 1.0f;
		int bulletHalfIntrval = multiShotNum % 2 == 0 ? (int)(bulletInterval / 2) : 0;

		glm::vec3 leftPos = glm::vec3(pos.x - bulletInterval * (multiShotNum - 1) / 2, pos.y, pos.z);

		game.PlayAudio(0, 0);
		for (int i = 0; i < multiShotNum; ++i) {

			if (Entity::Entity* p = game.AddEntity(EntityGroupId_PlayerShot, leftPos + glm::vec3(i*bulletInterval, 0, 0),
				"NormalShot", "Res/Model/Player.dds", std::make_shared<Bullet>(glm::vec3(0, 0, 200)), "NonLighting")) {

				p->Collision(collisionDataList[EntityGroupId_PlayerShot]);
				p->CastStencil(true);
				p->StencilColor(glm::vec4(0, 1, 0, 1));
			}
			pos.x += 0.25f;
		}
	}


	/**
	*	初期化処理
	*/
	void PlayerForProduction::Initialize(){

		entity->Position(glm::vec3(0, 0, -20));
	}
}