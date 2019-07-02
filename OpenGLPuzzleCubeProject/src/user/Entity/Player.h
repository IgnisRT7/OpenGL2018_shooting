/**
*	@file player.h
*/
#pragma once

#include "../../Engine/Entity.h"
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

		void StartMoveSet();
		void EndMoveSet();
		void AutoMove(float delta);
		void ShotBullet();

		int RemainingPlayer()const { return remainingPlayer; }

	private:

		bool initialized = false;		
		bool isAutoMove = false;		///< Velocity�l�ɂ�鎩�����샂�[�h
		bool isStartingMove = true;		///< �X�^�[�g����̈ړ�����
		float startMovValue = 20;		///< �X�^�[�g����̈ړ���
		float moveSpeed = 15.0f;		///< �x�[�X�ƂȂ�X�s�[�h
		float moveMultiply = 1.0f;		///< ���Z���鑬�x

		float timer = 0;
		float damageTimer;				///< ���G����
		float shotInterval = 0;			///< ���˂����܂ł̃N�[���^�C��

		int multiShotNum = 1;

		glm::vec3 goalLocation;			///< isAutoMove��true�̎��̃S�[���n�_

		const glm::vec3 moveBox[2] =	///< �v���C���[�̈ړ��\�ȗ̈�
		{	glm::vec3(-screenHalfW, -120, -screenHalfH) * 0.9f,
			glm::vec3(screenHalfW, 100, screenHalfH) * 0.9f };		

		int remainingPlayer = 3;		///< �v���C���[�c�@
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