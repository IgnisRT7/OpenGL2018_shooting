/**
*	@file Effect.h
*	@brief	3D���f�����g�p�����^���G�t�F�N�g
*	@author	TakuyaYokoyama
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