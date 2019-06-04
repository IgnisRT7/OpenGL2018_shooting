/**
*	@file GameMainScene.h
*/
#pragma once
#include "../Entity.h"
#include "Entity/Player.h"
#include "../Scene.h"

namespace GameState {


	///�w�i(�X���C�h)
	class Landscape : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(float delta) override;
	};

	///�w�i�̍X�V(��])
	class SpaceSphereMain : public Entity::EntityDataBase {

		void Initialize() override;

		void Update(float delta) override;
	};
	


	///���C���Q�[�����
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

		int resourceCount = 0;
		float nextLoadTimer = 0;
		bool isResourceLoaded = false;

		double stageTimer = -1;
		double interval = 0;

		float sceneTimer=0;
		int stageNo = 0;

		int playerType = 0;

		std::shared_ptr<Player> playerData;
		float cameraChangeTimer = 3.0f;

	};

}