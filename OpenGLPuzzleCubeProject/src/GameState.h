/**
*	@file GameState.h
*/
#pragma once


#define WINDOW_SIZE_W	800
#define WINDOW_SIZE_H	600


namespace GameState {

	///エンティティの衝突グループID
	enum EntityGroupId {

		EntityGroupId_Background,
		EntityGroupId_Player,
		EntityGroupId_PlayerShot,
		EntityGroupId_Enemy,
		EntityGroupId_EnemyShot,
		EntityGroupId_Item,
		EntityGroupId_Others
	};

	///衝突形状リスト
	static const Entity::CollisionData collisionDataList[] = {
		{ glm::vec3(0, 0, 0), glm::vec3(0, 0, 0) },
		{ glm::vec3(-0.5f,-0.5f,-0.5f), glm::vec3(0.5f, 0.5f,0.5f) },
		{ glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f,1.0f,1.0f) },
		{ glm::vec3(-2.0f,-2.0f,-2.0f), glm::vec3(2.0f,2.0f,2.0f) },
		{ glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.25f,0.25f,0.25f) },
		{ glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f,1.0f,1.0f) },
	};

	static const float screenSizeW = 60.f;
	static const float screenSizeH = 60.f;
	static const float screenHalfW = screenSizeW * 0.5f;
	static const float screenHalfH = screenSizeH * 0.5f;
	
}

