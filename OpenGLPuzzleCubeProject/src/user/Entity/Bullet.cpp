/**
*	@file Bullet.cpp
*/
#include "Bullet.h"


namespace GameState {


	/**
	*	���@�̒e�̏���������
	*/
	void Bullet::Initialize() {

		entity->CastShadow(false);

		entity->Velocity(target ? target->Position() - entity->Position() : velocity);

	}

	/**
	*	���@�̒e�̍X�V����
	*/
	void Bullet::Update(double delta) {


		//�͈͊O����
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
