/**
*	@file ModelLoadTestScene.cpp
*/
#include "ModelLoadTestscene.h"
#include "../GameEngine.h"
#include "../GameState.h"
#include <glm/common.hpp>

ModelLoadTestScene::ModelLoadTestScene(){

	GameEngine& game = GameEngine::Instance();

	game.RemoveAllEntity();
	game.ClearLevel();

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

	game.LoadMeshFromFile("Res/Model/Landscape.fbx");
	game.LoadTextureFromFile("Res/Model/BG02.Diffuse.dds");
	game.LoadTextureFromFile("Res/Model/BG02.Normal.bmp");

	//背景の更新処理
	for (int z = 0; z < 5; ++z) {
		const float offsetZ = static_cast<float>(z * 40 * 5);

		for (int x = 0; x < 5; ++x) {
			const float offsetX = static_cast<float>(x * 40 - 80) * 5.0f;
			game.AddEntity(GameState::EntityGroupId_Background, glm::vec3(offsetX, -300.0, offsetZ),
				"Landscape01", "Res/Model/BG02.Diffuse.dds", "Res/Model/BG02.Normal.bmp", nullptr);
		}
	}
	game.Camera(0, { glm::vec4(0,30,-15,1),glm::vec3(0,0,0),glm::vec3(0,1,0) });

	game.LoadMeshFromFile("Res/Model/sampleField.fbx");
	
	auto p = game.AddEntity(GameState::EntityGroupId_Player, glm::vec3(0,10,0), "Cube", "Res/Model/BG02.Diffuse.dds",nullptr);

}

void ModelLoadTestScene::operator()(double delta) {

	GameEngine& game = GameEngine::Instance();
	const GamePad& gamepad = game.GetGamePad();

	if (gamepad.mouseWheelY) {
		std::cout << gamepad.mouseWheelY << std::endl;
	}

	if (gamepad.mouseButtons & GamePad::MOUSE_RIGHT_BUTTON) {

		GameEngine::CameraData camera = game.Camera(0);

		glm::vec3 vec = glm::vec3(0);
		glm::vec3 rot = glm::vec3(0);

		if (gamepad.buttons & GamePad::DPAD_RIGHT) {
			vec.x += 1;
		}
		else if (gamepad.buttons & GamePad::DPAD_LEFT) {
			vec.x -= 1;
		}
		if (gamepad.buttons & GamePad::DPAD_UP) {
			vec.z -= 1;
		}
		else if (gamepad.buttons & GamePad::DPAD_DOWN) {
			vec.z += 1;
		}
		vec *= -1;

		glm::ivec2 mouseVel = gamepad.mouseVelocity;
		float mouseSensitivy = 2.0f;
		rot.y = mouseVel.x * mouseSensitivy * delta;
		rot.x = mouseVel.y * mouseSensitivy * delta * -1.0f;
		rot *= rotateSpeed;

		//カメラのZ軸(ワールド空間)
		glm::vec3 cameraForward = glm::normalize(camera.target - camera.position);
		//カメラのX軸(ワールド空間)
		glm::vec3 cameraRight = glm::normalize(glm::cross(camera.up, cameraForward));
		//カメラのY軸(ワールド空間)
		glm::vec3 cameraUp = glm::normalize(glm::cross(cameraForward, cameraRight));


		if (vec.x != 0 || vec.z != 0) {
			//カメラの移動

			glm::vec3 vel = glm::normalize(cameraForward * vec.z + cameraRight * vec.x) * moveSpeed * static_cast<float>(delta);

			camera.target += vel;
			camera.position += vel;
		}

		if (mouseVel.x != 0 || mouseVel.y != 0) {
			//カメラの回転

			//カメラのY軸回転
			glm::quat rotateY = glm::angleAxis(glm::radians(rot.y), cameraUp);
			//カメラのX軸回転
			glm::quat rotateX = glm::angleAxis(glm::radians(rot.x), cameraRight);
			//回転の合成
			glm::quat rotateXY = rotateY * rotateX;

			glm::vec3 newDir = cameraForward * rotateXY;

			camera.target = camera.position + newDir;

		}

		game.Camera(0,camera);
	}


}
