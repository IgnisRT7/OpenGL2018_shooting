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
		EntityGroupId_Others
	};

	///�^�C�g�����
	class Title {
	public:

		explicit Title(Entity::Entity* p = nullptr) : pSpaceSphere(p) {}
		void operator()(double delta);

	private:

		Entity::Entity* pSpaceSphere;
		float timer = 0;

	};

	///���C���Q�[�����
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
