/**
*	@file Item.h
*/
#pragma once 

#include "../../Entity.h"

namespace GameState {

	/// ƒAƒCƒeƒ€
	class Item : public Entity::EntityDataBase {
	public:

		Item::Item(int type = 0) : itemType(type) {}

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& e) override;

		int ItemType() { return itemType; }

	private:

		int itemType = 1;
	};

}