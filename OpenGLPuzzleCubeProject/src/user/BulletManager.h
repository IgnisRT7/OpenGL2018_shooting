/**
*	@file BulletManager.h
*/
#pragma once
#include "../Entity.h"


namespace GameState {

	///’e‚ÌŠÇ—ƒVƒXƒeƒ€
	class BulletManager {
	public:

		BulletManager(Entity::Entity& p,int groupId, Entity::Entity* t = nullptr);
		

		virtual void Update(float delta);
		virtual void CalcVelocity();
		void Target(Entity::Entity* t) { target = t; CalcVelocity(); }
		void GroupId(int i) { groupId = i; }

	protected:
		float timer;
		float shotInterval = 2;		/// ’e‚Ì”­ËŠÔŠu
		float initBulletSpeed = 1;
		int groupId = -1;

		Entity::Entity& parent;		/// ’e‚Ì”­ËŒ³
		Entity::Entity* target;		/// ’e‚Ì’Ç”ö‘ÎÛ
	};

	using BulletManagerPtr = std::shared_ptr<BulletManager>;

	///îó‚É5•ûŒü‚É’e‚ğ”­Ë‚·‚é
	class MultiWayShot : public BulletManager {
	public:
		MultiWayShot(Entity::Entity& p,int i, Entity::Entity* t = nullptr, float a = 15.f,int m = 3);

		void Update(float delta) override;

	private:

		float angleInterval;
		int maxBulletNum;
	};

	///‰ñ“]‚µ‚È‚ª‚ç˜A‘±‚Å’e‚ğ”­Ë‚·‚é
	class CircleShot : public BulletManager {
	public:

		CircleShot(Entity::Entity& parent, int i, float angleInterval = 15.f, float shotAngle = 0.f);

		void Update(float delta) override;

	private:

		float angleInterval;
		float shotAngle;

	};

}
