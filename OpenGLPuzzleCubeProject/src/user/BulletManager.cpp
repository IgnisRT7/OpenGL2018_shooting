/**
*	@file BulletManager.cpp
*/
#include "BulletManager.h"
#include "../GameEngine.h"
#include "../GameState.h"
#include "Entity/Bullet.h"

namespace GameState {

	BulletManager::BulletManager(Entity::Entity& p, Entity::Entity* t) :
		parent(p), target(t), timer(shotInterval) {

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

			if (Entity::Entity* p = game.AddEntity(EntityGroupId_EnemyShot, parent.Position(),
				"Sphere", "Res/Model/sphere.dds", std::make_shared<Bullet>(
					parent.Velocity(), target))) {



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
	void BulletManager::CalcVelocity(){

		initBulletSpeed = 20;
		glm::vec3 dir = glm::vec3(0, 0, -1);

		if (target) {
			dir = target->Position() - parent.Position();
			dir = glm::normalize(dir);
		}

		dir *= initBulletSpeed;
	}

	/**
	*	更新処理
	*/
	void FiveWayShot::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		timer -= delta;

		if (timer < 0) {
			//game.PlayAudio(1, CRI_CUESHEET_0_ENEMYSHOT);

			glm::vec3 centerDir = target ? target->Position() - parent.Position() : glm::vec3(0, 0, -1);
			centerDir = glm::normalize(centerDir);

			const float radInterval = glm::radians(10.f);

			for (int i = 0; i < 5; i++) {

				glm::quat rot = glm::angleAxis(radInterval * (i - 2), glm::vec3(0, 1, 0));
				glm::vec3 vel = rot * centerDir;
				vel *= 10;

				if (Entity::Entity* p = game.AddEntity(EntityGroupId_EnemyShot, parent.Position(),
					"Sphere", "Res/Model/sphere.dds", std::make_shared<Bullet>(
						vel, nullptr))) {

					p->CastStencil(true);
					p->StencilColor(glm::vec4(1, 0, 1, 1));
					p->Color(glm::vec4(1, 0, 0, 1));
					p->Scale(glm::vec3(0.5f));
					timer = shotInterval;
				}
			}
		}

	}

}