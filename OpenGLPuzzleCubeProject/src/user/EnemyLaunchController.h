/**
*	@file EnemyLaunchController.h
*/
#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "../Engine/Entity.h"
#include "EnemyMoveController.h"

///敵の出撃タイプデータ
struct EnemyLaunchType {

	float launchStartTimer;	///< ステージ開始後に出撃する時間
	int launchCount;		///< 出撃数
	float launchInterval;	///< 出撃間隔

	int enemyType;			///< 敵の種類
	int moveType;			///< 敵の挙動
	int bulletType;			///< 弾の種類
	int health;				///< 体力

	glm::vec3 startPostion;	///< 初期地点

	bool operator>(const EnemyLaunchType& cmp) const {
		return launchStartTimer > cmp.launchStartTimer;
	}
	bool operator<(const EnemyLaunchType& cmp) const { 
		return launchStartTimer < cmp.launchStartTimer;
	}
};

MoveControllerPtr MakeMoveControllerByMoveType(int type, bool inverse = false);

/**
*	1ステージ分の敵の出撃管理クラス
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