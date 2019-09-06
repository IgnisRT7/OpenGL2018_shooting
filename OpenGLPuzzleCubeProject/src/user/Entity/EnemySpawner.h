/**
*	@file	EnemySpawner
*	@brief	艦隊単位での敵のスポナー管理
*	@author	Takuya Yokoyama
*/

#pragma once

#include "../../Engine/Entity.h"
#include "../../Engine/GameEngine.h"
#include "Player.h"
#include "Enemy.h"
#include "../EnemyLaunchController.h"

namespace Application {

	/// 敵スポナー
	class EnemySpawner : public Entity::EntityDataBase {
	public:

		EnemySpawner() = default;

		/**
		*	コンストラクタ
		*	
		*	@param max			敵の生成数
		*	@param interval		生成の間隔
		*	@param enemyType	敵のタイプ
		*	@param moveType		移動タイプ
		*	@param health		体力
		*/
		EnemySpawner(int max, float interval, int enemyType, int moveType, int bulletType, int health);
		~EnemySpawner() = default;
		const EnemySpawner& operator=(const EnemySpawner&) = delete;

		/**
		*	初期化処理
		*/
		void Initialize() override;

		/**
		*	更新処理
		*
		*	@param deltaTime	経過時間
		*/
		void Update(float deltaTime) override;

		/**
		*	敵のスポーン処理
		*/
		void SpawnEnemy();

	private:

		float spawnInterval = 0.5;	///< スポーンする間隔
		float time = 0;				///< 経過時間
		int spawnMax = 5;			///< スポーン数
		int launchIndex = 0;		///< 出撃している敵の数
		int enemyType = -1;			///< 出撃する敵の種類
		int moveType = -1;
		int bulletType = -1;		///< 弾の種類 
		int health = 1;				///< 体力

		Entity::Entity* playerEntity;
	};

}