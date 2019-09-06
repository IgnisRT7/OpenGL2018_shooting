/**
*	@file Effect.h
*	@brief	3Dモデルを使用した疑似エフェクト
*	@author	TakuyaYokoyama, tn-mai(講義資料製作者)
*/
#pragma once
#include "../../Engine/Entity.h"


namespace Application {


	/**
	*	爆発の更新
	*/
	class Blast : public Entity::EntityDataBase {
	public:

		/**
		*	初期化処理
		*/
		void Initialize() override;

		/**
		*	更新処理
		*
		*	@param deltaTime	経過時間
		*/
		void Update(float delta) override;

	private:

		float timer = 0;			/// 経過時間

	};
}