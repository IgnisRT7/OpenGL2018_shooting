/**
*	@file EnemyLaunchController.cpp
*/

#include "EnemyLaunchController.h"
#include "Entity/Enemy.h"
#include "../GameEngine.h"
#include "../GameState.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace GameState;

MoveControllerPtr MakeMoveControllerByMoveType(int type, bool inverse) {

	auto seq = std::make_shared<MovePartSequencer>();

	switch (type) {
	case -1:
		seq->Add(std::make_shared<MoveStraight>(3,glm::vec3(0, 0, -30.f)));
		break;

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
	
	std::string filename = "Res/StageData/Stage";
	filename += std::to_string(stageNum);
	filename += ".txt";

	Load(filename);

	std::sort(launchList.begin(), launchList.end());

}

/**
*	ステージデータの読み込み処理
*/
void EnemyLaunchController::Load(const std::string& filename) {

	launchList.resize(0);

	std::ifstream ifs(filename);
	std::string buf((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	int i = 0;

	std::istringstream bufstream(buf);
	std::string lineStr;

	//ステージデータ取り出し
	while (std::getline(bufstream, lineStr, '\n')) {

		std::vector<float> buftmp;
		std::istringstream lineStream(lineStr);
		std::string valueStr;
		while (std::getline(lineStream, valueStr, ',')) {
			buftmp.push_back(std::atoi(valueStr.c_str()));
		}

		EnemyLaunchType launchData;
		launchData.launchCount = static_cast<int>(buftmp[0]);
		launchData.launchInterval = buftmp[1];
		launchData.type = static_cast<int>(buftmp[2]);
		launchData.moveType = static_cast<int>(buftmp[3]);
		launchData.bulletType = static_cast<int>(buftmp[4]);
		launchData.launchStartTimer = buftmp[5];
		launchData.startPostion = glm::vec3(buftmp[6], buftmp[7], buftmp[8]);

		launchList.push_back(launchData);
	}
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
	auto itr = (launchList.begin() + seekIndex);

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
