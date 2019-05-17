/**
*	@file Bullet.cpp
*/
#include "Bullet.h"
#include <glm/gtc/matrix_transform.hpp>
#include "../../GameState.h"

namespace GameState {


	/**
	*	自機の弾の初期化処理
	*/
	void Bullet::Initialize() {

		entity->CastShadow(false);
		entity->Collision(GameState::collisionDataList[EntityGroupId_EnemyShot]);

		entity->Velocity(target ? glm::normalize(target->Position() - entity->Position()) * glm::length(velocity) : velocity);

		glm::mat4 r = glm::lookAt(glm::vec3(0), -velocity, glm::vec3(0, 1, 0));
		entity->Rotation(glm::quat_cast(r));
	}

	/**
	*	自機の弾の更新処理
	*
	*	@param delta	経過時間
	*/
	void Bullet::Update(float delta) {



		//範囲外処理
		const glm::vec3 pos = entity->Position();
		if (std::abs(pos.x) > 40 || pos.z < -4 || pos.z > 40) {
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
