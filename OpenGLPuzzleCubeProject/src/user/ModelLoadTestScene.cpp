/**
*	@file ModelLoadTestScene.cpp
*/
#include "ModelLoadTestscene.h"
#include "../Engine/GameEngine.h"
#include "../GameState.h"
#include <glm/common.hpp>

ModelLoadTestScene::ModelLoadTestScene() :Scene("ModelLoadTest") {

}

/**
*	����������
*
*	@retval true	����������
*	@retval false	���������s
*/
bool ModelLoadTestScene::Initialize(){

	GameEngine& game = GameEngine::Instance();
	game.LoadMeshFromFile("Res/Model/Landscape.fbx");
	game.LoadMeshFromFile("Res/Model/sampleField.fbx");
	game.LoadTextureFromFile("Res/Model/BG02.Diffuse.dds");
	game.LoadTextureFromFile("Res/Model/BG02.Normal.bmp");

	return false;
}

/**
*	�I������
*/
void ModelLoadTestScene::Finalize(){
}

/**
*	�J�n����
*/
void ModelLoadTestScene::Play(){

	GameEngine& game = GameEngine::Instance();

	/// ���C�g�E�P�x�̐ݒ�
	game.AmbientLight(glm::vec4(0.05f, 0.1f, 0.2f, 1));
	game.Light(0, { glm::vec4(1,100,1,1),glm::vec4(12000,12000,12000,1) });
	game.KeyValue(0.2f);

	/// �V���h�E�̐ݒ�
	GameEngine::ShadowParameter shadowParam;
	shadowParam.lightPos = glm::vec3(20, 50, 50);
	shadowParam.lightDir = glm::normalize(glm::vec3(-25, -50, 25));
	shadowParam.lightUp = glm::vec3(0, 1, 0);
	shadowParam.near = 10;
	shadowParam.far = 200;
	shadowParam.range = glm::vec2(300, 300);
	game.Shadow(shadowParam);


	///�w�i�̍X�V����
	for (int z = 0; z < 5; ++z) {
		const float offsetZ = static_cast<float>(z * 40 * 5);

		for (int x = 0; x < 5; ++x) {
			const float offsetX = static_cast<float>(x * 40 - 80) * 5.0f;
			game.AddEntity(GameState::EntityGroupId_Background, glm::vec3(offsetX, -300.0, offsetZ),
				"Landscape01", "Res/Model/BG02.Diffuse.dds", "Res/Model/BG02.Normal.bmp", nullptr);
		}
	}

	///�J�����̐ݒ�
	game.MainCamera(std::static_pointer_cast<CameraComponent>(std::make_shared<CameraDebugComponent>()));


	auto p = game.AddEntity(GameState::EntityGroupId_Player, glm::vec3(0, 0, 0), "Cube", "Res/Model/BG02.Diffuse.dds", nullptr);

}

/**
*	��~����
*/
void ModelLoadTestScene::Stop(){
}

/**
*	��\������
*/
void ModelLoadTestScene::Hide(){
}

/**
*	�X�V����
*
*	@param delta	�o�ߎ���
*/
void ModelLoadTestScene::Update(float delta){
}
