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
		~MainGame() = default;
		MainGame(const MainGame&) = delete;
		const MainGame& operator=(const MainGame&) = delete;

		/**
		*	初期化処理
		*
		*	@return true	初期化成功
		*	@return false	初期化失敗
		*/
		bool Initialize() override;

		/**
		*	更新処理
		*
		*	@param deltaTime	経過時間
		*/
		void Update(float deltaTime) override;

		/**
		*	終了処理
		*/
		void Finalize() override;

		/**
		*	開始処理
		*/
		void Play() override;

		/**
		*	停止処理
		*/
		void Stop() override ;

		/**
		*	非表示処理
		*/
		void Hide() override {};

		/**
		*	プレイヤーのタイプを設定する
		*
		*	@param t	タイプ
		*/	
		void SelectPlayerType(int t) { playerType = t; }

		/**
		*	スクリーンの描画情報を設定する
		*
		*	@param deltaTime	経過時間
		*/
		void DrawScreenInfo(float deltaTime);

		/**
		*	ステージのロード処理
		*/
		void StageLoad();

		/**
		*	ステージクリア後の処理
		*
		*	@param nextStageTimer	次のステージの開始タイマ
		*/
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