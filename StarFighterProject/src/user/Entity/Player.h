/**
*	@file	Player.h
*	@brief	�v���C���[����p
*	@author	Takuya Yokoyama , tn-mai(�u�`���������)
*/
#pragma once

#include "../../Engine/Entity.h"
#include "../../GameState.h"

namespace Application {

	/*
	*	�v���C���[�N���X	�Q�[����ʑ��c�p
	*/
	class Player : public Entity::EntityDataBase {
	public:

		Player() = default;
		Player(const Player&) = delete;
		void operator=(const Player&) = delete;
		~Player() = default;

		/**
		*	����������
		*/
		void Initialize() override;

		/**
		*	�X�V����
		*
		*	@param delta	�o�ߎ���
		*/
		void Update(float delta) override;

		/**
		*	�Փ˔��菈��
		*
		*	@param entity	�Փ˂��Ă����G���e�B�e�B
		*/
		void CollisionEnter(Entity::Entity& entity) override;

		/**
		*	�_���[�W����
		*
		*	@param p	�_���[�W��
		*/
		void Damage(float p) override;

		/**
		*	�X�e�[�W�J�n���̉��o�p�ݒ菈��
		*/
		void StartMoveSet();

		/**
		*	�X�e�[�W�N���A���̉��o�p�ݒ菈��
		*
		*	@param delayTime	���̊֐����Ăяo���Ă��瓮���o������
		*/
		void EndMoveSet(float delayTIme);

		/**
		*	�X�e�[�W�J�n��������̓N���A��ɍs���鏈��
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void AutoMove(float deltaTime);

		/**
		*	�e�̔��ˏ���
		*/
		void ShotBullet();

		/**
		*	�c�@�̎擾
		*
		*	@return �c�@
		*/
		int RemainingPlayer()const { return remainingPlayer; }

	private:

		bool isAutoMove = false;		///< Velocity�l�ɂ�鎩�����샂�[�h
		float autoMoveStartTimer;		///< �ړ��J�n����܂ł̎���
		float startMovValue = 20;		///< �X�^�[�g����̈ړ���
		glm::vec3 autoMoveVel;			///< �������[�h���̈ړ���

		bool initialized = false;		

		float moveSpeed = 15.0f;		///< �x�[�X�ƂȂ�X�s�[�h
		float moveMultiply = 1.0f;		///< ���Z���鑬�x

		float timer = 0;

		float damageTimer;				///< ���G����
		float shotInterval = 0;			///< ���˂����܂ł̃N�[���^�C��

		int multiShotNum = 1;

		glm::vec3 goalLocation;			///< isAutoMove��true�̎��̃S�[���n�_

		const glm::vec3 moveBox[2] =	///< �v���C���[�̈ړ��\�ȗ̈�
		{	glm::vec3(-GameState::screenHalfW, -120, -GameState::screenHalfH) * 0.9f,
			glm::vec3(GameState::screenHalfW, 100, GameState::screenHalfH) * 0.9f };

		int remainingPlayer = 3;		///< �v���C���[�c�@
	};

	/**
	*	�v���C���[�N���X�@�^�C�g����ʉ��o�p
	*/
	class PlayerForProduction :public Entity::EntityDataBase {
	public:

		/**
		*	����������
		*/
		void Initialize() override;

		/**
		*	�X�V����
		*/
		void Update(float delta) override {};

		/**
		*	�v���C���[�̋@�̐F�̐ݒ�
		*
		*	@param c	�ݒ肷��F
		*/
		void Color(glm::vec4 c) { entity->Color(c); }

		/**
		*	�����X�^�[�g�p����
		*/
		void MoveStart() { entity->Velocity(glm::vec3(0, 0, moveSpeed)); }

	private:

		bool isMoveStart = false;
		float timer = 0;
		float moveSpeed = 20.0f;
	};


}