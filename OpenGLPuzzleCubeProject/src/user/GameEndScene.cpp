/**
*	@file GameEndScene.cpp
*/
#include "GameEndScene.h"
#include "../GameEngine.h"
#include "../GameState.h"
#include "TitleScene.h"

#include "../../Res/Audio/SampleSound_acf.h"
#include "../../Res/Audio/SampleCueSheet.h"

namespace GameState{

	/**
	*	コンストラクタ
	*/
	GameEnd::GameEnd() {

	}

	/**
	*	初期化処理
	*/
	void GameState::GameEnd::Initialize(){

		GameEngine& game = GameEngine::Instance();

		game.RemoveAllEntity();
		game.ClearLevel();
		game.LoadMeshFromFile("Res/Model/SpaceSphere.fbx");
		game.LoadTextureFromFile("Res/Model/SpaceSphere.dds");

		auto e = game.AddEntity(EntityGroupId_Background, glm::vec3(0, 0, 0),
			"SpaceSphere", "Res/Model/SpaceSphere.dds", std::make_shared<TitleSpaceSphere>(), "NonLighting");
		game.KeyValue(0.01f);
	}

	/**
	*	更新処理
	*/
	void GameEnd::operator()(double delta){

		GameEngine& game = GameEngine::Instance();
		game.Camera(0, { glm::vec4(0,20,-8,1),glm::vec3(0,0,12),glm::vec3(0,0,1) });


		const float offset = timer == 0 ? 0 : (2.0f - timer) * (2.0f - timer) * 2.0f;
		game.FontColor(glm::vec4(1, 0, 0, 1));
		game.FontScale(glm::vec2(10));
		game.AddString(glm::vec2(-0.55, 0.3), "GAME OVER...");

		char str[16];
		snprintf(str, 16, "SCORE : %08.0f", game.UserVariable("score"));
		game.FontColor(glm::vec4(0, 0, 0, 1));
		game.FontScale(glm::vec2(5));
		game.AddString(glm::vec2(-0.4, -0.2), str);

		//game.FontColor(glm::vec4(1, 1, 1, 1));
		game.FontScale(glm::vec2(3));
		game.AddString(glm::vec2(-0.4, -0.5), "Pressed enter to title...");

		auto gamepad = game.GetGamePad();

		/*
		if (gamepad.buttonDown & GamePad::B) {

			game.KeyValue(glm::min(1.0, game.KeyValue() + delta));
		}
		else if (gamepad.buttonDown & GamePad::X) {

			game.KeyValue(glm::max(0.0, game.KeyValue() - delta));
		}*/


		if (timer > 0) {
			timer -= static_cast<float>(delta);
			if (timer <= 0) {

				game.UpdateFunc(Title());

			}
		}
		else if (game.GetGamePad().buttonDown & GamePad::START) {
			game.PlayAudio(1, CRI_SAMPLECUESHEET_START);
			timer = 2;
		}
	}

}