/**
*	@file BulletManager.h
*/
#pragma once
#include "../Entity.h"

namespace GameState {

	///’e‚ÌŠÇ—ƒVƒXƒeƒ€
	class BulletManager {
	public:

		BulletManager(Entity::Entity& p, Entity::Entity* t = nullptr);

		virtual void Update(float delta);
		virtual void CalcVelocity();
		void Target(Entity::Entity* t) { target = t; CalcVelocity(); }


	protected:
		float timer = 0;
		float shotInterval = 2;		/// ’e‚Ì”­ËŠÔŠu
		float initBulletSpeed = 1;

		Entity::Entity& parent;		/// ’e‚Ì”­ËŒ³
		Entity::Entity* target;		/// ’e‚Ì’Ç”ö‘ÎÛ
	};

	using EnemyBulletManagerPtr = std::shared_ptr<BulletManager>;

	class FiveWayShot : public BulletManager {
	public:
		FiveWayShot(Entity::Entity& p, Entity::Entity* t = nullptr) :BulletManager(p, t) {}

		void Update(float delta) override;
	};
}
