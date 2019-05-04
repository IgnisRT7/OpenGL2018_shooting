/**
*	@file GameMainScene.h
*/
#pragma once
#include "../Entity.h"


namespace GameState {

	///背景(スライド)
	class Landscape : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(double delta) override;
	};

	///背景の更新(回転)
	class SpaceSphereMain : public Entity::EntityDataBase {

		void Initialize() override;

		void Update(double delta) override;
	};
	
	/**
	*	爆発の更新
	*/
	class Blast : public Entity::EntityDataBase{
	public:

		void Initialize() override;

		void Update(double delta) override;

	private:

		double timer = 0;
		bool isInitialized = false;
	};

	/**
	*	自機の弾の更新
	*/
	class PlayerShot : public Entity::EntityDataBase{
	public:

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& e) override;

	private:

		//glm::vec3 movVec = { 0,0,80 };
	};

	/// アイテム
	class Item : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& e) override ;

		int ItemType() { return itemType; }

	private:

		int itemType = 1;
	};


	/// プレイヤー
	class Player : public Entity::EntityDataBase{
	public:

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& entity) override;

		void damage();

		void StartMove(double delta);

	private:

		bool isStartingMove = true;		/// スタート直後の移動処理
		float startMovValue = 20;		/// スタート直後の移動量
		float moveSpeed = 5.0f;

		float timer = 0;
		float damageTimer;

		glm::vec3 moveBox[2] =
		{ {-25, -120, -1},{25, 100, 80} };		/// プレイヤーの可動域

		double shotInterval = 0;	/// 発射されるまでのクールタイム
		int multiShotNum = 1;			/// 一度に発射できる弾数


	};

	/// 敵
	class Toroid : public Entity::EntityDataBase{
	public:

		Toroid(int typeID = 0) :enemyType(typeID) {}

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& e) override;

	private:

		float timer = 0;
		int enemyType = 0;
		int hp = 2;

	};

	/// 敵スポナー
	class EnemyLaunchType : public Entity::EntityDataBase{
	public:

		void Initialize() override {}

		void Update(double delta) override;

		float spawnInterval = 0.5;	//スポーンする間隔
		float spawnMax = 5;			//スポーン数
		float time;					//経過時間
		int launchIndex = -1;		//出撃している敵の数
		int enemyType = -1;			//出撃する敵の種類
	};



	///メインゲーム画面
	class MainGame {
	public:

		MainGame();
		void operator()(double delta);

	private:

		//UpdatePlayer playerData;

		double interval = 0;
		int stageNo = 0;
		double stageTimer = -1;

	};

}