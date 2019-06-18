/**
*	@file Item.cpp
*/

#include "Item.h"
#include "../../Engine/GameEngine.h"

namespace GameState {

	/**
	*	�A�C�e���̏���������
	*/
	void Item::Initialize() {
		entity->Scale({ 0.8f,0.8f,0.8f });
		entity->Velocity({ 0,0,-5 });
		entity->Color({ 1,1,1,1 });
		//entity->Rotation(glm::angleAxis(90.0f, glm::vec3(0, 0, 1)));
	}

	/**
	*	�A�C�e���̍X�V����
	*
	*	@param  delta	�o�ߎ���
	*/
	void Item::Update(float delta) {

		//���邭���]������
		glm::quat addR = glm::angleAxis(5 * delta, glm::vec3(-1, 0, 0));
		entity->Rotation(entity->Rotation() * addR);

		glm::vec3 pos = entity->Position();

		//��ʊO���菈��
		if (std::abs(pos.x) > 200.0f || std::abs(pos.z) > 200.0f) {
			GameEngine::Instance().RemoveEntity(entity);
			return;
		}
	}

	/**
	*	�A�C�e���̏Փ˔��菈��
	*
	*	@param e	�Փ˂��Ă����G���e�B�e�B
	*/
	void Item::CollisionEnter(Entity::Entity& e) {

		auto p = e.EntityData();

		entity->Destroy();

	}

}