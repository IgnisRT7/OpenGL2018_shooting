/**
*	@file	Player.h
*	@brief	プレイヤー制御用
*	@author	Takuya Yokoyama , tn-mai(講義資料製作者)
*/
#pragma once

#include "../../Engine/Entity.h"
#include "../../GameState.h"

namespace Application {

	/*
	*	プレイヤークラス	ゲーム画面操縦用
	*/
	class Player : public Entity::EntityDataBase {
	public:

		Player() = default;
		Player(const Player&) = delete;
		void operator=(const Player&) = delete;
		~Player() = default;

		/**
		*	初期化処理
		*/
		void Initialize() override;

		/**
		*	更新処理
		*
		*	@param delta	経過時間
		*/
		void Update(float delta) override;

		/**
		*	衝突判定処理
		*
		*	@param entity	衝突してきたエンティティ
		*/
		void CollisionEnter(Entity::Entity& entity) override;

		/**
		*	ダメージ処理
		*
		*	@param p	ダメージ量
		*/
		void Damage(float p) override;

		/**
		*	ステージ開始時の演出用設定処理
		*/
		void StartMoveSet();

		/**
		*	ステージクリア時の演出用設定処理
		*
		*	@param delayTime	この関数を呼び出してから動き出す時間
		*/
		void EndMoveSet(float delayTIme);

		/**
		*	ステージ開始後もしくはクリア後に行われる処理
		*
		*	@param deltaTime	経過時間
		*/
		void AutoMove(float deltaTime);

		/**
		*	弾の発射処理
		*/
		void ShotBullet();

		/**
		*	残機の取得
		*
		*	@return 残機
		*/
		int RemainingPlayer()const { return remainingPlayer; }

	private:

		bool isAutoMove = false;		///< Velocity値による自動操作モード
		float autoMoveStartTimer;		///< 移動開始するまでの時間
		float startMovValue = 20;		///< スタート直後の移動量
		glm::vec3 autoMoveVel;			///< 自動モード時の移動量

		bool initialized = false;		

		float moveSpeed = 15.0f;		///< ベースとなるスピード
		float moveMultiply = 1.0f;		///< 加算する速度

		float timer = 0;

		float damageTimer;				///< 無敵時間
		float shotInterval = 0;			///< 発射されるまでのクールタイム

		int multiShotNum = 1;

		glm::vec3 goalLocation;			///< isAutoMoveがtrueの時のゴール地点

		const glm::vec3 moveBox[2] =	///< プレイヤーの移動可能な領域
		{	glm::vec3(-GameState::screenHalfW, -120, -GameState::screenHalfH) * 0.9f,
			glm::vec3(GameState::screenHalfW, 100, GameState::screenHalfH) * 0.9f };

		int remainingPlayer = 3;		///< プレイヤー残機
	};

	/**
	*	プレイヤークラス　タイトル画面演出用
	*/
	class PlayerForProduction :public Entity::EntityDataBase {
	public:

		/**
		*	初期化処理
		*/
		void Initialize() override;

		/**
		*	更新処理
		*/
		void Update(float delta) override {};

		/**
		*	プレイヤーの機体色の設定
		*
		*	@param c	設定する色
		*/
		void Color(glm::vec4 c) { entity->Color(c); }

		/**
		*	自動スタート用処理
		*/
		void MoveStart() { entity->Velocity(glm::vec3(0, 0, moveSpeed)); }

	private:

		bool isMoveStart = false;
		float timer = 0;
		float moveSpeed = 20.0f;
	};


}