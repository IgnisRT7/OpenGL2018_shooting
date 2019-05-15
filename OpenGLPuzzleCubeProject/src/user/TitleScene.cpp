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

	///�@�w�i���N���X��`

	void TitleSpaceSphere::Initialize() {
	}

	void TitleSpaceSphere::Update(double delta) {

		glm::vec3 rotSpace = glm::eulerAngles(entity->Rotation());
		rotSpace.x += static_cast<float>(glm::radians(2.5) * delta);
		entity->Rotation(rotSpace);
	}

	/// �^�C�g���N���X��`

	Title::Title() {
		Initialize();
	}

	/**
	*	�^�C�g����ʂ̏�����
	*/
	void Title::Initialize() {
		
		GameEngine& game = GameEngine::Instance();

		game.RemoveAllEntity();
		game.ClearLevel();
		game.LoadMeshFromFile("Res/Model/SpaceSphere.fbx");
		game.LoadTextureFromFile("Res/Model/SpaceSphere.dds");

		auto e = game.AddEntity(EntityGroupId_Background, glm::vec3(0, 0, 0),
			"SpaceSphere", "Res/Model/SpaceSphere.dds", std::make_shared<TitleSpaceSphere>(), "NonLighting");
		game.KeyValue(0.01f);

		game.Camera(0, { glm::vec4(0,20,-8,1),glm::vec3(0,0,12),glm::vec3(0,1,0) });
	}

	/*
	*	�^�C�g����ʂ̍X�V
	*
	*	@param delta �o�ߎ���
	*/
	void Title::operator()(double delta) {

		GameEngine& game = GameEngine::Instance();

		const float offset = timer == 0 ? 0 : (2.0f - timer) * (2.0f - timer) * 2.0f;
		game.FontColor(glm::vec4(1, 1, 1, 1));
		game.FontScale(glm::vec2(4));
		game.AddString(glm::vec2(-0.5 + offset, 0.3), "STAR FIGHTER");
		game.FontScale(glm::vec2(1));
		game.AddString(glm::vec2(-0.05f + offset, 0), "The ultimate in Manned-Fighter");

		static float tmpTimer = 0;
		tmpTimer += static_cast<float>(delta);
		auto f = (sinf(glm::radians((float)tmpTimer)) + 1) / 2;

		game.FontColor(glm::vec4(1.0f, 0, 0, 0));
		game.FontScale(glm::vec2(2));
		game.AddString(glm::vec2(-0.3, -0.5), "Pressed Enter...");

		
		auto gamepad = game.GetGamePad();

		if (gamepad.buttonDown & GamePad::B) {

			game.KeyValue(glm::min(1.0, game.KeyValue() + delta));
		}
		else if (gamepad.buttonDown & GamePad::X) {

			game.KeyValue(glm::max(0.0, game.KeyValue() - delta));
		}


		if (timer > 0) {
			timer -= static_cast<float>(delta);
			if (timer <= 0) {

				//�^�C�g����ʂ��烁�C���Q�[����ʂ̍X�V�����ֈڍs
				game.UpdateFunc(MainGame());

			}
		}
		else if (game.GetGamePad().buttonDown & GamePad::START) {
			game.PlayAudio(1, CRI_SAMPLECUESHEET_START);
			timer = 2;
		}
	}

}