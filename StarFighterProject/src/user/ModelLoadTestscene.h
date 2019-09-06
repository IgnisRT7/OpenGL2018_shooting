/**
*	@file ModelLoadTestScene.h
*	@brief	テスト用シーン制御用
*	@author	Takuya Yokoyama
*/
#pragma once
#include "../Engine/Scene.h"

namespace Application {

	struct ModelLoadTestScene : Scene {
	public:

		ModelLoadTestScene();
		~ModelLoadTestScene() = default;
		ModelLoadTestScene(const ModelLoadTestScene&) = delete;
		ModelLoadTestScene& operator=(const ModelLoadTestScene&) = delete;

		/**
		*	初期化処理
		*
		*	@retval true	初期化成功
		*	@retval false	初期化失敗
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
		void Hide() override;

	private:

		float moveSpeed = 10.0f;
		float rotateSpeed = 10.0f;

	};
}