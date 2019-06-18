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
*	�X�V����
*/
void CameraComponent::Update(float delta){

	matView = glm::lookAt(position, position + direction, up);
	matProj = glm::perspective(fov, aspect, near, far);
}

/**
*	����������
*/
void CameraDebugComponent::Initialize(){

	forward = direction;
	right = glm::cross(direction, upLocal);
	up = glm::cross(forward, right);
	Update(0);
}

/**
*	�X�V����
*
*	@param delta	�o�ߎ���
*/
void CameraDebugComponent::Update(float delta){

	GameEngine& game = GameEngine::Instance();
	GamePad gamepad = game.GetGamePad();

	if (gamepad.mouseButtons & GamePad::MOUSE_RIGHT_BUTTON) {

		/// �ړ��ʂ̌v�Z
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

		/// �}�E�X���x
		const float mouseSensitivy = 3;
		const float moveSpeed = 3.0f;
		glm::vec3 rot;	/// �J�����̎��ɑ΂����]��
		glm::vec2 mouseVel = gamepad.mouseVelocity;
		rot = glm::vec3(mouseVel.y, mouseVel.x, 0);
		rot *= mouseSensitivy * delta;


		if (glm::length(mov) >= 0.05f || glm::length(rot) >= 0.05f) {
			//�X�V�������s��

			/// ��]���̍X�V����
			forward = glm::normalize(direction);
			right = glm::normalize(glm::cross(direction, up));
			upLocal = glm::normalize(glm::cross(forward, right));

			///�J�����̉�]����		
			glm::quat rotateAxisY = glm::angleAxis(glm::radians(rot.y), upLocal);
			glm::quat rotateAxisX = glm::angleAxis(glm::radians(rot.x), -right);

			glm::quat rotateAxisXY = rotateAxisX * rotateAxisY;

			forward = rotateAxisXY * forward;

			///�J�����̈ړ�����
			glm::vec3 vel = forward * mov.z + right * mov.x;
			this->position += vel;
			
			position = this->position;
			direction = forward;
		}
		CameraComponent::Update(delta);
	}
}
