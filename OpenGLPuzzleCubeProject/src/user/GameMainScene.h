/**
*	@file GameMainScene.h
*/
#pragma once
#include "../Entity.h"
#include "Entity/Player.h"
#include "../Scene.h"
#include "EnemyLaunchController.h"

namespace GameState {


	///”wŒi(ƒXƒ‰ƒCƒh)
	class Landscape : public Entity::EntityDataBase {
	public:

		Landscape() = default;
		Landscape(bool l);

		void Initialize() override;

		void Update(float delta) override;
	private:

		float timer = 0;
		glm::vec3 start;
		bool isLoop = false;
	};

	///”wŒi‚ÌXV(‰ñ“])
	class SpaceSphereMain : public Entity::EntityDataBase {

		void Initialize() override;

		void Update(float delta) override;
	};
	


	///ƒƒCƒ“ƒQ[ƒ€‰æ–Ê
	class MainGame : public Scene{
	public:

		MainGame() :Scene("MainGame") {}

		bool Initialize() override;
		void Update(float d) override;
		void Finalize() override;
		void Play() override;
		void Stop() override ;
		void Hide() override {};

		void SelectPlayerType(int t) { playerType = t; }
		void DrawScreenInfo();
		void StageLoad();

	private:

		float stageTimer = -1;

		float sceneTimer=0;

		int stageNo = 0;
		int playerType = 0;

		struct FontDrawInfo {
			glm::vec2 pos;
			glm::vec2 size;
			glm::vec4 color;
			std::string str;
		}stageName;
		float stageNameFadeTimer = 0;

		std::shared_ptr<Player> playerData;


		EnemyLaunchControllerPtr launchController;
	};

}