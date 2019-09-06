/**
*	@file EnemyLaunchController.h
*	@brief	�G�̏o���V�X�e���̐���p
*	@author	Takuya Yokoyama
*/
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../Engine/Entity.h"
#include "EnemyMoveController.h"

namespace Application {

	///�G�̏o���^�C�v�f�[�^
	struct EnemyLaunchType {

		float launchStartTimer;	///< �X�e�[�W�J�n��ɏo�����鎞��
		int launchCount;		///< �o����
		float launchInterval;	///< �o���Ԋu

		int enemyType;			///< �G�̎��
		int moveType;			///< �G�̋���
		int bulletType;			///< �e�̎��
		int health;				///< �̗�

		glm::vec3 startPostion;	///< �����n�_

		bool operator>(const EnemyLaunchType& cmp) const {
			return launchStartTimer > cmp.launchStartTimer;
		}
		bool operator<(const EnemyLaunchType& cmp) const {
			return launchStartTimer < cmp.launchStartTimer;
		}
	};

	MoveControllerPtr MakeMoveControllerByMoveType(int type, bool inverse = false);

	/**
	*	1�X�e�[�W���̓G�̏o���Ǘ��N���X
	*/
	class EnemyLaunchController {
	public:

		EnemyLaunchController() = default;
		~EnemyLaunchController() {}
		EnemyLaunchController(const EnemyLaunchController&) = delete;
		void operator=(const EnemyLaunchController&) = delete;

		/**
		*	����������
		*
		*	@param stageNum	���[�h����X�e�[�W�̔ԍ�
		*/
		void Init(int stageNum);

		/**
		*	�X�e�[�W�f�[�^�̓ǂݍ��ݏ���
		*
		*	@param filename �X�e�[�W�̃t�@�C����
		*/
		void Load(const std::string& filename);

		/**
		*	�X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void Update(float deltaTime);

		/**
		*	�I����Ԃ̎擾
		*
		*	@retval true	�o�����I������
		*	@retval false	�o����
		*/
		bool IsFinish() const { return isFinished; }

		/**
		*	�Ō�ɃX�|�[������G�̎��Ԃ̎擾
		*
		*	@return �Ō�ɃX�|�[�����鎞��
		*/
		float LastSpawnedTime() const { return lastSpawnTime; }

	private:

		std::vector<EnemyLaunchType> launchList;
		std::vector<EnemyLaunchType> activeList;
		int seekIndex = 0;
		float timer = 0;

		bool isFinished;
		float lastSpawnTime = 0;
	};

	using EnemyLaunchControllerPtr = std::shared_ptr<EnemyLaunchController>;

}