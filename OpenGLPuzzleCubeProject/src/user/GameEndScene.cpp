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
	*	èâä˙âªèàóù
	*/
	bool GameState::GameEnd::Initialize(){

		GameEngine& game = GameEngine::Instance();

		game.LoadMeshFromFile("Res/Model/SpaceSphere.fbx");
		game.LoadTextureFromFile("Res/Model/SpaceSphere.dds");

		auto e = game.AddEntity(EntityGroupId_Background, glm::vec3(0, 0, 0),
			"SpaceSphere", "Res/Model/SpaceSphere.dds", std::make_shared<TitleSpaceSphere>(), "NonLightin44g");
		game.KeyValue(0.01f);

		game.MainCamera(std::make_shared<CameraComponent>());
		game.MainCamera()->LookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

		return true;
	}

	/**
	*	çXêVèàóù
	*/
	void GameEnd::Update(float delta){

		GameEngine& game = GameEngine::Instance();

		const float offset = timer == 0 ? 0 : (2.0f - timer) * (2.0f - timer) * 2.0f;
		game.FontColor(glm::vec4(1, 0, 0, 1));
		game.FontScale(glm::vec2(5));
		game.AddString(glm::vec2(-0.55, 0.3), "GAME OVER...");

		char str[16];
		snprintf(str, 16, "SCORE : %08.0f", game.UserVariable("score"));
		game.FontColor(glm::vec4(0, 0, 0, 1));
		game.FontScale(glm::vec2(5));
		game.AddString(glm::vec2(-0.4, -0.2), str);

		game.FontColor(glm::vec4(1, 1, 1, 1));
		game.FontScale(glm::vec2(2));
		game.AddString(glm::vec2(-0.4, -0.5), "Pressed enter to title...");

		auto gamepad = game.GetGamePad();

		if (timer > 0) {
			timer -= delta;
			if (timer <= 0) {

				game.PopScene();
			}
		}
		else if (game.GetGamePad().buttonDown & GamePad::START) {
			game.PlayAudio(1, CRI_SAMPLECUESHEET_START);
			timer = 2;
		}
	}

	/**
	*	èIóπèàóù
	*/
	void GameEnd::Finalize(){

		GameEngine& game = GameEngine::Instance();

		game.ClearCollisionHandlerList();
		game.RemoveAllEntity();
		game.ClearLevel();
	}

}