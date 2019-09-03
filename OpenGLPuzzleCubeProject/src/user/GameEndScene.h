/**
*	@file GameEndScene.h
*	@brief	�Q�[���I�[�o�[���̃V�[������p
*	@author	Takuya Yokoyama
*/
#pragma once

#include "TitleScene.h"
#include "../Engine/Scene.h"

namespace Application{

	///�^�C�g�����
	class GameEnd : public Scene{
	public:

		GameEnd(bool clear = false) :isClear(clear),Scene("GameEnd") {}

		/**
		*	����������
		*
		*	@return true	����������
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
		void Hide() override {}
		
	private:

		struct FontDrawInfo {
			glm::vec2 pos;
			glm::vec2 size;
			glm::vec4 color;
			std::string str;
			bool isCenter;
		};

		FontDrawInfo scoreStrInfo, highScoreStrInfo,gameoverStrInfo,pressButtonStrInfo;

		TitleSpaceSphere spaceSphere;

		float timer = 0;
		float strFadeTimer = 0;
		bool isClear;


	};
}