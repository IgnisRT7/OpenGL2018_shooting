/**
*	@file Effect.h
*	@brief	3Dモデルを使用した疑似エフェクト
*	@author	TakuyaYokoyama
*/
#pragma once
#include "../../Engine/Entity.h"


namespace GameState {


	/**
	*	爆発の更新
	*/
	class Blast : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(float delta) override;

	private:

		float timer = 0;			/// 経過時間

	};
}