/**
*	@file Item.cpp
*/

#include "Item.h"
#include "../../GameEngine.h"

namespace GameState {

	/**
	*	�A�C�e���̏���������
	*/
	void Item::Initialize() {
		//entity->Scale({ 50,50,50 });
		entity->Velocity({ 0,0,-5 });
		entity->Color({ 1,0,0,1 });
	}

	/**
	*	�A�C�e���̍X�V����
	*/
	void Item::Update(double delta) {

		glm::vec3 pos = entity->Position();

		//��ʊO���菈��
		if (std::abs(pos.x) > 200.0f || std::abs(pos.z) > 200.0f) {
			GameEngine::Instance().RemoveEntity(entity);
			return;
		}
	}

	/**
	*	�A�C�e���̏Փ˔��菈��
	*/
	void Item::CollisionEnter(Entity::Entity& e) {

		auto p = e.EntityData();

		entity->Destroy();

	}

}