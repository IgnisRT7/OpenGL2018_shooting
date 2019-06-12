/**
*	@file EnemyLaunchController.cpp
*/

#include "EnemyLaunchController.h"
#include <algorithm>
#include "Entity/Enemy.h"
#include "../GameEngine.h"
#include "../GameState.h"

using namespace GameState;

//TODO: 最終的にファイルからの入力で行う
//ステージ1の敵リスト
EnemyLaunchType stage1EnemyLaunchList[] = {

	/// count,interval,type,Mtype,Btype,time,position
	{ 5,	1,	1,	1,	-1,	5,	{screenHalfW * 0.7,0,screenHalfH}},
	{ 5,	1,	1,	1,	-1,	15, {-screenHalfW * 0.7,0,screenHalfH}},
	{ 5,	1,	1,	2,	1,	25, {screenHalfW,0,screenHalfH*0.8}},
	{ 5,	1,	1,	2,	1,	35, {-screenHalfW,0,screenHalfH*0.8}},
	{ 1,	1,	1,	1,	5 ,	45, {screenHalfW * 0.7,0,screenHalfH}},
	{ 1,	1,	1,	1,	5 ,	45, {-screenHalfW * 0.7,0,screenHalfH}},
	{ 3,	1,	1,	3,	-1,	55, {screenHalfW,0,screenHalfH*0.8}},
	{ 3,	1,	1,	3,	-1,	63, {-screenHalfW,0,screenHalfH*0.8}},
};

//ステージ2の敵リスト
EnemyLaunchType stage2EnemyLaunchList[] = {
/// count,interval,type,Mtype,Btype,time,position
	{ 5,	1,	1,	6,	1,	5,	{screenHalfW,0,screenHalfH}},
	{ 5,	1,	1,	6,	1,	15, {-screenHalfW,0,screenHalfH}},
	{ 2,	3,	1,	5,	5,	23, {screenHalfW * 0.7,0,screenHalfH}},
	{ 2,	3,	1,	5,	5,	23, {-screenHalfW * 0.7,0,screenHalfH}},
	{ 3,	1,	1,	4,	1 ,	30, {screenHalfW,0,screenHalfH*0.7f}},
	{ 3,	1,	1,	4,	1 ,	30, {-screenHalfW,0,screenHalfH*0.7f}},
//	{ 3,	1,	1,	3,	-1,	55, {screenHalfW,0,screenHalfH*0.8}},
//	{ 3,	1,	1,	3,	-1,	63, {-screenHalfW,0,screenHalfH*0.8}},
};


MoveControllerPtr MakeMoveControllerByMoveType(int type, bool inverse) {

	auto seq = std::make_shared<MovePartSequencer>();

	switch (type) {
	case 1:		//up→down to left from center
		seq->Add(std::make_shared<MoveStraight>(2.f, glm::vec3(0, 0, -20.f)));
		seq->Add(std::make_shared<MoveStraight>(10.f, glm::vec3(inverse? 100 : -100, 0, 0)));
		break;
	case 2:		// left→right  * 
	case 3:		// left→right (stair)
		seq->Add(std::make_shared<MoveStraight>(6.f,glm::vec3(screenSizeW * 1.2f * (inverse ? 1 : -1), 0, 0)));
		break;
	case 4:
		seq->Add(std::make_shared<MoveStraight>(4.f,glm::vec3(screenHalfW * (inverse ? 1 : -1),0,0)));
		seq->Add(std::make_shared<MoveStraight>(2.f,glm::vec3(0, 0, 0)));
		seq->Add(std::make_shared<MoveStraight>(4.f,glm::vec3(screenHalfW * (inverse ? -1.2 : 1.2), 0, 0)));
		break;
	case 5:
		seq->Add(std::make_shared<MoveStraight>(4.f, glm::vec3(0, 0, -screenHalfH* 0.3f)));
		seq->Add(std::make_shared<MoveStraight>(2.f, glm::vec3(0, 0, 0)));
		seq->Add(std::make_shared<MoveStraight>(4.f, glm::vec3(0, 0, -screenHalfH *2)));
		break;
	case 6:		//circle
		seq->Add(std::make_shared<MoveCircle>(120, 60 * (inverse ? -1 : 1), glm::pi<float>() * 0.5 * (inverse ? -1 : 1) , 20));
		
	default:
		break;
	}


	return  std::make_shared<MoveController>(seq);
}

/**
*	初期化処理l
*
*	@param stageNum	ロードするステージの番号
*/
void EnemyLaunchController::Init(int stageNum){
	
	auto LaunchEnemyList = {
		stage1EnemyLaunchList,
		stage2EnemyLaunchList};

	switch (stageNum){
	case 1:
		for (auto& launchData : stage1EnemyLaunchList) {

			launchList.push_back(launchData);
		}
		break;
	case 2:
		for (auto& launchData : stage2EnemyLaunchList) {

			launchList.push_back(launchData);
		}
		break;
	case 3:
		for (auto& launchData : stage1EnemyLaunchList) {

			launchList.push_back(launchData);
		}
		break;
	default:
		break;
	}

	std::sort(launchList.begin(), launchList.end());
	
	//launchList.push_back(stage1EnemyLaunchList[0]);
}

/**
*	更新処理
*
*	@param deltaTime	経過時間
*/
void EnemyLaunchController::Update(float deltaTime){

	if (isFinished) {
		return;
	}

	timer += deltaTime;

	//現在の参照位置の出撃データを取得する
	auto& itr = (launchList.begin() + seekIndex);

	while (true) {

		if (seekIndex >= launchList.size()) {
			isFinished = true;
			break;
		}

		if (itr->launchStartTimer < timer) {
			//出撃可能な状態になった


			auto e = std::make_shared<GameState::EnemySpawner>(
				itr->launchCount,itr->launchInterval,itr->type,itr->moveType,itr->bulletType);


			GameEngine::Instance().AddEntity(
				GameState::EntityGroupId_Others, itr->startPostion, nullptr, nullptr, e);
			std::cout << "EnemySpawner is Launched index: " << seekIndex << " time: "<<timer << std::endl;


			//次の出撃データを確認する
			itr++;
			seekIndex++;
		}
		else {
			break;
		}
	}
}

void EnemyLaunchController::Launch(){



//	GameEngine::Instance().AddEntity(
//		GameState::EntityGroupId_Others,activeList[0].startPostion,nullptr,nullptr,
//	)
}
