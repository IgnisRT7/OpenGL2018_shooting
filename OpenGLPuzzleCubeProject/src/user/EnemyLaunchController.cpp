/**
*	@file EnemyLaunchController.cpp
*/

#include "EnemyLaunchController.h"
#include <algorithm>
#include "Entity/Enemy.h"
#include "../GameEngine.h"
#include "../GameState.h"

EnemyLaunchType stage1EnemyLaunchList[] = {
	/// count,interval,type,time,position
	{ 5,	1,	1,	2,	{ -10, 0, 40 } },
	{ 5,	1,	1,	2,	{ 10 , 0, 40 } },
	{ 3,	1,	2,	5,	{ -10, 0, 40 } },
	{ 3,	1,	2,	5,	{ 10 , 0, 40 } },
	{ 5,	1,	1,	7,	{ -10, 0, 40 } },
	{ 5,	1,	1,	7,	{ 10 , 0, 40 } },
	{ 1,	1,	1,	10,	{ 0	 , 0, 40}},

};

int stage1EnemyBulletList[]{
	-1,-1,1,1,-1,-1,5,
};

/**
*	初期化処理
*
*	@param stageNum	ロードするステージの番号
*/
void EnemyLaunchController::Init(int stageNum){

	for (auto& launch : stage1EnemyLaunchList) {

		launchList.push_back(launch);
	}
	std::sort(launchList.begin(), launchList.end());
}

/**
*	更新処理
*
*	@param deltaTime	経過時間
*/
void EnemyLaunchController::Update(float deltaTime){

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
				itr->launchCount,itr->launchInterval,itr->enemyType,stage1EnemyBulletList[seekIndex]);


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
