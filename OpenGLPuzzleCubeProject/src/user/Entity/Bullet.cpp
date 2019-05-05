/**
*	@file Bullet.cpp
*/
#include "Bullet.h"


namespace GameState {


	/**
	*	自機の弾の初期化処理
	*/
	void PlayerShot::Initialize() {

		entity->CastShadow(false);

		entity->Velocity(glm::vec3(0, 0, 40));
	}

	/**
	*	自機の弾の更新処理
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
