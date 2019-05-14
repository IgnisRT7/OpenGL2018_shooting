/**
*	@file CameraComponent.cpp
*/

#include "src\Component\CameraComponent.h"
#include "src/GameEngine.h"

glm::mat4& CameraComponent::ViewMatrix() const{
	return glm::lookAt(position, position + direction, up);
}

glm::mat4& CameraComponent::ProjctionMatrix() const {
	return glm::perspective(fov, aspect, near, far);
}

/**
*	初期化処理
*/
void CameraDebugComponent::Initialize(){

	forward = direction;
	right = glm::cross(direction, upLocal);
	up = glm::cross(forward, right);
}

/**
*	更新処理
*
*	@param delta	経過時間
*/
void CameraDebugComponent::Update(float delta){

	GameEngine& game = GameEngine::Instance();
	GamePad gamepad = game.GetGamePad();
	GameEngine::CameraData camera = game.Camera(0);

	/// 移動量の計算
	glm::vec3 mov = glm::vec3(0);
	if (gamepad.buttons == GamePad::DPAD_LEFT) {
		mov.x -= 1;
	}
	else if (gamepad.buttons == GamePad::DPAD_RIGHT) {
		mov.x += 1;
	}
	if (gamepad.buttons == GamePad::DPAD_UP) {
		mov.y += 1;
	}
	else if (gamepad.buttons == GamePad::DPAD_DOWN) {
		mov.y -= 1;
	}
	mov = glm::normalize(mov);

	glm::vec3 rot;	/// カメラの軸に対する回転量
	glm::vec2 mouseVel = gamepad.mouseVelocity;
	rot = glm::vec3(mouseVel.y, mouseVel.x, 0);

	/// マウス感度
	float mouseSensitivy = 1.0f;
	
	if (mov.length() >= 0.05f || rot.length() >= 0.05f) {
		//更新処理を行う

		/// 回転軸の更新処理
		forward = direction;
		right = glm::normalize(glm::cross(direction, up));
		upLocal = glm::normalize(glm::cross(forward, right));

		///カメラの回転処理		
		glm::quat rotateAxisY = glm::angleAxis(glm::radians(rot.y), upLocal);
		glm::quat rotateAxisX = glm::angleAxis(glm::radians(rot.x), right);

		glm::quat rotateAxisXY = rotateAxisX * rotateAxisY;

		forward = rotateAxisXY * forward;

		///カメラの移動処理
		glm::vec3 vel = forward * mov.z + right * mov.x;
		this->position += vel;

		camera.position = this->position;
		camera.target = this->position + forward;

	}




}
