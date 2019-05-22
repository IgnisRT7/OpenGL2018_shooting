/**
*	@file Bullet.cpp
*/
#include "Bullet.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../GameState.h"

namespace GameState {


	/**
	*	�e�̏���������
	*/
	void Bullet::Initialize() {
		\
		entity->CastShadow(false);
		entity->Collision(GameState::collisionDataList[EntityGroupId_EnemyShot]);

		entity->Velocity(target ? glm::normalize(target->Position() - entity->Position()) * glm::length(velocity) : velocity);
	}

	/**
	*	�e�̍X�V����
	*
	*	@param delta	�o�ߎ���
	*/
	void Bullet::Update(float delta) {

		glm::vec3 normalizeVel = glm::length(entity->Velocity()) > 0.05f ? glm::normalize(entity->Velocity()) : glm::vec3(0,0,1);
		glm::vec3 forward = glm::vec3(0, 0, 1);
		//���ς���� cos�� (�v���K��)
		float costheta = glm::dot(normalizeVel, forward);
		//acos �� sin�������
		float sintheta = acosf(costheta);
		//�O�ς�����Ď��x�N�g��(�v���K��)�����
		glm::vec3 axis = glm::cross(glm::vec3(1,0,0), forward);

		auto rot = glm::angleAxis(glm::degrees(sintheta), axis);
		entity->Rotation(rot);

		//entity->Rotation(glm::rotation(glm::vec3(0, 0, -1), glm::normalize(velocity)));

		//auto rotByForward = glm::angleAxis(10.0f, glm::normalize(velocity));
		//auto rot = glm::quat(glm::normalize(entity->Velocity()), glm::vec3(1, 0, 0));

		//entity->Rotation(rot);

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
