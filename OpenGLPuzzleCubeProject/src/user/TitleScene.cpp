/**
*	@file TitleScene.cpp
*/

#include "../entity.h"
#include "TitleScene.h"
#include "../GameEngine.h"
#include "../GameState.h"
#include "GameMainScene.h"
#include "ModelLoadTestscene.h"
#include "GameEndScene.h"

//#include "../../Res/Audio/SampleSound_acf.h"
//#include "../../Res/Audio/SampleCueSheet.h"
#include "../../Res/Audio/testProject_acf.h"
#include "../../Res/Audio/CueSheet_0.h"

namespace GameState {

	///　背景球クラス定義

	void TitleSpaceSphere::Initialize() {
	}

	void TitleSpaceSphere::Update(float delta) {

		glm::vec3 rotSpace = glm::eulerAngles(entity->Rotation());
		rotSpace.x += glm::radians(2.5f) * delta;
		entity->Rotation(rotSpace);
	}

	/// タイトルクラス定義


	/**
	*	タイトル画面の初期化
	*/
	bool Title::Initialize() {
		
		GameEngine& game = GameEngine::Instance();
		game.ClearLevel();

		game.LoadMeshFromFile("Res/Model/SpaceSphere.fbx");
		game.LoadTextureFromFile("Res/Model/SpaceSphere.dds");
		game.LoadMeshFromFile("Res/Model/Player.fbx");
		game.LoadTextureFromFile("Res/Model/Player.dds");

		initialize = true;

		return true;
	}

	/*
	*	タイトル画面の更新
	*
	*	@param delta 経過時間
	*/
	void Title::Update(float delta){

		GameEngine& game = GameEngine::Instance();

		const float offset = timer == 0 ? 0 : (3.0f - timer) * (3.0f - timer) * 3.0f;
		game.FontColor(glm::vec4(1, 1, 1, 1));
		game.FontScale(glm::vec2(4));
		game.AddString(glm::vec2(-0.5 + offset, 0.3), "STAR FIGHTER");
		game.FontScale(glm::vec2(1));
		game.AddString(glm::vec2(-0.05f + offset, 0), "The ultimate in Manned-Fighter");

		static float tmpTimer = 0;
		tmpTimer += delta;
		auto f = (sinf(glm::radians((float)tmpTimer)) + 1) / 2;
		
		if (timer == 0) {

			game.FontColor(glm::vec4(1.0f, 0, 0, 0));
			game.FontScale(glm::vec2(2));
			game.AddString(glm::vec2(-0.3, -0.7), "Pressed Enter...");

			game.FontColor(glm::vec4(1));
			game.FontScale(glm::vec2(3.0f));
			game.AddString(glm::vec2(-0.3, -0.5), "<            >");
		}

		auto gamepad = game.GetGamePad();
		///シーン移行に関する処理
		if (timer > 0) {
			timer -= delta;
			if (timer <= 0) {

				//タイトル画面からメインゲーム画面の更新処理へ移行
				auto maingame = std::make_shared<MainGame>();
				maingame->SelectPlayerType(selectAirCraftType);

				game.PushScene(maingame);
				//game.PushScene(std::make_shared<GameEnd>());
				return;
			}
		}
		else if (game.GetGamePad().buttonDown & GamePad::START) {
			game.PlayAudio(1, CRI_CUESHEET_0_SELECT2);
			timer = 3;
			player->MoveStart();
			game.SceneFadeStart(true);

		}

		if (timer == 0) {

			///機種の変更に関する処理
			int select = 0;
			select += gamepad.buttonDown & GamePad::DPAD_RIGHT ? 1 : 0;
			select += gamepad.buttonDown & GamePad::DPAD_LEFT ? -1 : 0;

			if (select != 0) {

				glm::vec4 selectColor = glm::vec4(1);
				selectAirCraftType = (selectAirCraftType + 1) % 3;
				switch (selectAirCraftType) {
				case 0:
					selectColor = glm::vec4(1); break;
				case 1:
					selectColor = glm::vec4(1, 0, 0, 1); break;
				case 2:
					selectColor = glm::vec4(0, 0, 1, 1); break;
				default:break;
				}

				player->Color(selectColor);
				game.PlayAudio(1, CRI_CUESHEET_0_AIRCRAFTSELECT);
			}
		}

	}

	/**
	*	終了処理
	*/
	void Title::Finalize(){

		GameEngine::Instance().RemoveAllEntity();
	}

	/**
	*	開始処理
	*/
	void Title::Play(){

		GameEngine& game = GameEngine::Instance();

		timer = 0;

		game.AddEntity(EntityGroupId_Background, glm::vec3(0, 0, 0),
			"SpaceSphere", "Res/Model/SpaceSphere.dds", std::make_shared<TitleSpaceSphere>(), "NonLighting");

		player = std::make_shared<PlayerForProduction>();
		game.AddEntity(EntityGroupId_Others, glm::vec3(0, 0, 0), "Aircraft", "Res/Model/Player.dds", player, "NonLighting");

		game.KeyValue(0.01f);

		game.MainCamera(std::make_shared<CameraComponent>());
		game.MainCamera()->LookAt(glm::vec3(0, 20, -40), glm::vec3(0, 0, -1));

		game.PlayAudio(1, CRI_CUESHEET_0_TITLE1);
	}

	/**
	*	停止処理
	*/
	void Title::Stop(){

		GameEngine& game = GameEngine::Instance();
		game.StopAllAudio();
	}

}