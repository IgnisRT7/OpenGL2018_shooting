/**
*	@file player.h
*/
#pragma once

#include "../../Entity.h"
#include "../BulletManager.h"

namespace GameState {

	/*
	*	�v���C���[�N���X	�Q�[����ʑ��c�p
	*/
	class Player : public Entity::EntityDataBase {
	public:

		void Initialize() override;
		void Update(float delta) override;
		void CollisionEnter(Entity::Entity& entity) override;
		void Damage(float p) override;

		void StartMove(float delta);
		void ShotBullet();

		int RemainingPlayer()const { return remainingPlayer; }

	private:

		bool isStartingMove = true;		/// �X�^�[�g����̈ړ�����
		float startMovValue = 20;		/// �X�^�[�g����̈ړ���
		float moveSpeed = 5.0f;

		float timer = 0;
		float damageTimer;				/// ���G����

		glm::vec3 moveBox[2] =
		{ {-25, -120, -1},{25, 100, 80} };		/// �v���C���[�̉���

		BulletManagerPtr bulletManager;	

		float shotInterval = 0;			/// ���˂����܂ł̃N�[���^�C��
		int multiShotNum = 1;			/// ��x�ɔ��˂ł���e��

		int remainingPlayer = 0;		/// �v���C���[�c�@
	};

	/**
	*	�v���C���[�N���X�@�^�C�g����ʉ��o�p
	*/
	class PlayerForProduction :public Entity::EntityDataBase {
	public:
		void Initialize() override;
		void Update(float delta) override {};
		void Color(glm::vec4 c) { entity->Color(c); }

		void MoveStart() { entity->Velocity(glm::vec3(0, 0, moveSpeed)); }

	private:

		bool isMoveStart = false;
		float timer = 0;
		float moveSpeed = 20.0f;
	};


}