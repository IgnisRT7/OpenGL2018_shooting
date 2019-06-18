/**
*	@file Effect.h
*/
#pragma once
#include "../../Engine/Entity.h"


namespace GameState {


	/**
	*	�����̍X�V
	*/
	class Blast : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(float delta) override;

	private:

		float timer = 0;			/// �o�ߎ���

	};
}