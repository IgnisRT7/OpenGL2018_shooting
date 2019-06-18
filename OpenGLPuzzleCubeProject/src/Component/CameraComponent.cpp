/**
*	@file CameraComponent.cpp
*/

#include "CameraComponent.h"
#include "../Engine/GameEngine.h"


const glm::mat4& CameraComponent::ViewMatrix() const{
	return matView;
}

const glm::mat4& CameraComponent::ProjctionMatrix() const {
	return matProj;
}

/**
*	更新処理
*/
void CameraComponent::Update(float delta){

	matView = glm::lookAt(position, position + direction, up);
	matProj = glm::perspective(fov, aspect, near, far);
}

/**
*	初期化処理
*/
void CameraDebugComponent::Initialize(){

	forward = direction;
	right = glm::cross(direction, upLocal);
	up = glm::cross(forward, right);
	Update(0);
}

/**
*	更新処理
*
*	@param delta	経過時間
*/
void CameraDebugComponent::Update(float delta){

	GameEngine& game = GameEngine::Instance();
	GamePad gamepad = game.GetGamePad();

	if (gamepad.mouseButtons & GamePad::MOUSE_RIGHT_BUTTON) {

		/// 移動量の計算
		glm::vec3 mov = glm::vec3(0);
		if (gamepad.buttons == GamePad::DPAD_LEFT) {
			mov.x -= 1;
		}
		else if (gamepad.buttons == GamePad::DPAD_RIGHT) {
			mov.x += 1;
		}
		if (gamepad.buttons == GamePad::DPAD_UP) {
			mov.z += 1;
		}
		else if (gamepad.buttons == GamePad::DPAD_DOWN) {
			mov.z -= 1;
		}
		if (glm::length(mov) > 0.01f) {
			mov = glm::normalize(mov);
		}

		/// マウス感度
		const float mouseSensitivy = 3;
		const float moveSpeed = 3.0f;
		glm::vec3 rot;	/// カメラの軸に対する回転量
		glm::vec2 mouseVel = gamepad.mouseVelocity;
		rot = glm::vec3(mouseVel.y, mouseVel.x, 0);
		rot *= mouseSensitivy * delta;


		if (glm::length(mov) >= 0.05f || glm::length(rot) >= 0.05f) {
			//更新処理を行う

			/// 回転軸の更新処理
			forward = glm::normalize(direction);
			right = glm::normalize(glm::cross(direction, up));
			upLocal = glm::normalize(glm::cross(forward, right));

			///カメラの回転処理		
			glm::quat rotateAxisY = glm::angleAxis(glm::radians(rot.y), upLocal);
			glm::quat rotateAxisX = glm::angleAxis(glm::radians(rot.x), -right);

			glm::quat rotateAxisXY = rotateAxisX * rotateAxisY;

			forward = rotateAxisXY * forward;

			///カメラの移動処理
			glm::vec3 vel = forward * mov.z + right * mov.x;
			this->position += vel;
			
			position = this->position;
			direction = forward;
		}
		CameraComponent::Update(delta);
	}
}
