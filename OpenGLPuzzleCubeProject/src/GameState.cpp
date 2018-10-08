/**
*	@file TitleState.cpp
*/
#include "GameState.h"
#include "GameEngine.h"
#include "../Res/Audio/SampleCueSheet.h"

namespace GameState {

	///�w�i�̍X�V
	void UpdateSpaceSphere(Entity::Entity& entity, double delta) {

		glm::vec3 rotSpace = glm::eulerAngles(entity.Rotation());
		rotSpace.x += static_cast<float>(glm::radians(2.5) * delta);
		entity.Rotation(rotSpace);
	}

	///�^�C�g����ʂ̍X�V
	void Title::operator()(double delta) {

		GameEngine& game = GameEngine::Instance();
		game.Camera({ glm::vec4(0,20,-8,1),glm::vec3(0,0,12),glm::vec3(0,0,1) });
		game.KeyValue(0.2);

		if (initial) {
			initial = false;
			game.RemoveAllEntity();
			game.ClearLevel();
			game.LoadMeshFromFile("Res/Model/SpaceSphere.fbx");
			game.LoadTextureFromFile("Res/Model/SpaceSphere.bmp");
			auto e = game.AddEntity(EntityGroupId_Background, glm::vec3(0, 0, 0), 
				"SpaceSphere", "Res/Model/SpaceSphere.bmp", &UpdateSpaceSphere, "NonLighting");
		}


		const float offset = timer == 0 ? 0 : (2.0f - timer) * (2.0f - timer) * 2.0f;
		game.FontColor(glm::vec4(1, 1, 1, 1));
		game.FontScale(glm::vec2(2));
		game.AddString(glm::vec2(-0.25f + offset, 0.125f), "STAR FIGHTER");
		game.FontScale(glm::vec2(0.5f));
		game.AddString(glm::vec2(0.1f + offset, -0.05f), "The ultimate in Manned-Fighter");

		static float tmpTimer= 0;
		tmpTimer += static_cast<float>(delta);
		auto f = (sinf(glm::radians((float)tmpTimer)) + 1) / 2;

		game.FontColor(glm::vec4(1.0f,0, 0, 0));
		game.AddString(glm::vec2(-0.2, -0.5), "Pressed Enter...");

		
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