/**
*	@file EnemyLaunchController.cpp
*	@brief	�G�̏o���V�X�e���̐���p
*	@author	Takuya Yokoyama
*/

#include "EnemyLaunchController.h"
#include "Entity/Enemy.h"
#include "../Engine/GameEngine.h"
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

	case 1:		//up��down to left from center
		seq->Add(std::make_shared<MoveStraight>(2.f, glm::vec3(0, 0, -20.f)));
		seq->Add(std::make_shared<MoveStraight>(10.f, glm::vec3(inverse? 100 : -100, 0, 0)));
		break;
	case 2:		// left��right  * 
	case 3:		// left��right (stair)
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
*	����������l
*
*	@param stageNum	���[�h����X�e�[�W�̔ԍ�
*/
void EnemyLaunchController::Init(int stageNum){
	
	std::string filename = "Res/StageData/Stage";
	filename += std::to_string(stageNum);
	filename += ".csv";

	Load(filename);

	std::sort(launchList.begin(), launchList.end());

}

/**
*	�X�e�[�W�f�[�^�̓ǂݍ��ݏ���
*
*	@param filename �X�e�[�W�̃t�@�C����
*/
void EnemyLaunchController::Load(const std::string& filename) {

	launchList.clear();

	std::ifstream ifs(filename);
	std::string buf((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	int i = 0;

	std::istringstream bufstream(buf);
	std::string lineStr;

	//�X�e�[�W�f�[�^���o��
	while (std::getline(bufstream, lineStr, '\n')) {
		//1�s��(1�o����)�̃f�[�^���o��

		std::vector<float> buftmp;
		std::istringstream lineStream(lineStr);
		std::string valueStr;
		while (std::getline(lineStream, valueStr, ',')) {
			//���l�f�[�^�����Ă���

			buftmp.push_back(static_cast<float>(std::atof(valueStr.c_str())));
		}

		EnemyLaunchType launchData;
		launchData.launchStartTimer = buftmp[0];
		launchData.launchCount = static_cast<int>(buftmp[1]);
		launchData.launchInterval = buftmp[2];
		launchData.enemyType = static_cast<int>(buftmp[3]);
		launchData.moveType = static_cast<int>(buftmp[4]);
		launchData.bulletType = static_cast<int>(buftmp[5]);
		launchData.health = static_cast<int>(buftmp[6]);
		
		launchData.startPostion = glm::vec3(buftmp[7], buftmp[8], buftmp[9]);

		launchList.push_back(launchData);
	}

	auto lastLaunchData = launchList[launchList.size() - 1];
	lastSpawnTime = lastLaunchData.launchCount * lastLaunchData.launchInterval + 10.0f;
}

/**
*	�X�V����
*
*	@param deltaTime	�o�ߎ���
*/
void EnemyLaunchController::Update(float deltaTime){

	if (isFinished) {
		return;
	}

	timer += deltaTime;

	//���݂̎Q�ƈʒu�̏o���f�[�^���擾����
	auto itr = (launchList.begin() + seekIndex);

	while (true) {

		if (static_cast<size_t>(seekIndex) >= launchList.size()) {
			isFinished = true;
			break;
		}

		if (itr->launchStartTimer < timer) {
			//�o���\�ȏ�ԂɂȂ���

			auto e = std::make_shared<GameState::EnemySpawner>(
				itr->launchCount,itr->launchInterval,itr->enemyType,itr->moveType,itr->bulletType,itr->health);

			GameEngine::Instance().AddEntity(
				GameState::EntityGroupId_Others, itr->startPostion, nullptr, nullptr, e);

			
			//���̏o���f�[�^��
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
