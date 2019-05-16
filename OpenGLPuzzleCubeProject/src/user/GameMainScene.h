/**
*	@file GameMainScene.h
*/
#pragma once
#include "../Entity.h"
#include "Entity/Player.h"


namespace GameState {

	///”wŒi(ƒXƒ‰ƒCƒh)
	class Landscape : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(double delta) override;
	};

	///”wŒi‚ÌXV(‰ñ“])
	class SpaceSphereMain : public Entity::EntityDataBase {

		void Initialize() override;

		void Update(double delta) override;
	};
	
	///ƒƒCƒ“ƒQ[ƒ€‰æ–Ê
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