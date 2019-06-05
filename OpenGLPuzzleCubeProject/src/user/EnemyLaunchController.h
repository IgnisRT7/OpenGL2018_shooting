/**
*	@file EnemyLaunchController.h
*/

#include <vector>

struct EnemyLaunchType {

	int launchCount;	///< 出撃数
	float launchInterval;	///< 出撃間隔
	int enemyType;		///< 敵の種類
	float launchStartTimer;	///< ステージ開始後に出撃する時間
};

/**
*	1ステージ分の敵の出撃管理クラス
*/
class EnemyLaunchController {
public:

	EnemyLaunchController();
	~EnemyLaunchController();
	EnemyLaunchController(const EnemyLaunchController&) = delete;
	void operator=(const EnemyLaunchController&) = delete;

	void Init(int stageNum);

	void Update(float delta);

private:

	std::vector<EnemyLaunchType> launchList;



};
