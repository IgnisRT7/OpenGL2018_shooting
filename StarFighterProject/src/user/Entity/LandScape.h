/**
*	@file	Landscape.h
*	@brief	背景用エンティティ制御
*	@author	Takuya Yokoyama
*/

#pragma once
#include "../../Engine/Entity.h"

namespace Application {

	///背景(スライド)
	class Landscape : public Entity::EntityDataBase {
	public:

		//Landscape() = default;
		~Landscape() = default;
		Landscape(const Landscape&) = delete;
		const Landscape& operator=(const Landscape&) = delete;

		/**
		*	コンストラクタ
		*
		*	@param l	ループするかどうかのフラグ
		*/
		Landscape(bool l = false);

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

	private:

		float timer = 0;
		glm::vec3 start;
		bool isLoop = false;
	};

	///背景の更新(回転)
	class SpaceSphereMain : public Entity::EntityDataBase {

		/**
		*	背景の初期化処理
		*/
		void Initialize() override;

		/**
		*	更新処理
		*
		*	@param deltaTime	経過時間
		*/
		void Update(float deltaTime) override;
	};

}