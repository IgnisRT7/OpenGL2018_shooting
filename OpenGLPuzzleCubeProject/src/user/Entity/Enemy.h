/**
*	@file Entity.h
*/
#pragma once

#include "../../Entity.h"

namespace GameState {

	/// 敵
	class Toroid : public Entity::EntityDataBase {
	public:

		Toroid(int typeID = 0, bool itemDrop = false) : enemyType(typeID), isItemDrop(itemDrop) {}

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& e) override;

	private:

		float timer = 0;
		int enemyType = 0;
		int hp = 2;

		bool isItemDrop = false;
	};

	/// 敵スポナー
	class EnemyLaunchType : public Entity::EntityDataBase {
	public:

		void Initialize() override {}

		void Update(double delta) override;

		float spawnInterval = 0.5;	//スポーンする間隔
		float spawnMax = 5;			//スポーン数
		float time;					//経過時間
		int launchIndex = -1;		//出撃している敵の数
		int enemyType = -1;			//出撃する敵の種類
	};
}