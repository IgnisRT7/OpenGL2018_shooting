/**
*	@file player.h
*/
#pragma once

#include "../../Entity.h"
#include "../BulletManager.h"
#include "../../GameState.h"

namespace GameState {

	/*
	*	�v���C���[�N���X	�Q�[����ʑ��c�p
	*/
	class Player : public Entity::EntityDataBase {
	public:

		Player() = default;
		Player(const Player&) = delete;
		void operator=(const Player&) = delete;

		void Initialize() override;
		void Update(float delta) override;
		void CollisionEnter(Entity::Entity& entity) override;
		void Damage(float p) override;

		void StartMove(float delta);

		int RemainingPlayer()const { return remainingPlayer; }

	private:

		bool isStartingMove = true;		/// �X�^�[�g����̈ړ�����
		float startMovValue = 20;		/// �X�^�[�g����̈ړ���
		float moveSpeed = 15.0f;
		float moveMultiply = 1.0f;

		float timer = 0;
		float damageTimer;				/// ���G����
		float shotInterval = 0;			/// ���˂����܂ł̃N�[���^�C��

		const glm::vec3 moveBox[2] =
		{	glm::vec3(-screenHalfW, -120, -screenHalfH) * 0.8f,
			glm::vec3(screenHalfW, 100, screenHalfH) * 0.8f };		/// �v���C���[�̉���

		BulletManagerPtr bulletManager;	

		int remainingPlayer = 3;		/// �v���C���[�c�@
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