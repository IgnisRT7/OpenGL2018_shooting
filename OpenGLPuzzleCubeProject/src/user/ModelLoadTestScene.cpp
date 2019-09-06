/**
*	@file ModelLoadTestScene.cpp
*	@brief	テスト用シーン制御用
*	@author	Takuya Yokoyama
*/
#include "ModelLoadTestscene.h"
#include "../Engine/GameEngine.h"
#include "../GameState.h"
#include <glm/common.hpp>

namespace Application {

	ModelLoadTestScene::ModelLoadTestScene() :Scene("ModelLoadTest") {

	}

	bool ModelLoadTestScene::Initialize() {

		GameEngine& game = GameEngine::Instance();
		game.LoadMeshFromFile("Res/Model/Landscape.fbx");
		game.LoadMeshFromFile("Res/Model/sampleField.fbx");
		game.LoadTextureFromFile("Res/Model/BG02.Diffuse.dds");
		game.LoadTextureFromFile("Res/Model/BG02.Normal.bmp");

		return false;
	}

	void ModelLoadTestScene::Finalize() {
	}

	void ModelLoadTestScene::Play() {

		GameEngine& game = GameEngine::Instance();

		/// ライト・輝度の設定
		game.AmbientLight(glm::vec4(0.05f, 0.1f, 0.2f, 1));
		game.Light(0, { glm::vec4(1,100,1,1),glm::vec4(12000,12000,12000,1) });
		game.KeyValue(0.2f);

		/// シャドウの設定
		GameEngine::ShadowParameter shadowParam;
		shadowParam.lightPos = glm::vec3(20, 50, 50);
		shadowParam.lightDir = glm::normalize(glm::vec3(-25, -50, 25));
		shadowParam.lightUp = glm::vec3(0, 1, 0);
		shadowParam.near = 10;
		shadowParam.far = 200;
		shadowParam.range = glm::vec2(300, 300);
		game.Shadow(shadowParam);


		///背景の更新処理
		for (int z = 0; z < 5; ++z) {
			const float offsetZ = static_cast<float>(z * 40 * 5);

			for (int x = 0; x < 5; ++x) {
				const float offsetX = static_cast<float>(x * 40 - 80) * 5.0f;
				game.AddEntity(GameState::EntityGroupId_Background, glm::vec3(offsetX, -300.0, offsetZ),
					"Landscape01", "Res/Model/BG02.Diffuse.dds", "Res/Model/BG02.Normal.bmp", nullptr);
			}
		}

		///カメラの設定
		game.MainCamera(std::static_pointer_cast<CameraComponent>(std::make_shared<CameraDebugComponent>()));


		auto p = game.AddEntity(GameState::EntityGroupId_Player, glm::vec3(0, 0, 0), "Cube", "Res/Model/BG02.Diffuse.dds", nullptr);

	}

	void ModelLoadTestScene::Stop() {
	}

	void ModelLoadTestScene::Hide() {
	}

	void ModelLoadTestScene::Update(float delta) {
	}
}