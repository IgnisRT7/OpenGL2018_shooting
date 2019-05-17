/**
*	@file GameEndScene.h
*/
#pragma once

#include "TitleScene.h"

namespace GameState{

	///�^�C�g�����
	class GameEnd {
	public:

		GameEnd();

		void Initialize();

		void operator()(float delta);

	private:

		float timer = 0;

		TitleSpaceSphere spaceSphere;
	};
}