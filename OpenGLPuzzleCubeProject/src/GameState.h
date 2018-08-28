/**
*	@file GameState.h
*/
#pragma once
#include "Entity.h"

namespace GameState {

	///エンティティの衝突グループID
	enum EntityGroupId {

		EntityGroupId_Background,
		EntityGroupId_Player,
		EntityGroupId_PlayerShot,
		EntityGroupId_Enemy,
		EntityGroupId_EnemyShot,
		EntityGroupId_Others
	};

	///タイトル画面
	class Title {
	public:

		explicit Title(Entity::Entity* p = nullptr) : pSpaceSphere(p) {}
		void operator()(double delta);

	private:

		Entity::Entity* pSpaceSphere;
		float timer = 0;

	};

	///メインゲーム画面
	class MainGame {
	public:

		explicit MainGame(Entity::Entity* p);
		void operator()(double delta);
		
	private:

		Entity::Entity* pSpaceSphere = nullptr;
		Entity::Entity* pPlayer = nullptr;
		double interval = 0;

	};

}
