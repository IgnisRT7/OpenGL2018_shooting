/**
*	@file	Bullet.h
*	@brief	�e�̃G���e�B�e�B����p
*	@author	Takuya Yokoyama
*/
#pragma once

#include "../../Engine/Entity.h"

namespace Application {

	/// �e�̃G���e�B�e�B�N���X
	class Bullet : public Entity::EntityDataBase {
	public:
		
		Bullet() = default;
		virtual ~Bullet() = default;
		Bullet(const Bullet&) = delete;
		const Bullet& operator=(const Bullet&) = delete;

		/**
		*	����������
		*/
		void Initialize() override;

		/**
		*	�e�̍X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void Update(float deltaTime) override;	

		/**
		*	�Փˏ���
		*
		*	@param e	�Փ˂��Ă����G���e�B�e�B
		*/
		void CollisionEnter(Entity::Entity& e) override;

		/**
		*	�ړ��ʂ̎擾
		*
		*	@return �ړ���
		*/
		glm::vec3 Velocity() const { return velocity; }

		/**
		*	�ړ��ʂ̐ݒ�
		*
		*	@param v	�ݒ肷��ړ���
		*/
		void Velocity(glm::vec3& v) { velocity = v; }

	private:

		glm::vec3 velocity;

	};


}
