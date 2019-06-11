/**
*	@file GameEndScene.h
*/
#pragma once

#include "TitleScene.h"
#include "../Scene.h"

namespace GameState{

	///�^�C�g�����
	class GameEnd : public Scene{
	public:

		GameEnd(bool clear = false) :isClear(clear),Scene("GameEnd") {}

		bool Initialize() override;
		void Update(float delta) override;
		void Finalize() override;

		void Play() override;
		void Stop() override;
		void Hide() override {}
		
//		void operator()(float delta);

	private:

		float timer = 0;
		bool isClear;

		TitleSpaceSphere spaceSphere;
	};
}