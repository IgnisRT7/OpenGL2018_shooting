/**
*	@file Bullet.cpp
*/
#include "Bullet.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../GameState.h"

namespace GameState {


	/**
	*	���@�̒e�̏���������
	*/
	void Bullet::Initialize() {

		entity->CastShadow(false);
		entity->Collision(GameState::collisionDataList[EntityGroupId_EnemyShot]);

		entity->Velocity(target ? glm::normalize(target->Position() - entity->Position()) * glm::length(velocity) : velocity);

		glm::mat4 r = glm::lookAt(glm::vec3(0), -velocity, glm::vec3(0, 1, 0));
		entity->Rotation(glm::quat_cast(r));
	}

	/**
	*	���@�̒e�̍X�V����
	*
	*	@param delta	�o�ߎ���
	*/
	void Bullet::Update(float delta) {



		//�͈͊O����
		const glm::vec3 pos = entity->Position();
		if (std::abs(pos.x) > 40 || pos.z < -4 || pos.z > 40) {
			entity->Destroy();
			return;
		}
	}

	/**
	*	�Փˏ���
	*
	*	@param e	�Փ˂��Ă����G���e�B�e�B
	*/
	void Bullet::CollisionEnter(Entity::Entity& e) {

		entity->Destroy();
		e.EntityData()->Damage(1);
	}




}
