/**
*	@file EnemyMoveController.cpp
*/
#include "EnemyMoveController.h"

MovePart::MovePart(float d) {
	duration = d;
}

MoveController::MoveController(const MovePartPtr& p):
part(p),elapsedTime(0.f){
}

void MoveController::Update(Entity::Entity& entity, float delta) {

	if (!part) {
		//–¢Ý’è‚È‚Ì‚Åˆ—‚µ‚È‚¢
		return;
	}

	
	if(!isInitialized){
		isInitialized=true;
		part->Initialize(entity);
	}
	
	float durationTime = part->Duration();
	elapsedTime += delta;
	if (elapsedTime < durationTime) {
		//“®ìŠúŠÔ“à

		part->Update(entity,elapsedTime);
	}
	else {
		//“®ìŠúŠÔŠO

		part->Update(entity, durationTime);

		if (isLoop) {

			part->Initialize(entity);
			elapsedTime -= durationTime;
			part->Update(entity, elapsedTime);
		}

	}
}

MoveStraight::MoveStraight(float d, const glm::vec3& v) :
	moveVel(v),MovePart(d){
}

void MoveStraight::Initialize(Entity::Entity& entity) {

	MovePart::Initialize(entity);
	start = entity.Position();
}

void MoveStraight::Update(Entity::Entity& entity, float elapsedTime) {

	const float ratio = glm::clamp(elapsedTime / duration, 0.f, 1.f);
	glm::vec3 newPos = start + moveVel * ratio;
	entity.Position(newPos);
}

MoveCircle::MoveCircle(float d, float a,float o, float r):
angle(a),offsetAngle(o),range(r),MovePart(d){

	a *= o / glm::abs(o);

}

void MoveCircle::Initialize(Entity::Entity& entity){

	MovePart::Initialize(entity);
	center = entity.Position(); 
}

void MoveCircle::Update(Entity::Entity& entity, float elapsedTime){

	const float ratio = glm::clamp(elapsedTime / duration, 0.f, 1.f);

	const glm::vec3 forward = glm::vec3(0, 0, 1);
	glm::quat rot = glm::angleAxis(offsetAngle + angle * ratio, glm::vec3(0, -1, 0));
	entity.Position(center + rot * (forward*range));
}

void MovePartSequencer::Initialize(Entity::Entity& entity) {

	MovePart::Initialize(entity);
	execIndex = -1;
	currentTimeBegin = currentTimeEnd = 0;
	NextMove(entity);
}

void MovePartSequencer::Update(Entity::Entity& entity, float elapsedTime){

	if (execIndex < 0 || static_cast<size_t>(execIndex) >= movList.size()) {
		//ŽÀs‘ÎÛ‚ª‘¶Ý‚µ‚È‚¢
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

void MovePartSequencer::Add(const MovePartPtr p){

	movList.push_back(p);
	duration += p->Duration();
}

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


