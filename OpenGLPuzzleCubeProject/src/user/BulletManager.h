/**
*	@file BulletManager.h
*/
#pragma once
#include "../Entity.h"

namespace GameState {

	///�e�̊Ǘ��V�X�e��
	class BulletManager {
	public:

		BulletManager(Entity::Entity& p, Entity::Entity* t = nullptr);

		virtual void Update(float delta);


	protected:
		float timer = 0;
		float shotInterval = 2;		/// �e�̔��ˊԊu

		Entity::Entity& parent;		/// �e�̔��ˌ�
		Entity::Entity* target;		/// �e�̒ǔ��Ώ�
	};

	using EnemyBulletManagerPtr = std::shared_ptr<BulletManager>;

	class FiveWayShot : public BulletManager {
	public:
		FiveWayShot(Entity::Entity& p, Entity::Entity* t = nullptr) :BulletManager(p, t) {}

		void Update(float delta) override;
	};
}
