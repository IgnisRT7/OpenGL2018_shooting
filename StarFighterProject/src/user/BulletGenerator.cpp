/**
*	@file	BulletGenerator.cpp
*	@brief	敵の弾生成システムの制御用
*	@author	Takuya Yokoyama
*/

#include "BulletGenerator.h"
#include "../Engine/GameEngine.h"
#include "../GameState.h"
#include "Entity/Bullet.h"
#include "../../Res/Resource.h"

#include "../../Res/Audio/testProject_acf.h"
#include "../../Res/Audio/CueSheet_0.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Application {

	BulletGenerator::BulletGenerator(Entity::Entity& p,int groupId, Entity::Entity* t) :
		parent(p), target(t), timer(0),groupId(groupId) {

		initBulletSpeed = 10.f;
		timer = shotInterval* 0.2f;

		if (target) {
			CalcVelocity();
		}
	}

	void BulletGenerator::ShotBullet(){

		auto b = std::make_shared<Bullet>();

		//エンティティの追加処理
		if (Entity::Entity* p = GameEngine::Instance().AddEntity(groupId, parent.Position(),
			"Sphere", Resource::tex_bullet, b)) {

			b->Velocity(direction * initBulletSpeed);
			b->Color(color);
			
			p->Scale(glm::vec3(0.5f));
			timer = shotInterval;
		}
	}

	void BulletGenerator::CalcVelocity() {
		
		direction = glm::vec3(0, 0, -1);

		if (target) {
			direction = target->Position() - parent.Position();
			direction = glm::normalize(direction);
		}
	}

	NormalShot::NormalShot(Entity::Entity& p, int id, Entity::Entity* t) :
		BulletGenerator(p, id, t) {

		initBulletSpeed = 10.0f;

	}

	void NormalShot::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		timer -= delta;

		if (timer < 0) {

			if (target) {
				CalcVelocity();
			}
	
			game.PlayAudio(1, CRI_CUESHEET_0_LASER2);

			ShotBullet();
		}
	}

	MultiWayShot::MultiWayShot(Entity::Entity& p,int i, Entity::Entity* t, float a, int m) :
		BulletGenerator(p, i, t),angleInterval(a),maxBulletNum(m) {

		initBulletSpeed = 10.f;
	}

	void MultiWayShot::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		timer -= delta;

		if (timer < 0) {
			game.PlayAudio(1, CRI_CUESHEET_0_LASER2);

			if (target) {
				CalcVelocity();
			}

			glm::vec3 centerDir = direction;
			
			for (int i = 0; i < maxBulletNum; i++) {

				int settingIndex = (i - (int)(maxBulletNum / 2));

				//弾の角度・速度設定処理
				glm::quat rot = glm::angleAxis(glm::radians(angleInterval * settingIndex), glm::vec3(0, 1, 0));
				direction = rot * centerDir;

				ShotBullet();
			}
		}

	}

	void MultiWayShot::CenterDirectiton(glm::vec3 v){

		direction =v;
	}

	CircleShot::CircleShot(Entity::Entity& p, int i,float angInterval,float initAngle) :
		BulletGenerator(p,i),angleInterval(angInterval),shotAngle(initAngle){

		shotInterval = 0.2f;
		timer = 0;
	}

	void CircleShot::Update(float delta){

		GameEngine& game = GameEngine::Instance();

		timer += delta;

		if ( timer > shotInterval) {
			///弾の発射処理

			if (target) {
				CalcVelocity();
			}

			game.PlayAudio(1, CRI_CUESHEET_0_LASER2);

			GameEngine& game = GameEngine::Instance();

			std::shared_ptr<Bullet> b = std::make_shared<Bullet>();
			Entity::Entity* p = game.AddEntity(groupId, parent.Position(),
				"Sphere", Resource::tex_bullet, b);

			if (shotAngle > glm::pi<float>() * 2.0f) {
				shotAngle -= glm::pi<float>() * 2.0f;
			}
			glm::quat rot = glm::angleAxis(shotAngle, glm::vec3(0,1,0));
			glm::vec3 newVel = rot * glm::vec3(1, 0, 0);

			b->Velocity(newVel * initBulletSpeed);
			p->Scale(glm::vec3(0.5));
			p->Color(color);

			shotAngle += glm::radians(angleInterval);
			timer = 0;
		}

	}
}