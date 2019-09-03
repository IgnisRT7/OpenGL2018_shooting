/**
*	@file Item.h
*	@brief	�v���C���[����������A�C�e��
*	@author	Takuya Yokoyama
*/
#pragma once 

#include "../../Engine/Entity.h"

namespace Application {

	/// �A�C�e��
	class Item : public Entity::EntityDataBase {
	public:

		/**
		*	�R���X�g���N�^
		*
		*	@param type	�K�p����A�C�e���^�C�v
		*/
		Item(int type = 0) : itemType(type) {}
		~Item() = default;
		Item(const Item&) = delete;
		const Item& operator=(const Item&) = delete;

		/**
		*	�A�C�e���̏���������
		*/
		void Initialize() override;

		/**
		*	�A�C�e���̍X�V����
		*
		*	@param  delta	�o�ߎ���
		*/
		void Update(float delta) override;

		/**
		*	�A�C�e���̏Փ˔��菈��
		*
		*	@param e	�Փ˂��Ă����G���e�B�e�B
		*/
		void CollisionEnter(Entity::Entity& e) override;

		/**
		*	�A�C�e���^�C�v�̎擾
		*
		*	@return �A�C�e���^�C�v
		*/
		int ItemType() { return itemType; }

	private:

		int itemType = 1;	/// �A�C�e���̎��
	};

}