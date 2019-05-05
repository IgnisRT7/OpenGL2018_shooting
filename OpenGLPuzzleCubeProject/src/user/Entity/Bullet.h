/**
*	@file Bullet.h
*/
#pragma once

#include "../../Entity.h"

namespace GameState {


	/**
	*	自機の弾の更新
	*/
	class PlayerShot : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& e) override;

	private:

		//glm::vec3 movVec = { 0,0,80 };
	};


}
