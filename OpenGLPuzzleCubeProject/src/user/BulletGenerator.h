/**
*	@file BulletGenerator.h
*/
#pragma once
#include "../Engine/Entity.h"


namespace GameState {

	///’e‚Ì¶¬ƒVƒXƒeƒ€‚Ìƒx[ƒXƒNƒ‰ƒX
	class BulletGenerator {
	public:

		BulletGenerator(Entity::Entity& p,int groupId, Entity::Entity* t = nullptr);	

		virtual void Update(float delta) = 0;
		virtual void ShotBullet();
		virtual void CalcVelocity();

		void Target(Entity::Entity* t) { target = t; CalcVelocity(); }
		void GroupId(int i) { groupId = i; }
		void ShotInterval(float i) { shotInterval = i; }
		float ShotInterval() const { return shotInterval; }
		void BulletSpeed(float s) { initBulletSpeed = s; }
		float BulletSpeed() const { return initBulletSpeed; }
		void Color(glm::vec4 c) { color = c; }

	protected:

		float timer;					///< ”­ËŠÔŠu§Œä—pƒ^ƒCƒ}[
		float shotInterval = 2;			///< ’e‚Ì”­ËŠÔŠu
		float initBulletSpeed = 1;		///< ’e‚Ì‘¬“x
		int groupId = -1;				///< ’e‚ÌID
		glm::vec4 color = glm::vec4(1);	///< ”­Ë‚·‚é’e‚ÌF
		glm::vec3 direction;			///< ’e‚ğ”­Ë‚³‚¹‚éŒü‚«

		Entity::Entity& parent;		/// ’e‚Ì”­ËŒ³
		Entity::Entity* target;		/// ’e‚Ì’Ç”ö‘ÎÛ
	};

	using BulletGeneratorPtr = std::shared_ptr<BulletGenerator>;

	///’Êí‚Ì’e”­Ë
	class NormalShot : public BulletGenerator {
	public:

		NormalShot(Entity::Entity& p, int i, Entity::Entity* t = nullptr);
		NormalShot(const NormalShot&) = delete;
		void operator=(const NormalShot&) = delete;

		void Update(float delta) override;
	};

	///•¡”‚Ì•ûŒü‚É’e‚ğ”­Ë‚·‚é
	class MultiWayShot : public BulletGenerator {
	public:

		MultiWayShot(Entity::Entity& p,int i, Entity::Entity* t = nullptr, float a = 15.f,int m = 3);
		MultiWayShot(const MultiWayShot&) = delete;
		void operator=(const MultiWayShot&) = delete;

		void Update(float delta) override;

		void CenterDirectiton(glm::vec3 v);

	private:

		float angleInterval;		///< ’e‚Æ’e‚ÌŠÔŠu()
		int maxBulletNum;
		bool isUseParentRotation;	///< e‚Ì‰ñ“]‚É‰e‹¿‚ğó‚¯‚é‚©
	};

	///‰ñ“]‚µ‚È‚ª‚ç˜A‘±‚Å’e‚ğ”­Ë‚·‚é
	class CircleShot : public BulletGenerator {
	public:

		CircleShot(Entity::Entity& parent, int i, float angleInterval = 15.f, float shotAngle = 0.f);

		void Update(float delta) override;

	private:

		float angleInterval;
		float shotAngle;

	};

}
