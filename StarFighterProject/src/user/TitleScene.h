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

namespace Application {

	///タイトル画面
	class Title : public Scene{
	public:

		Title() : Scene("Title") {}
		~Title() = default;
		Title(const Title&) = delete;
		const Title& operator=(const Title&) = delete;

		/**
		*	初期化処理
		*
		*	@retval true	初期化成功
		*	@retval false	初期化失敗
		*/
		bool Initialize() override;

		/**
		*	更新処理
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
		
		float timer = 0;
		int selectAirCraftType = 0;
		SpritePtr sampleSprite;

		std::shared_ptr<PlayerForProduction> player;
	};
}


