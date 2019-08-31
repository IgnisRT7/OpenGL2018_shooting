/**
*	@file	GameEndScene.cpp
*	@brief	ゲームオーバー時のシーン制御用
*	@author	Takuya Yokoyama
*/
#include "GameEndScene.h"
#include "../Engine/GameEngine.h"
#include "../GameState.h"
#include "TitleScene.h"

#include "../../Res/Audio/testProject_acf.h"
#include "../../Res/Audio/CueSheet_0.h"

#include <iostream>
#include <fstream>

namespace GameState{

	/**
	*	初期化処理
	*/
	bool GameState::GameEnd::Initialize(){

		GameEngine& game = GameEngine::Instance();

		game.LoadMeshFromFile("Res/Model/SpaceSphere.fbx");
		game.LoadTextureFromFile("Res/Model/SpaceSphere.dds");

		//ハイスコア読み込み処理
		int highScore = 100000;
		std::ifstream ifs("Res/HighScore.txt");
		if (!ifs.fail()) {
			char tmp[64];
			ifs.getline(tmp, 64);
			highScore = atoi(tmp);
		}
		
		//スコア読み込み処理
		int score = static_cast<int>(game.UserVariable("score"));

		if (score > highScore) {
			//ハイスコアを超えた

			highScore = score;
			std::ofstream ofs("Res/highScore.txt");
			if (!ofs.fail()) {
				ofs << score << std::endl;
			}
		}

		//画面描画用文字列の初期化処理

		scoreStrInfo.color = glm::vec4(1);
		scoreStrInfo.size = glm::vec2(4.5f);
		scoreStrInfo.pos = glm::vec2(0, -0.05f);
		scoreStrInfo.str = std::string("SCORE: ") + std::to_string(score);
		scoreStrInfo.isCenter = true;

		highScoreStrInfo.color = glm::vec4(1);
		highScoreStrInfo.size = glm::vec2(4.5f);
		highScoreStrInfo.pos = glm::vec2(0, 0.1f);
		highScoreStrInfo.str = std::string("HIGH SCORE: ") + std::to_string(highScore);
		highScoreStrInfo.isCenter = true;

		gameoverStrInfo.color = glm::vec4(1, 0, 0, 1);
		gameoverStrInfo.size = glm::vec2(8);
		gameoverStrInfo.pos = glm::vec2(0, 0.5f);
		gameoverStrInfo.str = isClear ? "GAME CLEAR!!" : "GAME OVER...";
		gameoverStrInfo.isCenter = true;

		pressButtonStrInfo.color = glm::vec4(1, 0, 0, 1);
		pressButtonStrInfo.size = glm::vec2(2);
		pressButtonStrInfo.pos = glm::vec2(0,-0.5f);
		pressButtonStrInfo.str = "Pressed enter to title...";
		pressButtonStrInfo.isCenter = true;

		return true;
	}

	/**
	*	終了処理
	*/
	void GameEnd::Finalize() {

	}

	/**
	*	シーンの開始処理
	*/
	void GameEnd::Play() {

		GameEngine& game = GameEngine::Instance();

		auto e = game.AddEntity(EntityGroupId_Background, glm::vec3(0, 0, 0),
			"SpaceSphere", "Res/Model/SpaceSphere.dds", std::make_shared<TitleSpaceSphere>(), "NonLighting");
		game.KeyValue(0.1f);

		game.MainCamera(std::make_shared<CameraComponent>());
		game.MainCamera()->LookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

		game.PlayAudio(0, CRI_CUESHEET_0_GAMEOVER);
	}

	/**
	*	シーンの停止処理
	*/
	void GameEnd::Stop() {
	}


	/**
	*	更新処理
	*/
	void GameEnd::Update(float delta){

		GameEngine& game = GameEngine::Instance();

		strFadeTimer += delta * (timer > 0 ? 20 : 2);

		float fadeAlpha = (glm::cos(strFadeTimer) + 1) * 0.25f;// 0 <= fadeAlpha <= 0.5
		pressButtonStrInfo.color.a = 0.5f + fadeAlpha;

		FontDrawInfo infoArray[] = {
			scoreStrInfo,highScoreStrInfo,gameoverStrInfo,pressButtonStrInfo
		};

		for (auto& info : infoArray) {
			game.FontColor(info.color);
			game.FontScale(info.size);
			game.AddString(info.pos, info.str.c_str(),info.isCenter);
		}

		auto gamepad = game.GetGamePad();

		if (timer > 0) {
			timer -= delta;
			if (timer <= 0) {

				game.PopScene();
				return;
			}
		}
		else if (game.GetGamePad().buttonDown & GamePad::START) {
			game.PlayAudio(1, CRI_CUESHEET_0_SELECT2);
			timer = 2;
		}
	}



}