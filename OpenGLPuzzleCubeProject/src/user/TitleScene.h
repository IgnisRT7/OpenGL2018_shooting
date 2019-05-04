/**
*	@file TitleScene.h
*/
#pragma once

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

		TitleSpaceSphere spaceSphere;
	};
}


