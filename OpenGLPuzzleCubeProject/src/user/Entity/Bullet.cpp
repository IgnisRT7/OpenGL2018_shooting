/**
*	@file Bullet.cpp
*/
#include "Bullet.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../GameState.h"

namespace GameState {


	/**
	*	弾の初期化処理
	*/
	void Bullet::Initialize() {
		
		entity->CastShadow(false);
		entity->Collision(GameState::collisionDataList[EntityGroupId_EnemyShot]);
	}

	/**
	*	弾の更新処理
	*
	*	@param delta	経過時間
	*/
	void Bullet::Update(float delta) {

		//弾の向きの調整処理

		glm::vec3 normalizeVel = glm::length(entity->Velocity()) > 0.05f ? glm::normalize(entity->Velocity()) : glm::vec3(0,0,-1);
		glm::vec3 forward = glm::vec3(0, 0, 1);
		float costheta = glm::dot(normalizeVel, forward);
		float sintheta = acosf(costheta);
		glm::vec3 axis = glm::cross(glm::vec3(1,0,0), forward);

		auto rot = glm::angleAxis(glm::degrees(sintheta), axis);
		entity->Rotation(rot);

		entity->Velocity(velocity);

		//範囲外処理
		const glm::vec3 pos = entity->Position();
		if (std::abs(pos.x) > screenHalfW || std::abs(pos.z) > screenHalfH) {
			entity->Destroy();
			return;
		}
	}
	
	/**
	*	衝突処理
	*
	*	@param e	衝突してきたエンティティ
	*/
	void Bullet::CollisionEnter(Entity::Entity& e) {

		entity->Destroy();
		e.EntityData()->Damage(1);
	}

}
