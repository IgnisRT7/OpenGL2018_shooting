/**
*	@file TitleScene.h
*/
#pragma once
#include "../Entity.h"
#include "../user/Entity/Player.h"

namespace GameState {

	///�w�i�̍X�V
	class TitleSpaceSphere : public Entity::EntityDataBase {

		void Initialize() override;

		void Update(double delta) override;
	};

	///�^�C�g�����
	class Title {
	public:

		Title();

		void Initialize();

		void operator()(double delta) ;

	private:

		bool initial = true;
		float timer = 0;

		std::shared_ptr<PlayerForProduction> player;
	};
}


