/**
*	@file GameEndScene.h
*	@brief	ゲームオーバー時のシーン制御用
*	@author	Takuya Yokoyama
*/
#pragma once

#include "TitleScene.h"
#include "../Engine/Scene.h"

namespace GameState{

	///タイトル画面
	class GameEnd : public Scene{
	public:

		GameEnd(bool clear = false) :isClear(clear),Scene("GameEnd") {}

		bool Initialize() override;
		void Update(float delta) override;
		void Finalize() override;

		void Play() override;
		void Stop() override;
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