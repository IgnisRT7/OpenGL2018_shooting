/**
*	@file Bullet.cpp
*/
#include "Bullet.h"


namespace GameState {


	/**
	*	自機の弾の初期化処理
	*/
	void Bullet::Initialize() {

		entity->CastShadow(false);

		entity->Velocity(target ? target->Position() - entity->Position() : velocity);

	}

	/**
	*	自機の弾の更新処理
	*/
	void Bullet::Update(double delta) {


		//範囲外処理
		const glm::vec3 pos = entity->Position();
		if (std::abs(pos.x) > 40 || pos.z < -4 || pos.z > 40) {
			entity->Destroy();
			return;
		}
	}

	void Bullet::CollisionEnter(Entity::Entity& e) {

		entity->Destroy();
		e.EntityData()->Damage(1);
	}




}
