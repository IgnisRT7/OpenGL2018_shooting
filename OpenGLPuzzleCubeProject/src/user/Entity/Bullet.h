/**
*	@file Bullet.h
*/
#pragma once

#include "../../Entity.h"

namespace GameState {

	/// ’eƒNƒ‰ƒX
	class Bullet : public Entity::EntityDataBase {
	public:
		
		Bullet(glm::vec3 v = glm::vec3(0, 0, 1), Entity::Entity* t = nullptr) :velocity(v), target(t) {}

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& e) override;

	private:

		glm::vec3 velocity;
		Entity::Entity* target;
	};


}
