/**
*	@file TitleScene.h
*/
#pragma once
#include "../Entity.h"
#include "../user/Entity/Player.h"
#include "../Scene.h"
#include <memory>

namespace GameState {

	///背景の更新
	class TitleSpaceSphere : public Entity::EntityDataBase {

		void Initialize() override;

		void Update(float delta) override;
	};

	///タイトル画面
	class Title : public Scene{
	public:

		Title() : Scene("Title") {}

		bool Initialize() override;
		void Update(float delta) override;
		void Finalize() override;
		void Play() override;
		void Stop() override;
		void Hide() override {}


		//void operator()(float delta) ;

	private:

		bool initialize = false;
		float timer = 0;
		int selectAirCraftType = 0;

		std::shared_ptr<PlayerForProduction> player;
	};
}


