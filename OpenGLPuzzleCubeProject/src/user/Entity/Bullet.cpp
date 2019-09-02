/**
*	@file	Bullet.cpp
*/

#include "Bullet.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../GameState.h"

namespace GameState {

	void Bullet::Initialize() {
		
		entity->CastShadow(false);
		entity->Collision(GameState::collisionDataList[EntityGroupId_EnemyShot]);
	}

	void Bullet::Update(float deltaTime) {

		//’e‚ÌŒü‚«‚Ì’²®ˆ—

		glm::vec3 normalizeVel = glm::length(entity->Velocity()) > 0.05f ? glm::normalize(entity->Velocity()) : glm::vec3(0,0,-1);
		glm::vec3 forward = glm::vec3(0, 0, 1);
		float costheta = glm::dot(normalizeVel, forward);
		float sintheta = acosf(costheta);
		glm::vec3 axis = glm::cross(glm::vec3(1,0,0), forward);

		auto rot = glm::angleAxis(glm::degrees(sintheta), axis);
		entity->Rotation(rot);

		entity->Velocity(velocity);

		//”ÍˆÍŠOˆ—
		const glm::vec3 pos = entity->Position();
		if (std::abs(pos.x) > screenHalfW || std::abs(pos.z) > screenHalfH) {
			entity->Destroy();
			return;
		}
	}
	
	void Bullet::CollisionEnter(Entity::Entity& e) {

		entity->Destroy();
		e.EntityData()->Damage(1);
	}

}
