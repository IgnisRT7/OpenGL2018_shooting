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


	protected:
		float timer = 0;
		float shotInterval = 2;		/// ’e‚Ì”­ËŠÔŠu

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
