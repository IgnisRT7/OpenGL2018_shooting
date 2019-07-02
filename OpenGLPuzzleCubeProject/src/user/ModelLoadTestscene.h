/**
*	@file ModelLoadTestScene.h
*/
#pragma once
#include "../Engine/Scene.h"

struct ModelLoadTestScene : Scene{
public:

	ModelLoadTestScene() :Scene("ModelLoadTest") {}
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
