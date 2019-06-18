/**
*	@file Item.cpp
*/

#include "Item.h"
#include "../../Engine/GameEngine.h"

namespace GameState {

	/**
	*	アイテムの初期化処理
	*/
	void Item::Initialize() {
		entity->Scale({ 0.8f,0.8f,0.8f });
		entity->Velocity({ 0,0,-5 });
		entity->Color({ 1,1,1,1 });
		//entity->Rotation(glm::angleAxis(90.0f, glm::vec3(0, 0, 1)));
	}

	/**
	*	アイテムの更新処理
	*
	*	@param  delta	経過時間
	*/
	void Item::Update(float delta) {

		//くるくる回転させる
		glm::quat addR = glm::angleAxis(5 * delta, glm::vec3(-1, 0, 0));
		entity->Rotation(entity->Rotation() * addR);

		glm::vec3 pos = entity->Position();

		//画面外判定処理
		if (std::abs(pos.x) > 200.0f || std::abs(pos.z) > 200.0f) {
			GameEngine::Instance().RemoveEntity(entity);
			return;
		}
	}

	/**
	*	アイテムの衝突判定処理
	*
	*	@param e	衝突してきたエンティティ
	*/
	void Item::CollisionEnter(Entity::Entity& e) {

		auto p = e.EntityData();

		entity->Destroy();

	}

}