/**
*	@file GameMainScene.h
*/
#pragma once
#include "../Entity.h"
#include "Entity/Player.h"


namespace GameState {

	///�w�i(�X���C�h)
	class Landscape : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(double delta) override;
	};

	///�w�i�̍X�V(��])
	class SpaceSphereMain : public Entity::EntityDataBase {

		void Initialize() override;

		void Update(double delta) override;
	};
	
	///���C���Q�[�����
	class MainGame {
	public:

		MainGame();
		void operator()(double delta);

	private:

		double stageTimer = -1;
		double interval = 0;

		float sceneTimer=0;
		int stageNo = 0;

		std::shared_ptr<Player> playerData;


	};

}