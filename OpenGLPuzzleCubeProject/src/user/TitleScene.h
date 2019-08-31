/**
*	@file	TitleScene.h
*	@brief	タイトルシーン制御用
*	@author Takuya Yokoyama
*/
#pragma once
#include "../Engine/Entity.h"
#include "../user/Entity/Player.h"
#include "../Engine/Scene.h"
#include <memory>
#include "../Engine/Sprite.h"

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
		SpritePtr sampleSprite;

		std::shared_ptr<PlayerForProduction> player;
	};
}


