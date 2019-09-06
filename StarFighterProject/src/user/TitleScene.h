/**
*	@file	TitleScene.h
*	@brief	�^�C�g���V�[������p
*	@author Takuya Yokoyama
*/
#pragma once
#include "../Engine/Entity.h"
#include "../user/Entity/Player.h"
#include "../Engine/Scene.h"
#include <memory>
#include "../Engine/Sprite.h"

namespace Application {

	///�^�C�g�����
	class Title : public Scene{
	public:

		Title() : Scene("Title") {}
		~Title() = default;
		Title(const Title&) = delete;
		const Title& operator=(const Title&) = delete;

		/**
		*	����������
		*
		*	@retval true	����������
		*	@retval false	���������s
		*/
		bool Initialize() override;

		/**
		*	�X�V����
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
		
		float timer = 0;
		int selectAirCraftType = 0;
		SpritePtr sampleSprite;

		std::shared_ptr<PlayerForProduction> player;
	};
}


