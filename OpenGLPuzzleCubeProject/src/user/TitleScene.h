/**
*	@file TitleScene.h
*/
#pragma once

namespace GameState {

	///背景の更新
	class TitleSpaceSphere : public Entity::EntityDataBase {

		void Initialize() override;

		void Update(double delta) override;
	};

	///タイトル画面
	class Title {
	public:

		Title();

		void Initialize();

		void operator()(double delta) ;

	private:

		bool initial = true;
		float timer = 0;

		TitleSpaceSphere spaceSphere;
	};
}


