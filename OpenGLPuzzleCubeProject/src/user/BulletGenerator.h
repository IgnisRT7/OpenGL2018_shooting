/**
*	@file BulletGenerator.h
*/
#pragma once
#include "../Engine/Entity.h"


namespace GameState {

	///�e�̐����V�X�e���̃x�[�X�N���X
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

		float timer;					///< ���ˊԊu����p�^�C�}�[
		float shotInterval = 2;			///< �e�̔��ˊԊu
		float initBulletSpeed = 1;		///< �e�̑��x
		int groupId = -1;				///< �e��ID
		glm::vec4 color = glm::vec4(1);	///< ���˂���e�̐F
		glm::vec3 direction;			///< �e�𔭎˂��������

		Entity::Entity& parent;		/// �e�̔��ˌ�
		Entity::Entity* target;		/// �e�̒ǔ��Ώ�
	};

	using BulletGeneratorPtr = std::shared_ptr<BulletGenerator>;

	///�ʏ�̒e����
	class NormalShot : public BulletGenerator {
	public:

		NormalShot(Entity::Entity& p, int i, Entity::Entity* t = nullptr);
		NormalShot(const NormalShot&) = delete;
		void operator=(const NormalShot&) = delete;

		void Update(float delta) override;
	};

	///�����̕����ɒe�𔭎˂���
	class MultiWayShot : public BulletGenerator {
	public:

		MultiWayShot(Entity::Entity& p,int i, Entity::Entity* t = nullptr, float a = 15.f,int m = 3);
		MultiWayShot(const MultiWayShot&) = delete;
		void operator=(const MultiWayShot&) = delete;

		void Update(float delta) override;

		void CenterDirectiton(glm::vec3 v);

	private:

		float angleInterval;		///< �e�ƒe�̊Ԋu()
		int maxBulletNum;
		bool isUseParentRotation;	///< �e�̉�]�ɉe�����󂯂邩
	};

	///��]���Ȃ���A���Œe�𔭎˂���
	class CircleShot : public BulletGenerator {
	public:

		CircleShot(Entity::Entity& parent, int i, float angleInterval = 15.f, float shotAngle = 0.f);

		void Update(float delta) override;

	private:

		float angleInterval;
		float shotAngle;

	};

}
