/**
*	@file Effect.h
*	@brief	3D���f�����g�p�����^���G�t�F�N�g
*	@author	TakuyaYokoyama, tn-mai(�u�`���������)
*/
#pragma once
#include "../../Engine/Entity.h"


namespace Application {


	/**
	*	�����̍X�V
	*/
	class Blast : public Entity::EntityDataBase {
	public:

		/**
		*	����������
		*/
		void Initialize() override;

		/**
		*	�X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void Update(float delta) override;

	private:

		float timer = 0;			/// �o�ߎ���

	};
}