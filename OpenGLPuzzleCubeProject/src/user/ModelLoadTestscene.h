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


		bool Initialize() override;
		void Update(float d) override;
		void Finalize() override;

		void Play() override;
		void Stop() override;
		void Hide() override;

	private:

		float moveSpeed = 10.0f;
		float rotateSpeed = 10.0f;

	};
}