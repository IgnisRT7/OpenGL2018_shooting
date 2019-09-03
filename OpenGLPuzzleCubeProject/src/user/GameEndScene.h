/**
*	@file GameEndScene.h
*	@brief	ゲームオーバー時のシーン制御用
*	@author	Takuya Yokoyama
*/
#pragma once

#include "TitleScene.h"
#include "../Engine/Scene.h"

namespace Application{

	///タイトル画面
	class GameEnd : public Scene{
	public:

		GameEnd(bool clear = false) :isClear(clear),Scene("GameEnd") {}

		/**
		*	初期化処理
		*
		*	@return true	初期化成功
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
		void Stop() override;

		/**
		*	非表示処理
		*/
		void Hide() override {}
		
	private:

		struct FontDrawInfo {
			glm::vec2 pos;
			glm::vec2 size;
			glm::vec4 color;
			std::string str;
			bool isCenter;
		};

		FontDrawInfo scoreStrInfo, highScoreStrInfo,gameoverStrInfo,pressButtonStrInfo;

		TitleSpaceSphere spaceSphere;

		float timer = 0;
		float strFadeTimer = 0;
		bool isClear;


	};
}