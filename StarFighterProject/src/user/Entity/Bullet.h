/**
*	@file	Bullet.h
*	@brief	弾のエンティティ制御用
*	@author	Takuya Yokoyama
*/
#pragma once

#include "../../Engine/Entity.h"

namespace Application {

	/// 弾のエンティティクラス
	class Bullet : public Entity::EntityDataBase {
	public:
		
		Bullet() = default;
		virtual ~Bullet() = default;
		Bullet(const Bullet&) = delete;
		const Bullet& operator=(const Bullet&) = delete;

		/**
		*	初期化処理
		*/
		void Initialize() override;

		/**
		*	弾の更新処理
		*
		*	@param deltaTime	経過時間
		*/
		void Update(float deltaTime) override;	

		/**
		*	衝突処理
		*
		*	@param e	衝突してきたエンティティ
		*/
		void CollisionEnter(Entity::Entity& e) override;

		/**
		*	移動量の取得
		*
		*	@return 移動量
		*/
		glm::vec3 Velocity() const { return velocity; }

		/**
		*	移動量の設定
		*
		*	@param v	設定する移動量
		*/
		void Velocity(glm::vec3& v) { velocity = v; }

	private:

		glm::vec3 velocity;

	};


}
