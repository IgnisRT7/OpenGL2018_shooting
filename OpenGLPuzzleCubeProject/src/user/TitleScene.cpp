/**
*	@file TitleScene.cpp
*	@brief	タイトルシーン制御用
*	@author Takuya Yokoyama
*/

#include "../Engine/entity.h"
#include "TitleScene.h"
#include "../Engine/GameEngine.h"
#include "../GameState.h"
#include "GameMainScene.h"
#include "ModelLoadTestscene.h"
#include "../../Res/Resource.h"


#include "../../Res/Audio/testProject_acf.h"
#include "../../Res/Audio/CueSheet_0.h"

namespace Application {

	/**
	*	タイトル画面の初期化
	*/
	bool Title::Initialize() {
		
		GameEngine& game = GameEngine::Instance();

		
		game.LoadMeshFromFile(Resource::fbx_spaceSphere);
		game.LoadTextureFromFile(Resource::tex_spaceSphere);
		game.LoadMeshFromFile(Resource::fbx_player);
		game.LoadTextureFromFile(Resource::tex_player);

		return true;
	}

	/**
	*	終了処理
	*/
	void Title::Finalize() {

	}

	/**
	*	開始処理
	*/
	void Title::Play() {

		GameEngine& game = GameEngine::Instance();

		timer = 0;

		game.AddEntity(GameState::EntityGroupId_Background, glm::vec3(0, 0, 0),
			"SpaceSphere", Resource::tex_spaceSphere, std::make_shared<SpaceSphereMain>(), "NonLighting");

		player = std::make_shared<PlayerForProduction>();
		game.AddEntity(GameState::EntityGroupId_Others, glm::vec3(0, 0, 0), "Aircraft", Resource::tex_player, player, "NonLighting");

		game.KeyValue(0.01f);

		game.MainCamera(std::make_shared<CameraComponent>());
		game.MainCamera()->LookAt(glm::vec3(0, 20, -40), glm::vec3(0, 0, -1));


		game.PlayAudio(1, CRI_CUESHEET_0_TITLE1);
	}

	/**
	*	停止処理
	*/
	void Title::Stop() {

	}

	/*
	*	タイトル画面の更新
	*
	*	@param delta 経過時間
	*/
	void Title::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		const float offset = timer == 0 ? 0 : (3.0f - timer) * (3.0f - timer) * 3.0f;
		game.FontColor(glm::vec4(1, 1, 1, 1));
		game.FontScale(glm::vec2(7));
		game.AddString(glm::vec2(offset, 0.25), "STAR FIGHTER", true);
		game.FontScale(glm::vec2(2));
		game.AddString(glm::vec2(offset - 0.15, 0), "The ultimate in Manned-Fighter");

		//game.AddSprite(*sampleSprite);

		static float tmpTimer = 0;
		tmpTimer += delta;
		auto f = (sinf(glm::radians((float)tmpTimer)) + 1) / 2;

		if (timer == 0) {

			game.FontColor(glm::vec4(1.0f, 0, 0, 1));
			game.FontScale(glm::vec2(2));
			game.AddString(glm::vec2(0, -0.7), "Pressed Enter...",true);

			game.FontColor(glm::vec4(1));
			game.FontScale(glm::vec2(3.0f));
			game.AddString(glm::vec2(0, -0.5), "<            >",true);
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
		else if (game.GetGamePad().buttonDown & GamePad::R) {
			game.PushScene(std::make_shared<ModelLoadTestScene>());
		}

		if (timer == 0) {

			///機種の変更に関する処理
			int select = 0;
			select += gamepad.buttonDown & GamePad::DPAD_RIGHT ? 1 : 0;
			select += gamepad.buttonDown & GamePad::DPAD_LEFT ? -1 : 0;

			if (select != 0) {

				glm::vec4 selectColor = glm::vec4(1);
				selectAirCraftType = (select + 1) % 3;
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

}