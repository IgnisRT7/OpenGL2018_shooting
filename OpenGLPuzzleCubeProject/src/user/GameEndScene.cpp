/**
*	@file GameEndScene.cpp
*/
#include "GameEndScene.h"
#include "../GameEngine.h"
#include "../GameState.h"
#include "TitleScene.h"

#include "../../Res/Audio/testProject_acf.h"
#include "../../Res/Audio/CueSheet_0.h"

namespace GameState{

	/**
	*	初期化処理
	*/
	bool GameState::GameEnd::Initialize(){

		GameEngine& game = GameEngine::Instance();

		game.LoadMeshFromFile("Res/Model/SpaceSphere.fbx");
		game.LoadTextureFromFile("Res/Model/SpaceSphere.dds");

		return true;
	}

	/**
	*	終了処理
	*/
	void GameEnd::Finalize() {

	}

	/**
	*	シーンの開始処理
	*/
	void GameEnd::Play() {

		GameEngine& game = GameEngine::Instance();


		auto e = game.AddEntity(EntityGroupId_Background, glm::vec3(0, 0, 0),
			"SpaceSphere", "Res/Model/SpaceSphere.dds", std::make_shared<TitleSpaceSphere>(), "NonLighting");
		game.KeyValue(0.01f);

		game.MainCamera(std::make_shared<CameraComponent>());
		game.MainCamera()->LookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

		game.PlayAudio(0, CRI_CUESHEET_0_GAMEOVER);
	}

	/**
	*	シーンの停止処理
	*/
	void GameEnd::Stop() {
	}


	/**
	*	更新処理
	*/
	void GameEnd::Update(float delta){

		GameEngine& game = GameEngine::Instance();


		const float offset = timer == 0 ? 0 : (2.0f - timer) * (2.0f - timer) * 2.0f;
		game.FontColor(glm::vec4(1, 0, 0, 1));
		game.FontScale(glm::vec2(5));
		game.AddString(glm::vec2(-0.5, 0.3), "GAME OVER...");

		char str[16];
		snprintf(str, 16, "SCORE : %08.0f", game.UserVariable("score"));
		game.FontColor(glm::vec4(0, 0, 0, 1));
		game.FontScale(glm::vec2(5));
		game.AddString(glm::vec2(-0.4, -0.2), str,true);

		game.FontColor(glm::vec4(1, 1, 1, 1));
		game.FontScale(glm::vec2(2));
		game.AddString(glm::vec2(-0.4, -0.5), "Pressed enter to title...",true);

		auto gamepad = game.GetGamePad();

		if (timer > 0) {
			timer -= delta;
			if (timer <= 0) {

				game.PopScene();
				return;
			}
		}
		else if (game.GetGamePad().buttonDown & GamePad::START) {
			game.PlayAudio(1, CRI_CUESHEET_0_SELECT);
			timer = 2;
		}
	}



}