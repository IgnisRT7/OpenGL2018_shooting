/**
*	@file player.h
*/
#pragma once

#include "../../Entity.h"
#include "../BulletManager.h"
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

		void StartMove(float delta);

		int RemainingPlayer()const { return remainingPlayer; }

	private:

		bool isStartingMove = true;		/// スタート直後の移動処理
		float startMovValue = 20;		/// スタート直後の移動量
		float moveSpeed = 15.0f;
		float moveMultiply = 1.0f;

		float timer = 0;
		float damageTimer;				/// 無敵時間
		float shotInterval = 0;			/// 発射されるまでのクールタイム

		const glm::vec3 moveBox[2] =
		{	glm::vec3(-screenHalfW, -120, -screenHalfH) * 0.8f,
			glm::vec3(screenHalfW, 100, screenHalfH) * 0.8f };		/// プレイヤーの可動域

		BulletManagerPtr bulletManager;	

		int remainingPlayer = 3;		/// プレイヤー残機
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