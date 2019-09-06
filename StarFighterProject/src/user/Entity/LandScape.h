/**
*	@file	Landscape.h
*	@brief	�w�i�p�G���e�B�e�B����
*	@author	Takuya Yokoyama
*/

#pragma once
#include "../../Engine/Entity.h"

namespace Application {

	///�w�i(�X���C�h)
	class Landscape : public Entity::EntityDataBase {
	public:

		//Landscape() = default;
		~Landscape() = default;
		Landscape(const Landscape&) = delete;
		const Landscape& operator=(const Landscape&) = delete;

		/**
		*	�R���X�g���N�^
		*
		*	@param l	���[�v���邩�ǂ����̃t���O
		*/
		Landscape(bool l = false);

		/**
		*	����������
		*/
		void Initialize() override;

		/**
		*	�X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void Update(float deltaTime) override;

	private:

		float timer = 0;
		glm::vec3 start;
		bool isLoop = false;
	};

	///�w�i�̍X�V(��])
	class SpaceSphereMain : public Entity::EntityDataBase {

		/**
		*	�w�i�̏���������
		*/
		void Initialize() override;

		/**
		*	�X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void Update(float deltaTime) override;
	};

}