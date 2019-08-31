/**
*	@file CameraComponent.h
*	@brief	�V�[���Ŏg�p�����J�����R���|�[�l���g
*	@author	takuya Yokoyama
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

	virtual const glm::mat4& ViewMatrix() const;
	virtual const glm::mat4& ProjctionMatrix() const;

	virtual void Initialize() {};

	/**
	*	�X�V����
	*
	*	@param deltaTime	�o�ߎ���
	*/
	virtual void Update(float delta);

	void Position(glm::vec3& p) { position = p; }		
	void Direction(glm::vec3& d) { direction = d; }
	void UpVector(glm::vec3& u) { up = u; }
	void Fov(float f) { fov = f; }
	void Aspect(float a) { aspect = a; }
	void Near(float n) { near = n; }
	void Far(float f) { far = f; }

	void ViewMatrixParam(glm::vec3& p, glm::vec3& d, glm::vec3 u) { position = p; direction = d; up = u; }
	void ProjMatrixParam(float f, float a, float n, float fa) { far = f; aspect = a; near = n; far = fa; }
	void LookAt(glm::vec3 pos, glm::vec3 at) { position = pos; direction = glm::normalize(at - pos); }


protected:

	glm::vec3 position = glm::vec3(0, 0, 0);		/// �J�������W
	glm::vec3 direction = glm::vec3(0, 0, 1);		/// �J��������
	glm::vec3 up = glm::vec3(0, 1, 0);			/// �J�����p��

	float fov = 45.0f;		/// ��p
	float aspect = 1.0f;	/// �A�X�y�N�g��
	float near = 0.1f;		/// �ŋߓ_
	float far = 1000.0f;	/// �ŉ��_

	glm::mat4 matView;
	glm::mat4 matProj;
};

/**
*	�f�o�b�O�p�@�J��������N���X
*/
class CameraDebugComponent : public CameraComponent {
public:

	CameraDebugComponent() = default;
	CameraDebugComponent(const CameraDebugComponent&) = delete;
	void operator=(const CameraDebugComponent&) = delete;

private:

	/**
	*	����������
	*/
	void Initialize() override;

	/**
	*	�X�V����
	*
	*	@param deltaTime	�o�ߎ���
	*/
	void Update(float delta) override;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 upLocal;

};




