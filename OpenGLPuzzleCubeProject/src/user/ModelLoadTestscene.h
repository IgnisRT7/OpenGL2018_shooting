/**
*	@file ModelLoadTestScene.h
*/
#pragma once

struct ModelLoadTestScene {
public:

	ModelLoadTestScene();
	void operator()(float delta);


private:

	float moveSpeed = 10.0f;
	float rotateSpeed = 10.0f;

};
