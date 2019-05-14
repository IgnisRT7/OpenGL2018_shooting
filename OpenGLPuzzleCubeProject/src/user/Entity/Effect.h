/**
*	@file Effect.h
*/
#pragma once
#include "../../Entity.h"


namespace GameState {


	/**
	*	�����̍X�V
	*/
	class Blast : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(double delta) override;

	private:

		double timer = 0;			/// �o�ߎ���

	};
}