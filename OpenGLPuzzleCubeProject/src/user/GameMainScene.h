/**
*	@file GameMainScene.h
*/
#pragma once
#include "../Engine/Entity.h"
#include "Entity/Player.h"
#include "../Engine/Scene.h"
#include "EnemyLaunchController.h"
#include "../Engine/Sprite.h"

namespace GameState {


	///背景(スライド)
	class Landscape : public Entity::EntityDataBase {
	public:

		//Landscape() = default;
		Landscape(bool l = false);

		void Initialize() override;
		void Update(float delta) override;
	private:

		float timer = 0;
		glm::vec3 start;
		bool isLoop = false;
	};

	///背景の更新(回転)
	class SpaceSphereMain : public Entity::EntityDataBase {

		void Initialize() override;

		void Update(float delta) override;
	};
	


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
		void DrawScreenInfo();
		void StageLoad();
		void StageClear(float nextStageTimer) { stageTimer = nextStageTimer; }

	private:

		float stageNameFadeTimer = 0;	///< ステージ名を表示するためのタイマー
		float stageTimer = -1;			///< ステージが遷移するまでのタイマー
		float sceneTimer=0;				///< シーンが遷移するまでのタイマー
		
		bool isStageClear = false;		///< ステージをクリアしたかどうかのフラグ

		int stageNo = 0;				///< 現在のステージ番号
		int playerType = 0;				///< プレイヤーのタイプ

		struct FontDrawInfo {
			glm::vec2 pos;
			glm::vec2 size;
			glm::vec4 color;
			std::string str;
		}stageName;


		std::shared_ptr<Player> playerData;	///< プレイヤーのエンティティデータ
		std::shared_ptr<Sprite> hpguage;	///< デバッグ用HPゲージ用スプライト

		EnemyLaunchControllerPtr launchController;	
	};

}