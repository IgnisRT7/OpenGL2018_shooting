/**
*	@file EnemyMoveController.cpp
*/
#include "EnemyMoveController.h"

MovePart::MovePart(float d) {
	duration = d;
}

/**
*	�R���X�g���N�^
*
*	@param p	���삷�铮��p�[�c
*/
MoveController::MoveController(const MovePartPtr& p):
part(p),elapsedTime(0.f){
}

/**
*	�X�V����
*
*	@param entity	���䂷��G���e�B�e�B
*	@param delta	�o�ߎ���
*/
void MoveController::Update(Entity::Entity& entity, float delta) {

	if (!part) {
		//���ݒ�Ȃ̂ŏ������Ȃ�
		return;
	}

	
	if(!isInitialized){
		isInitialized=true;
		part->Initialize(entity);
	}
	
	float durationTime = part->Duration();
	elapsedTime += delta;
	if (elapsedTime < durationTime) {
		//������ԓ�

		part->Update(entity,elapsedTime);
	}
	else {
		//������ԊO

		part->Update(entity, durationTime);

		if (isLoop) {

			part->Initialize(entity);
			elapsedTime -= durationTime;
			part->Update(entity, elapsedTime);
		}

	}
}

/**
*	�R���X�g���N�^
*
*	@param d	���쎞��
*	@param v	�ړ�����
*/
MoveStraight::MoveStraight(float d, const glm::vec3& v) :
	moveVel(v),MovePart(d){
}

/**
*	����������
*
*	@param entity	���䂷��G���e�B�e�B
*/
void MoveStraight::Initialize(Entity::Entity& entity) {

	MovePart::Initialize(entity);
	start = entity.Position();
}

/**
*	�X�V����
*
*	@param entity		���䂷��G���e�B�e�B
*	@param elapsedTime	�o�ߎ���
*/
void MoveStraight::Update(Entity::Entity& entity, float elapsedTime) {

	const float ratio = glm::clamp(elapsedTime / duration, 0.f, 1.f);
	glm::vec3 newPos = start + moveVel * ratio;
	entity.Position(newPos);
}

/**
*	�R���X�g���N�^
*
*	@param d	���쎞��
*	@param a	�ړ��p�x
*	@param r	���a
*/
MoveCircle::MoveCircle(float d, float a,float o, float r):
angle(a),offsetAngle(o),range(r),MovePart(d){

	a *= o / glm::abs(o);

}

/**
*	����������
*
*	@param entity	���䂷��G���e�B�e�B
*/
void MoveCircle::Initialize(Entity::Entity& entity){

	MovePart::Initialize(entity);
	center = entity.Position(); 
}

/**
*	�X�V����
*
*	@param entity	���䂷��G���e�B�e�B
*	@param elapsedTime	�o�ߎ���
*/
void MoveCircle::Update(Entity::Entity& entity, float elapsedTime){

	const float ratio = glm::clamp(elapsedTime / duration, 0.f, 1.f);

	const glm::vec3 forward = glm::vec3(0, 0, 1);
	glm::quat rot = glm::angleAxis(offsetAngle + angle * ratio, glm::vec3(0, -1, 0));
	entity.Position(center + rot * (forward*range));
}

/**
*	����������
*
*	@param entity	����Ώۂ̃G���e�B�e�B
*/
void MovePartSequencer::Initialize(Entity::Entity& entity) {

	MovePart::Initialize(entity);
	execIndex = -1;
	currentTimeBegin = currentTimeEnd = 0;
	NextMove(entity);
}

/**
*	�X�V����
*
*	@param entity		����Ώۂ̃G���e�B�e�B
*	@param elapsedTime	�o�ߎ���
*/
void MovePartSequencer::Update(Entity::Entity& entity, float elapsedTime){

	if (execIndex < 0 || static_cast<size_t>(execIndex) >= movList.size()) {
		//���s�Ώۂ����݂��Ȃ�
		return;
	}

	while (elapsedTime >= currentTimeEnd) {
		//

		movList[execIndex]->Update(entity, movList[execIndex]->Duration());
		if (!NextMove(entity)) {
			return;
		}
	}

	movList[execIndex]->Update(entity, elapsedTime - currentTimeBegin);
}

/**
*	���샊�X�g�ɒǉ�
*
*	@param p	�ǉ����铮��p�[�c
*/
void MovePartSequencer::Add(const MovePartPtr p){

	movList.push_back(p);
	duration += p->Duration();
}

/**
*	���Ɏ��s�����铮��p�[�c�̐ݒ������
*
*	@param entity	����Ώۂ̃G���e�B�e�B
*
*	@retval true	���ɓ��삳����p�[�c�����݂��ݒ肵��
*	@retval false	���ɓ��삳����p�[�c�����݂��Ȃ�(last index)
*/
bool MovePartSequencer::NextMove(Entity::Entity& entity) {

	if (execIndex >= static_cast<int>(movList.size()) - 1) {
		return false;
	}

	execIndex++;

	currentTimeBegin = currentTimeEnd;
	currentTimeEnd += movList[execIndex]->Duration();
	movList[execIndex]->Initialize(entity);

	return true;
}


