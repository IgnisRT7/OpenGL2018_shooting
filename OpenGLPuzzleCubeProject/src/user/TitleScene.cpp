/**
*	@file TitleScene.cpp
*/

#include "../entity.h"
#include "TitleScene.h"
#include "../GameEngine.h"
#include "../GameState.h"
#include "GameMainScene.h"
#include "ModelLoadTestscene.h"

#include "../../Res/Audio/SampleSound_acf.h"
#include "../../Res/Audio/SampleCueSheet.h"

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

	Title::Title() {
		Initialize();
	}

	/**
	*	タイトル画面の初期化
	*/
	void Title::Initialize() {
		
		GameEngine& game = GameEngine::Instance();

		game.RemoveAllEntity();
		game.ClearLevel();
		game.LoadMeshFromFile("Res/Model/SpaceSphere.fbx");
		game.LoadTextureFromFile("Res/Model/SpaceSphere.dds");
		game.LoadMeshFromFile("Res/Model/Player.fbx");
		game.LoadTextureFromFile("Res/Model/Player.dds");

		game.KeyValue(0.01f);

		game.AddEntity(EntityGroupId_Background, glm::vec3(0, 0, 0),
			"SpaceSphere", "Res/Model/SpaceSphere.dds", std::make_shared<TitleSpaceSphere>(), "NonLighting");
		
		player = std::make_shared<PlayerForProduction>();
		game.AddEntity(EntityGroupId_Others, glm::vec3(0, 0, 0), "Aircraft", "Res/Model/Player.dds", player, "NonLighting");

		auto cam = std::make_shared<CameraDebugComponent>();
		game.MainCamera()->LookAt(glm::vec3(0, 10, -30), glm::vec3(0, -1, 1));
		game.MainCamera(std::static_pointer_cast<CameraComponent>(cam));
		
	}

	/*
	*	タイトル画面の更新
	*
	*	@param delta 経過時間
	*/
	void Title::operator()(float delta) {

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

		game.FontColor(glm::vec4(1.0f, 0, 0, 0));
		game.FontScale(glm::vec2(2));
		game.AddString(glm::vec2(-0.3, -0.5), "Pressed Enter...");

		
		auto gamepad = game.GetGamePad();

		static std::shared_ptr<MainGame> gameMain;

		if (timer > 0) {
			timer -= delta;
			if (timer <= 0) {

				gameMain = std::make_shared<MainGame>();
				//タイトル画面からメインゲーム画面の更新処理へ移行
				game.UpdateFunc(*gameMain);

			}
		}
		else if (game.GetGamePad().buttonDown & GamePad::START) {
			game.PlayAudio(1, CRI_SAMPLECUESHEET_START);
			timer = 3;
			player->MoveStart();
			game.SceneFadeStart(true);

		}
	}

}