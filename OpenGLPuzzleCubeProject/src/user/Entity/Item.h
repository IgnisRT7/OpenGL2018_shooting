/**
*	@file Item.h
*	@brief	�v���C���[����������A�C�e��
*	@author	Takuya Yokoyama
*/
#pragma once 

#include "../../Engine/Entity.h"

namespace GameState {

	/// �A�C�e��
	class Item : public Entity::EntityDataBase {
	public:

		Item(int type = 0) : itemType(type) {}

		void Initialize() override;

		void Update(float delta) override;

		void CollisionEnter(Entity::Entity& e) override;

		int ItemType() { return itemType; }

	private:

		int itemType = 1;	/// �A�C�e���̎��
	};

}