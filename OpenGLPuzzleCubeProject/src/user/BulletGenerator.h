/**
*	@file BulletGenerator.h
*/
#pragma once
#include "../Engine/Entity.h"


namespace GameState {

	///弾の生成システムのベースクラス
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

		float timer;					///< 発射間隔制御用タイマー
		float shotInterval = 2;			///< 弾の発射間隔
		float initBulletSpeed = 1;		///< 弾の速度
		int groupId = -1;				///< 弾のID
		glm::vec4 color = glm::vec4(1);	///< 発射する弾の色
		glm::vec3 direction;			///< 弾を発射させる向き

		Entity::Entity& parent;		/// 弾の発射元
		Entity::Entity* target;		/// 弾の追尾対象
	};

	using BulletGeneratorPtr = std::shared_ptr<BulletGenerator>;

	///通常の弾発射
	class NormalShot : public BulletGenerator {
	public:

		NormalShot(Entity::Entity& p, int i, Entity::Entity* t = nullptr);
		NormalShot(const NormalShot&) = delete;
		void operator=(const NormalShot&) = delete;

		void Update(float delta) override;
	};

	///複数の方向に弾を発射する
	class MultiWayShot : public BulletGenerator {
	public:

		MultiWayShot(Entity::Entity& p,int i, Entity::Entity* t = nullptr, float a = 15.f,int m = 3);
		MultiWayShot(const MultiWayShot&) = delete;
		void operator=(const MultiWayShot&) = delete;

		void Update(float delta) override;

		void CenterDirectiton(glm::vec3 v);

	private:

		float angleInterval;		///< 弾と弾の間隔()
		int maxBulletNum;
		bool isUseParentRotation;	///< 親の回転に影響を受けるか
	};

	///回転しながら連続で弾を発射する
	class CircleShot : public BulletGenerator {
	public:

		CircleShot(Entity::Entity& parent, int i, float angleInterval = 15.f, float shotAngle = 0.f);

		void Update(float delta) override;

	private:

		float angleInterval;
		float shotAngle;

	};

}
