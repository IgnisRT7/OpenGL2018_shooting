/**
*	@file EnemyMoveController.h
*/

#pragma once
#include "../Entity.h"

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
	explicit MoveController(const MovePartPtr& p);
	MoveController(const MoveController&) = delete;
	MoveController& operator=(const MoveController&) = delete;

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
	MoveStraight(float d,const glm::vec3& v);

	MoveStraight(const MoveStraight&) = delete;
	MoveStraight& operator=(const MoveStraight&) = delete;
	virtual ~MoveStraight() = default;

	virtual void Initialize(Entity::Entity& p) override;
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
	MoveCircle(float d,float a,float o, float r);
	MoveCircle(const MoveCircle&) = delete;
	MoveCircle& operator=(const MoveCircle&) = delete;
	virtual ~MoveCircle() = default;

	virtual void Initialize(Entity::Entity& p)override;
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

	virtual void Initialize(Entity::Entity& p) override;
	virtual void Update(Entity::Entity& p, float d) override;
	void Add(const MovePartPtr p);

private:

	bool NextMove(Entity::Entity&);

	std::vector<MovePartPtr> movList;	///< ����p�[�c�̃��X�g
	int execIndex = -1;					///< ���ݎ��s���̓���p�[�c
	float currentTimeBegin;				///< ���s���̓���p�[�c�̊J�n����
	float currentTimeEnd;				///< ���s���̓���p�[�c�̏I������
};
