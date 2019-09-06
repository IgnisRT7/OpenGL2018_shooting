/**
*	@file ModelLoadTestScene.h
*	@brief	�e�X�g�p�V�[������p
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
		*	����������
		*
		*	@retval true	����������
		*	@retval false	���������s
		*/
		bool Initialize() override;

		/**
		*	�X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void Update(float deltaTime) override;

		/**
		*	�I������
		*/
		void Finalize() override;

		/**
		*	�J�n����
		*/
		void Play() override;

		/**
		*	��~����
		*/
		void Stop() override;

		/**
		*	��\������
		*/
		void Hide() override;

	private:

		float moveSpeed = 10.0f;
		float rotateSpeed = 10.0f;

	};
}