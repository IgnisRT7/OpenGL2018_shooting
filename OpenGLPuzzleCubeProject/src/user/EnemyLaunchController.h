/**
*	@file EnemyLaunchController.h
*/
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../Engine/Entity.h"
#include "EnemyMoveController.h"

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

	void Init(int stageNum);
	void Load(const std::string& filename);

	void Update(float deltaTime);
	void Launch();
	bool IsFinish() const { return isFinished; }
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