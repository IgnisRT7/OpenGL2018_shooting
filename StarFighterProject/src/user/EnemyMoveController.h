/**
*	@file EnemyMoveController
*	@brief	�G�̈ړ��V�X�e���̐���p
*	@author	Takuya Yokoyama , tn-mai(�u�`�����̐����)
*/

#pragma once
#include "../Engine/Entity.h"

/**
*	�ړ�����̃p�[�c�N���X
*	tips:	�G���e�B�e�B�̓���Ƃ��̓���ɂ����鎞�ԂȂǂ��Ǘ�����
*/
class MovePart {
public:

	MovePart() = default;
	MovePart(float d);
	MovePart& operator=(const MovePart&) = delete;
	virtual ~MovePart() = default;

	float Duration() const { return duration; }
	void Duration(float t) { duration = t; }

	virtual void Initialize(Entity::Entity& e) { }
	virtual void Update(Entity::Entity& e,float d) = 0;



protected:
	
	float duration;		///< �o�ߎ���
};

using MovePartPtr = std::shared_ptr<MovePart>;

/**
*	�ړ�����N���X
*/
class MoveController {
public:

	MoveController() = default;

	/**
	*	�R���X�g���N�^
	*
	*	@param p	���삷�铮��p�[�c
	*/
	explicit MoveController(const MovePartPtr& p);
	MoveController(const MoveController&) = delete;
	MoveController& operator=(const MoveController&) = delete;

	/**
	*	�X�V����
	*
	*	@param entity	���䂷��G���e�B�e�B
	*	@param delta	�o�ߎ���
	*/
	void Update(Entity::Entity& entity, float delta);

private:

	MovePartPtr part;	///< ���䂷�铮��N���X
	float elapsedTime;	///< �o�ߎ���

	bool isInitialized = false;	///< �������t���O
	bool isLoop = false;		///< ���[�v����t���O
};

using MoveControllerPtr = std::shared_ptr<MoveController>;

/**
*	�܂������ɐi�ޓ���N���X
*/
class MoveStraight : public MovePart {
public:

	MoveStraight() = default;

	/**
	*	�R���X�g���N�^
	*
	*	@param d	���쎞��
	*	@param v	�ړ�����
	*/
	MoveStraight(float d,const glm::vec3& v);

	MoveStraight(const MoveStraight&) = delete;
	MoveStraight& operator=(const MoveStraight&) = delete;
	virtual ~MoveStraight() = default;

	/**
	*	����������
	*
	*	@param entity	���䂷��G���e�B�e�B
	*/
	virtual void Initialize(Entity::Entity& p) override;

	/**
	*	�X�V����
	*
	*	@param entity		���䂷��G���e�B�e�B
	*	@param elapsedTime	�o�ߎ���
	*/
	void Update(Entity::Entity& e,float d) override;

private:

	glm::vec3 start;	///< �X�^�[�g�n�_(�G���e�B�e�B�����ʒu)
	glm::vec3 moveVel;	///< �X�^�[�g����̈ړ���
	float ratio;		///< ���݂̐i�s��� 1�ɂȂ�Ɗ���
};

/**
*	�~��`���Ĉړ�����N���X
*/
class MoveCircle : public MovePart {
public:

	MoveCircle() = default;

	/**
	*	�R���X�g���N�^
	*
	*	@param d	���쎞��
	*	@param a	�ړ��p�x
	*	@param r	���a
	*/
	MoveCircle(float d,float a,float o, float r);
	MoveCircle(const MoveCircle&) = delete;
	MoveCircle& operator=(const MoveCircle&) = delete;
	virtual ~MoveCircle() = default;

	/**
	*	����������
	*
	*	@param entity	���䂷��G���e�B�e�B
	*/
	virtual void Initialize(Entity::Entity& p)override;

	/**
	*	�X�V����
	*
	*	@param entity	���䂷��G���e�B�e�B
	*	@param elapsedTime	�o�ߎ���
	*/
	void Update(Entity::Entity& e, float d)override;

private:

	float angle;
	float offsetAngle;
	float range;
	glm::vec3 center;
};

/**
*	MovePart���V�[�P���X���s����N���X
*/
class MovePartSequencer : public MovePart{
public:

	MovePartSequencer() = default;
	MovePartSequencer(const MovePartSequencer&) = delete;
	MovePartSequencer& operator=(const MovePartSequencer&) = delete;
	virtual ~MovePartSequencer() = default;

	/**
	*	����������
	*
	*	@param entity	����Ώۂ̃G���e�B�e�B
	*/
	virtual void Initialize(Entity::Entity& p) override;

	/**
	*	�X�V����
	*
	*	@param entity		����Ώۂ̃G���e�B�e�B
	*	@param elapsedTime	�o�ߎ���
	*/
	virtual void Update(Entity::Entity& p, float d) override;

	/**
	*	���샊�X�g�ɒǉ�
	*
	*	@param p	�ǉ����铮��p�[�c
	*/
	void Add(const MovePartPtr p);

private:

	/**
	*	���Ɏ��s�����铮��p�[�c�̐ݒ������
	*
	*	@param entity	����Ώۂ̃G���e�B�e�B
	*
	*	@retval true	���ɓ��삳����p�[�c�����݂��ݒ肵��
	*	@retval false	���ɓ��삳����p�[�c�����݂��Ȃ�(last index)
	*/
	bool NextMove(Entity::Entity&);

	std::vector<MovePartPtr> movList;	///< ����p�[�c�̃��X�g
	int execIndex = -1;					///< ���ݎ��s���̓���p�[�c
	float currentTimeBegin;				///< ���s���̓���p�[�c�̊J�n����
	float currentTimeEnd;				///< ���s���̓���p�[�c�̏I������
};
