/**
*	@file BulletGenerator.h
*/
#pragma once
#include "../Engine/Entity.h"


namespace GameState {

	///íeÇÃê∂ê¨ÉVÉXÉeÉÄÇÃÉxÅ[ÉXÉNÉâÉX
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
		void Rotation(glm::quat r) { direction = r * glm::vec3(-1, 0, 0); }

	protected:

		float timer;					///< î≠éÀä‘äuêßå‰ópÉ^ÉCÉ}Å[
		float shotInterval = 2;			///< íeÇÃî≠éÀä‘äu
		float initBulletSpeed = 1;		///< íeÇÃë¨ìx
		int groupId = -1;				///< íeÇÃID
		glm::vec4 color = glm::vec4(0.3, 0.3, 1, 1);	///< î≠éÀÇ∑ÇÈíeÇÃêF
		glm::vec3 direction;			///< íeÇî≠éÀÇ≥ÇπÇÈå¸Ç´
		glm::quat rotation;				///< íeÇî≠éÀÇ≥ÇπÇÈäpìx

		Entity::Entity& parent;		/// íeÇÃî≠éÀå≥
		Entity::Entity* target;		/// íeÇÃí«îˆëŒè€
	};

	using BulletGeneratorPtr = std::shared_ptr<BulletGenerator>;

	///í èÌÇÃíeî≠éÀ
	class NormalShot : public BulletGenerator {
	public:

		NormalShot(Entity::Entity& p, int i, Entity::Entity* t = nullptr);
		NormalShot(const NormalShot&) = delete;
		void operator=(const NormalShot&) = delete;

		void Update(float delta) override;
	};

	///ï°êîÇÃï˚å¸Ç…íeÇî≠éÀÇ∑ÇÈ
	class MultiWayShot : public BulletGenerator {
	public:

		MultiWayShot(Entity::Entity& p,int i, Entity::Entity* t = nullptr, float a = 15.f,int m = 3);
		MultiWayShot(const MultiWayShot&) = delete;
		void operator=(const MultiWayShot&) = delete;

		void Update(float delta) override;

		void CenterDirectiton(glm::vec3 v);
		void MaxBulletNum(int m) { maxBulletNum = m; }

	private:

		float angleInterval;		///< íeÇ∆íeÇÃä‘äu()
		int maxBulletNum;
		bool isUseParentRotation;	///< êeÇÃâÒì]Ç…âeãøÇéÛÇØÇÈÇ©
	};

	///âÒì]ÇµÇ»Ç™ÇÁòAë±Ç≈íeÇî≠éÀÇ∑ÇÈ
	class CircleShot : public BulletGenerator {
	public:

		CircleShot(Entity::Entity& parent, int i, float angleInterval = 15.f, float shotAngle = 0.f);

		void Update(float delta) override;

	private:

		float angleInterval;
		float shotAngle;

	};

}
