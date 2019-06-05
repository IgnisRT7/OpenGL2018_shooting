/**
*	@file EnemyLaunchController.h
*/

#include <vector>
#include <glm/glm.hpp>
#include "../Entity.h"

struct EnemyLaunchType {

	int launchCount;		///< 出撃数
	float launchInterval;	///< 出撃間隔
	int enemyType;			///< 敵の種類
	float launchStartTimer;	///< ステージ開始後に出撃する時間
	glm::vec3 startPostion;	///< 初期地点

	bool operator>(const EnemyLaunchType& cmp) const {
		return launchStartTimer > cmp.launchStartTimer;
	}
	bool operator<(const EnemyLaunchType& cmp) const { 
		return launchStartTimer < cmp.launchStartTimer;
	}
};

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

	void Update(float deltaTime);
	void Launch();
	bool IsFinish() { return isFinished; }

private:

	std::vector<EnemyLaunchType> launchList;
	std::vector<EnemyLaunchType> activeList;
	int seekIndex = 0;
	float timer = 0;
	bool isFinished;

};

using EnemyLaunchControllerPtr = std::shared_ptr<EnemyLaunchController>;