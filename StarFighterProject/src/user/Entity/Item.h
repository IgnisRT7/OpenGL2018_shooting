/**
*	@file Item.h
*	@brief	プレイヤーを強化するアイテム
*	@author	Takuya Yokoyama
*/
#pragma once 

#include "../../Engine/Entity.h"

namespace Application {

	/// アイテム
	class Item : public Entity::EntityDataBase {
	public:

		/**
		*	コンストラクタ
		*
		*	@param type	適用するアイテムタイプ
		*/
		Item(int type = 0) : itemType(type) {}
		~Item() = default;
		Item(const Item&) = delete;
		const Item& operator=(const Item&) = delete;

		/**
		*	アイテムの初期化処理
		*/
		void Initialize() override;

		/**
		*	アイテムの更新処理
		*
		*	@param  delta	経過時間
		*/
		void Update(float delta) override;

		/**
		*	アイテムの衝突判定処理
		*
		*	@param e	衝突してきたエンティティ
		*/
		void CollisionEnter(Entity::Entity& e) override;

		/**
		*	アイテムタイプの取得
		*
		*	@return アイテムタイプ
		*/
		int ItemType() { return itemType; }

	private:

		int itemType = 1;	/// アイテムの種類
	};

}