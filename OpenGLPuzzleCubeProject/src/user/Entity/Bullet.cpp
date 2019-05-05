/**
*	@file Bullet.cpp
*/
#include "Bullet.h"


namespace GameState {


	/**
	*	Ž©‹@‚Ì’e‚Ì‰Šú‰»ˆ—
	*/
	void PlayerShot::Initialize() {

		entity->CastShadow(false);

		entity->Velocity(glm::vec3(0, 0, 40));
	}

	/**
	*	Ž©‹@‚Ì’e‚ÌXVˆ—
	*/
	void PlayerShot::Update(double delta) {

		const glm::vec3 pos = entity->Position();
		if (std::abs(pos.x) > 40 || pos.z < -4 || pos.z > 40) {
			entity->Destroy();
			return;
		}
	}

	void PlayerShot::CollisionEnter(Entity::Entity& e) {

		entity->Destroy();
	}




}
