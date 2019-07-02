/**
*	@file EnemyMoveController.cpp
*/
#include "EnemyMoveController.h"

MovePart::MovePart(float d) {
	duration = d;
}

/**
*	コンストラクタ
*
*	@param p	操作する動作パーツ
*/
MoveController::MoveController(const MovePartPtr& p):
part(p),elapsedTime(0.f){
}

/**
*	更新処理
*
*	@param entity	制御するエンティティ
*	@param delta	経過時間
*/
void MoveController::Update(Entity::Entity& entity, float delta) {

	if (!part) {
		//未設定なので処理しない
		return;
	}

	
	if(!isInitialized){
		isInitialized=true;
		part->Initialize(entity);
	}
	
	float durationTime = part->Duration();
	elapsedTime += delta;
	if (elapsedTime < durationTime) {
		//動作期間内

		part->Update(entity,elapsedTime);
	}
	else {
		//動作期間外

		part->Update(entity, durationTime);

		if (isLoop) {

			part->Initialize(entity);
			elapsedTime -= durationTime;
			part->Update(entity, elapsedTime);
		}

	}
}

/**
*	コンストラクタ
*
*	@param d	動作時間
*	@param v	移動距離
*/
MoveStraight::MoveStraight(float d, const glm::vec3& v) :
	moveVel(v),MovePart(d){
}

/**
*	初期化処理
*
*	@param entity	制御するエンティティ
*/
void MoveStraight::Initialize(Entity::Entity& entity) {

	MovePart::Initialize(entity);
	start = entity.Position();
}

/**
*	更新処理
*
*	@param entity		制御するエンティティ
*	@param elapsedTime	経過時間
*/
void MoveStraight::Update(Entity::Entity& entity, float elapsedTime) {

	const float ratio = glm::clamp(elapsedTime / duration, 0.f, 1.f);
	glm::vec3 newPos = start + moveVel * ratio;
	entity.Position(newPos);
}

/**
*	コンストラクタ
*
*	@param d	動作時間
*	@param a	移動角度
*	@param r	半径
*/
MoveCircle::MoveCircle(float d, float a,float o, float r):
angle(a),offsetAngle(o),range(r),MovePart(d){

	a *= o / glm::abs(o);

}

/**
*	初期化処理
*
*	@param entity	制御するエンティティ
*/
void MoveCircle::Initialize(Entity::Entity& entity){

	MovePart::Initialize(entity);
	center = entity.Position(); 
}

/**
*	更新処理
*
*	@param entity	制御するエンティティ
*	@param elapsedTime	経過時間
*/
void MoveCircle::Update(Entity::Entity& entity, float elapsedTime){

	const float ratio = glm::clamp(elapsedTime / duration, 0.f, 1.f);

	const glm::vec3 forward = glm::vec3(0, 0, 1);
	glm::quat rot = glm::angleAxis(offsetAngle + angle * ratio, glm::vec3(0, -1, 0));
	entity.Position(center + rot * (forward*range));
}

/**
*	初期化処理
*
*	@param entity	制御対象のエンティティ
*/
void MovePartSequencer::Initialize(Entity::Entity& entity) {

	MovePart::Initialize(entity);
	execIndex = -1;
	currentTimeBegin = currentTimeEnd = 0;
	NextMove(entity);
}

/**
*	更新処理
*
*	@param entity		制御対象のエンティティ
*	@param elapsedTime	経過時間
*/
void MovePartSequencer::Update(Entity::Entity& entity, float elapsedTime){

	if (execIndex < 0 || static_cast<size_t>(execIndex) >= movList.size()) {
		//実行対象が存在しない
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
*	動作リストに追加
*
*	@param p	追加する動作パーツ
*/
void MovePartSequencer::Add(const MovePartPtr p){

	movList.push_back(p);
	duration += p->Duration();
}

/**
*	次に実行させる動作パーツの設定をする
*
*	@param entity	制御対象のエンティティ
*
*	@retval true	次に動作させるパーツが存在し設定した
*	@retval false	次に動作させるパーツが存在しない(last index)
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


