/**
*	@file player.h
*/
#pragma once

#include "../../Engine/Entity.h"
#include "../../GameState.h"

namespace GameState {

	/*
	*	プレイヤークラス	ゲーム画面操縦用
	*/
	class Player : public Entity::EntityDataBase {
	public:

		Player() = default;
		Player(const Player&) = delete;
		void operator=(const Player&) = delete;

		void Initialize() override;
		void Update(float delta) override;
		void CollisionEnter(Entity::Entity& entity) override;
		void Damage(float p) override;

		void StartMoveSet();
		void EndMoveSet(float delay);
		void AutoMove(float delta);
		void ShotBullet();

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
		{	glm::vec3(-screenHalfW, -120, -screenHalfH) * 0.9f,
			glm::vec3(screenHalfW, 100, screenHalfH) * 0.9f };		

		int remainingPlayer = 3;		///< プレイヤー残機
	};

	/**
	*	プレイヤークラス　タイトル画面演出用
	*/
	class PlayerForProduction :public Entity::EntityDataBase {
	public:
		void Initialize() override;
		void Update(float delta) override {};
		void Color(glm::vec4 c) { entity->Color(c); }

		void MoveStart() { entity->Velocity(glm::vec3(0, 0, moveSpeed)); }

	private:

		bool isMoveStart = false;
		float timer = 0;
		float moveSpeed = 20.0f;
	};


}