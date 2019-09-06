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
*	TODO: �R�R�ɏ����ׂ������ł͂Ȃ����ߌ�Ɉڍs�\��
*/
class SceneComponent {
public:

	SceneComponent() = default;
	SceneComponent(const SceneComponent&) = delete;
	virtual ~SceneComponent() = default;
	const SceneComponent& operator=(const SceneComponent&) = delete;

private:
};

/**
*	�J�����̃R���|�[�l���g�N���X
*/
class CameraComponent :public SceneComponent{
public:

	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = delete;
	virtual ~CameraComponent() = default;
	const CameraComponent& operator=(const CameraComponent&) = delete;
	
	/**
	*	�r���[�ϊ��s��̎擾
	*
	*	@return �r���[�ϊ��s��
	*/
	virtual const glm::mat4& ViewMatrix() const;

	/**
	*	�v���W�F�N�V�����ϊ��s��̎擾
	*
	*	@return �v���W�F�N�V�����ϊ��s��
	*/
	virtual const glm::mat4& ProjctionMatrix() const;

	/**
	*	����������
	*/
	virtual void Initialize() {};

	/**
	*	�X�V����
	*
	*	@param deltaTime	�o�ߎ���
	*/
	virtual void Update(float delta);

	/**
	*	�J�����̈ʒu���̐ݒ�
	*
	*	@return p	�ݒ肷��J�����̍��W
	*/
	void Position(glm::vec3& p) { position = p; }		

	/**
	*	�J�����̌��Ă�������̐ݒ�
	*
	*	@param d	�J�����̕����x�N�g��
	*/
	void Direction(glm::vec3& d) { direction = d; }

	/**
	*	�J�����̏�����x�N�g���̐ݒ�
	*
	*	@param u	�J�����̏�����x�N�g��
	*/
	void UpVector(glm::vec3& u) { up = u; }

	/**
	*	�J�����̉�p�̐ݒ�
	*
	*	@param f	��p
	*/
	void Fov(float f) { fov = f; }

	/**
	*	�A�X�y�N�g��̐ݒ�
	*
	*	@param a	�A�X�y�N�g��
	*/
	void Aspect(float a) { aspect = a; }

	/**
	*	�J������Near������ݒ肷��
	*
	*	@param n	Near�l
	*/
	void Near(float n) { near = n; }

	/**
	*	�J������Far������ݒ肷��
	*
	*	@param f	Far�l
	*/
	void Far(float f) { far = f; }

	/**
	*	�J�����̃r���[�p�����[�^�̈ꊇ�ݒ�
	*
	*	@param p	���W
	*	@param d	�����x�N�g��
	*	@param u	������x�N�g��
	*/
	void ViewMatrixParam(glm::vec3& p, glm::vec3& d, glm::vec3 u) { position = p; direction = d; up = u; }

	/**
	*	�J�����̃v���W�F�N�V�����p�����[�^�̈ꊇ�ݒ�
	*
	*	@param fo	��p
	*	@param a	�A�X�y�N�g��
	*	@param n	Near�l
	*	@param fa	Far�l
	*/
	void ProjMatrixParam(float fo, float a, float n, float fa) { fov = fo; aspect = a; near = n; far = fa; }

	/**
	*	�J�����̈ʒu�Ƃ݂Ă��������������x�N�g����ݒ�
	*	
	*	@param pos	�J������������W
	*	@param at	�J�����̒����_
	*/
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
	~CameraDebugComponent() = default;
	const CameraDebugComponent& operator=(const CameraDebugComponent&) = delete;

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

private:

	glm::vec3 forward;	/// �O�����x�N�g��
	glm::vec3 right;	/// �E�����x�N�g��
	glm::vec3 upLocal;	/// ������x�N�g��(���[�J��)

};




