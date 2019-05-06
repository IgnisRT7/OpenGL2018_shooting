/**
*	@file Item.cpp
*/

#include "Item.h"
#include "../../GameEngine.h"

namespace GameState {

	/**
	*	アイテムの初期化処理
	*/
	void Item::Initialize() {
		entity->Scale({ 0.8f,0.8f,0.8f });
		entity->Velocity({ 0,0,-5 });
		entity->Color({ 1,1,1,1 });
	}

	/**
	*	アイテムの更新処理
	*/
	void Item::Update(double delta) {

		glm::vec3 pos = entity->Position();



		entity->Rotation();

		//画面外判定処理
		if (std::abs(pos.x) > 200.0f || std::abs(pos.z) > 200.0f) {
			GameEngine::Instance().RemoveEntity(entity);
			return;
		}
	}

	/**
	*	アイテムの衝突判定処理
	*/
	void Item::CollisionEnter(Entity::Entity& e) {

		auto p = e.EntityData();

		entity->Destroy();

	}

}