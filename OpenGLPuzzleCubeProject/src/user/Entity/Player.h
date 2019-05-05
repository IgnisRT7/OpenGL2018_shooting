/**
*	@file player.h
*/
#pragma once

#include "../../Entity.h"

namespace GameState {
	/// �v���C���[
	class Player : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& entity) override;

		void damage();

		void StartMove(double delta);

	private:

		bool isStartingMove = true;		/// �X�^�[�g����̈ړ�����
		float startMovValue = 20;		/// �X�^�[�g����̈ړ���
		float moveSpeed = 5.0f;

		float timer = 0;
		float damageTimer;

		glm::vec3 moveBox[2] =
		{ {-25, -120, -1},{25, 100, 80} };		/// �v���C���[�̉���

		double shotInterval = 0;	/// ���˂����܂ł̃N�[���^�C��
		int multiShotNum = 1;			/// ��x�ɔ��˂ł���e��

	};
}