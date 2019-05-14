/**
*	@file CameraComponent.h
*/
#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class SceneComponent;
using SceneComponentPtr = std::shared_ptr<SceneComponent>;

/**
*	�V�[���R���|�[�l���g
*	TODO: �R�R�ɏ����ׂ������ł͂Ȃ����ߌ�Ɉڍs����悤��
*/
class SceneComponent {
public:

private:
};

/**
*	�J�����̃R���|�[�l���g�N���X
*/
class CameraComponent :public SceneComponent{
public:

	virtual glm::mat4& ViewMatrix() const;
	virtual glm::mat4& ProjctionMatrix() const;

	virtual void Initialize() {};
	virtual void Update(float delta) {};

	void Position(glm::vec3& p) { position = p; }
	void Direction(glm::vec3& d) { direction = d; }
	void UpVector(glm::vec3& u) { up = u; }
	void Fov(float f) { fov = f; }
	void Aspect(float a) { aspect = a; }
	void Near(float n) { near = n; }
	void Far(float f) { far = f; }

protected:

	glm::vec3 position = glm::vec3(0, 0, 0);		/// �J�������W
	glm::vec3 direction = glm::vec3(0, 0, 1);		/// �J��������
	glm::vec3 up = glm::vec3(0, 1, 0);			/// �J�����p��

	float fov = 45.0f;		/// ��p
	float aspect = 1.0f;	/// �A�X�y�N�g��
	float near = 0.1f;		/// �ŋߓ_
	float far = 1000.0f;	/// �ŉ��_

};

class CameraDebugComponent : public CameraComponent {
public:

private:

	void Initialize() override;
	void Update(float delta) override;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 upLocal;

};



