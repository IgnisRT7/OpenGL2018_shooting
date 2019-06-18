/**
*	@file Bullet.h
*/
#pragma once

#include "../../Engine/Entity.h"

namespace GameState {

	/// 弾のエンティティクラス
	class Bullet : public Entity::EntityDataBase {
	public:
		
		Bullet(glm::vec3 v = glm::vec3(0, 0, 1)) :velocity(v) {}
		void Initialize() override;
		void Update(float delta) override;	
		void CollisionEnter(Entity::Entity& e) override;

		glm::vec3 Velocity() const { return velocity; }
		void Velocity(glm::vec3& v) { velocity = v; }

	private:

		glm::vec3 velocity;

		//Entity::Entity* target;
	};


}
