/**
*	@file BulletManager.cpp
*/
#include "BulletManager.h"
#include "../GameEngine.h"
#include "../GameState.h"
#include "Entity/Bullet.h"

#include "../../Res/Audio/testProject_acf.h"
#include "../../Res/Audio/CueSheet_0.h"

#include <glm/gtc/matrix_transform.hpp>


namespace GameState {

	BulletManager::BulletManager(Entity::Entity& p,int groupId, Entity::Entity* t) :
		parent(p), target(t), timer(0),groupId(groupId) {

		initBulletSpeed = 10.f;
		timer = shotInterval* 0.2f;
	}

	/**
	*	更新処理
	*
	*	@param delta	経過時間
	*/
	void BulletManager::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		timer -= delta;

		if (timer < 0) {
			//game.PlayAudio(1, CRI_CUESHEET_0_ENEMYSHOT);

			glm::vec3 targetVel = target ? target->Position() - parent.Position() : glm::vec3(0, 0, -1);
			targetVel = glm::normalize(targetVel) * initBulletSpeed;

			if (Entity::Entity* p = game.AddEntity(groupId, parent.Position(),
				"Sphere", "Res/Model/sphere.dds", std::make_shared<Bullet>())) {

				p->Velocity(targetVel);
				p->CastStencil(true);
				p->StencilColor(glm::vec4(1, 0, 1, 1));
				p->Scale(glm::vec3(0.5f));
				timer = shotInterval;

			}
		}
	}

	/**
	*	移動方向の再計算処理
	*/
	void BulletManager::CalcVelocity() {
		
		glm::vec3 dir = glm::vec3(0, 0, -1);

		if (target) {
			dir = target->Position() - parent.Position();
			dir = glm::normalize(dir);
		}

		dir *= initBulletSpeed;
	}

	/**
	*	初期化処理
	*
	*	@param p	発射元のエンティティ
	*	@param i	所属するグループID
	*	@param t	ターゲット
	*	@param a	弾と弾の間隔の角度
	*	@param m	同時に発射できる弾数
	*/
	MultiWayShot::MultiWayShot(Entity::Entity& p,int i, Entity::Entity* t, float a, int m) :
		BulletManager(p, i, t),angleInterval(a),maxBulletNum(m) {

		initBulletSpeed = 10.f;
	}

	/**
	*	更新処理
	*/
	void MultiWayShot::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		timer -= delta;

		if (timer < 0) {
			//game.PlayAudio(1, CRI_CUESHEET_0_ENEMYSHOT);

			glm::vec3 centerDir = target ? target->Position() - parent.Position() : glm::vec3(0, 0, -1);
			centerDir = glm::normalize(centerDir);
			
			for (int i = 0; i < maxBulletNum; i++) {

				//弾の角度・速度設定処理
				glm::quat rot = glm::angleAxis(angleInterval * (i - (int)(maxBulletNum / 2)), glm::vec3(0, 1, 0));
				glm::vec3 vel = rot * centerDir;
				vel *= initBulletSpeed;

				//エンティティの追加処理
				if (Entity::Entity* p = game.AddEntity(groupId, parent.Position(),
					"Sphere", "Res/Model/sphere.dds", std::make_shared<Bullet>(
						vel))) {

					p->Color(glm::vec4(1, 0, 0, 1));
					p->Scale(glm::vec3(0.5f));
					timer = shotInterval;
				}
			}
		}

	}

	/**
	*	初期化処理
	*
	*	@param p	親のエンティティ
	*	@param i	所属するグループID
	*	@param angInterval	発射する角度の間隔
	*	@param initAngle	最初に発射する弾の角度
	*/
	CircleShot::CircleShot(Entity::Entity& p, int i,float angInterval,float initAngle) :
		BulletManager(p,i),angleInterval(angInterval),shotAngle(initAngle){

		shotInterval = 0.2f;
		timer = 0;
	}

	void CircleShot::Update(float delta){

		timer += delta;

		if ( timer > shotInterval) {
			///弾の発射処理

			GameEngine& game = GameEngine::Instance();

			std::shared_ptr<Bullet> b = std::make_shared<Bullet>();
			Entity::Entity* p = game.AddEntity(groupId, parent.Position(),
				"Sphere", "Res/Model/sphere.dds", b);

			if (shotAngle > glm::pi<float>() * 2.0f) {
				shotAngle -= glm::pi<float>() * 2.0f;
			}
			glm::quat rot = glm::angleAxis(shotAngle, glm::vec3(0,1,0));
			glm::vec3 newVel = rot * glm::vec3(0, 0, 1);

			p->Velocity(newVel * initBulletSpeed);
			p->Scale(glm::vec3(0.5));

			//game.PlayAudio()
			shotAngle += angleInterval;
			timer = 0;
		}

	}

	/**
	*	コンストラクタ
	*
	*	@param parent	親クラスのエンティティ
	*/
	PlayerShot_TypeNormal::PlayerShot_TypeNormal(Entity::Entity & parent):
		BulletManager(parent,EntityGroupId_PlayerShot){

		initBulletSpeed = 90.f;
		shotInterval = 0.2f;
		shotButton = GamePad::A;
		
	}

	/**
	*	更新処理
	*
	*	@param delta	経過時間
	*/
	void PlayerShot_TypeNormal::Update(float delta){

		GameEngine& game = GameEngine::Instance();
		const GamePad& gamepad = game.GetGamePad();

		if (timer > 0)timer -= delta;

		if ((gamepad.buttons & shotButton) && (timer <= 0)) {
			//弾の発射処理

			static const float bulletInterval = 1;
 			glm::vec3 parPos = parent.Position();

			glm::vec3 leftPos = glm::vec3(parPos.x - bulletInterval * (bulletLevel - 1) / 2, parPos.y, parPos.z);

			game.PlayAudio(1, CRI_CUESHEET_0_SHOT);
			for (int i = 0; i < bulletLevel; ++i) {

				if (Entity::Entity* p = game.AddEntity(groupId, leftPos + glm::vec3(i*bulletInterval, 0, 0),
					"NormalShot", "Res/Model/Player.dds", std::make_shared<Bullet>(glm::vec3(0, 0, 200)), "NonLighting")) {

					p->Velocity(glm::vec3(0, 0, 1) * initBulletSpeed);
					p->Scale(glm::vec3(2.f));
				}
				parPos.x += 0.25f;
			}

			timer = shotInterval;
		}
	}


}