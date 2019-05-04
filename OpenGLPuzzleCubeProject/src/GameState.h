/**
*	@file GameState.h
*/
#pragma once
#include "Entity.h"

namespace GameState {

	///�G���e�B�e�B�̏Փ˃O���[�vID
	enum EntityGroupId {

		EntityGroupId_Background,
		EntityGroupId_Player,
		EntityGroupId_PlayerShot,
		EntityGroupId_Enemy,
		EntityGroupId_EnemyShot,
		EntityGroupId_Item,
		EntityGroupId_Others
	};

	///�Փˌ`�󃊃X�g
	static const Entity::CollisionData collisionDataList[] = {
		{ glm::vec3( 0, 0, 0), glm::vec3(0, 0, 0) },
		{ glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f,1.0f,1.0f) },
		{ glm::vec3(-0.5f,-0.5f,-1.0f), glm::vec3(0.5f,0.5f,1.0f) },
		{ glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f,1.0f,1.0f) },
		{ glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.25f,0.25f,0.25f) },
		{ glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f,1.0f,1.0f) },
	};

}

