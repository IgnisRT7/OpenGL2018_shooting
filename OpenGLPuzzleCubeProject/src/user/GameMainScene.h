/**
*	@file	GameMainScene.h
*	@brief	ゲームのメインシーン制御用
*	@author	Takuya Yokoyama
*/
#pragma once
#include "../Engine/Entity.h"
#include "Entity/Player.h"
#include "../Engine/Scene.h"
#include "EnemyLaunchController.h"
#include "../Engine/Sprite.h"
#include "Entity/LandScape.h"

namespace Application {

	///メインゲーム画面
	class MainGame : public Scene{
	public:

		MainGame() :Scene("MainGame") {}

		bool Initialize() override;
		void Update(float d) override;
		void Finalize() override;
		void Play() override;
		void Stop() override ;
		void Hide() override {};

		void SelectPlayerType(int t) { playerType = t; }
		void DrawScreenInfo(float dt);
		void StageLoad();
		void StageClear(float nextStageTimer);

	private:

		float stageNameFadeTimer = 0;	///< ステージ名を表示するためのタイマー
		float stageTimer = -1;			///< ステージが遷移するまでのタイマー
		float sceneTimer=0;				///< シーンが遷移するまでのタイマー
		
		bool isStageClear = false;		///< ステージをクリアしたかどうかのフラグ

		int stageNo = 0;				///< 現在のステージ番号
		int playerType = 0;				///< プレイヤーのタイプ(シーンからの引き継ぎ用)

		struct FontDrawInfo {
			glm::vec2 pos;
			glm::vec2 size;
			glm::vec4 color;
			std::string str;

		};

		FontDrawInfo stageName;
		FontDrawInfo stageClearString = {
			{0,0},{4,4},{0,0.3,1.0,1.0},"STAGE CLEAR!!"
		};
		FontDrawInfo scoreString = {
			{-0.75,-0.85f}, {3,3}, {0.8f,0.8f,0,1.0f },""
		};
		
		

		std::shared_ptr<Player> playerData;	///< プレイヤーのエンティティデータ
		std::shared_ptr<Sprite> hpguage;	///< デバッグ用HPゲージ用スプライト

		Application::EnemyLaunchControllerPtr launchController;	
	};

}