/**
*	@file Item.h
*/
#pragma once 

#include "../../Engine/Entity.h"

namespace GameState {

	/// �A�C�e��
	class Item : public Entity::EntityDataBase {
	public:

		Item::Item(int type = 0) : itemType(type) {}

		void Initialize() override;

		void Update(float delta) override;

		void CollisionEnter(Entity::Entity& e) override;

		int ItemType() { return itemType; }

	private:

		int itemType = 1;	/// �A�C�e���̎��
	};

}