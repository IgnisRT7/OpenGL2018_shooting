/**
*	@file BulletGenerator.h
*	@brief	�G�̒e�����V�X�e���̐���p
*	@author	Takuya Yokoyama
*/
#pragma once
#include "../Engine/Entity.h"


namespace Application {

	///�e�̐����V�X�e���̃x�[�X�N���X
	class BulletGenerator {
	public:

		BulletGenerator() = default;

		/**
		*	�R���X�g���N�^
		*
		*	@param parent	���t����G���e�B�e�B
		*	@param groupId	�K�p����O���[�vID
		*	@param target	�^�[�Q�b�g�̃G���e�B�e�B
		*/
		BulletGenerator(Entity::Entity& parent,int groupId, Entity::Entity* target = nullptr);	

		virtual ~BulletGenerator() = default;
		const BulletGenerator& operator=(const BulletGenerator&) = delete;

		/**
		*	�X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		virtual void Update(float deltaTime) = 0;

		/**
		*	�e�̔��ˏ���
		*/
		virtual void ShotBullet();

		/**
		*	�ړ��ʂ̌v�Z����
		*/
		virtual void CalcVelocity();

		/**
		*	�^�[�Q�b�g�̐ݒ�
		*
		*	@param t	�^�[�Q�b�g�Ƃ��Đݒ肷��G���e�B�e�B
		*/
		void Target(Entity::Entity* t) { target = t; CalcVelocity(); }

		/**
		*	�O���[�vID�̐ݒ�
		*/
		void GroupId(int i) { groupId = i; }

		/**
		*	���ˊԊu�̐ݒ�
		*
		*	@param i	�K�p���锭�ˊԊu
		*/
		void ShotInterval(float i) { shotInterval = i; }

		/**
		*	���ˊԊu���̎擾
		*
		*	@return ���ˊԊu
		*/
		float ShotInterval() const { return shotInterval; }

		/**
		*	�e���̐ݒ�
		*	
		*	@param s	�K�p����e��
		*/
		void BulletSpeed(float s) { initBulletSpeed = s; }

		/**
		*	�e���̎擾
		*	
		*	@return �e��
		*/
		float BulletSpeed() const { return initBulletSpeed; }

		/**
		*	�e�̐F�̐ݒ�
		*
		*	@param c	�K�p����F
		*/
		void Color(glm::vec4 c) { color = c; }

		/**
		*	�e�̉�]����ݒ肷��
		*
		*	@param r	��]���
		*/
		void Rotation(glm::quat r) { direction = r * glm::vec3(-1, 0, 0); }

	protected:

		float timer;					///< ���ˊԊu����p�^�C�}�[
		float shotInterval = 2;			///< �e�̔��ˊԊu
		float initBulletSpeed = 1;		///< �e�̑��x
		int groupId = -1;				///< �e��ID
		glm::vec4 color = glm::vec4(0.3, 0.3, 1, 1);	///< ���˂���e�̐F
		glm::vec3 direction;			///< �e�𔭎˂��������
		glm::quat rotation;				///< �e�𔭎˂�����p�x

		Entity::Entity& parent;		/// �e�̔��ˌ�
		Entity::Entity* target;		/// �e�̒ǔ��Ώ�
	};

	using BulletGeneratorPtr = std::shared_ptr<BulletGenerator>;

	///�ʏ�̒e����
	class NormalShot : public BulletGenerator {
	public:

		NormalShot() = default;

		/**
		*	�R���X�g���N�^
		*
		*	@param parent	���t����G���e�B�e�B
		*	@param interval	���ˊԊu
		*	@param target	�^�[�Q�b�g�̃G���e�B�e�B
		*/
		NormalShot(Entity::Entity& p, int i, Entity::Entity* t = nullptr);
		NormalShot(const NormalShot&) = delete;
		void operator=(const NormalShot&) = delete;

		/**
		*	�X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void Update(float deltaTime) override;
	};

	///�����̕����ɒe�𔭎˂���
	class MultiWayShot : public BulletGenerator {
	public:

		MultiWayShot() = default;

		/**
		*	�R���X�g���N�^
		*	
		*	@param p	���t����G���e�B�e�B
		*	@param i	���ˊԊu
		*	@param t	�^�[�Q�b�g�̃G���e�B�e�B
		*	@param a	�e���m�̊Ԋu
		*	@param m	�������˒e��
		*/
		MultiWayShot(Entity::Entity& parent,int i, Entity::Entity* t = nullptr, float a = 15.f,int m = 3);
		MultiWayShot(const MultiWayShot&) = delete;
		void operator=(const MultiWayShot&) = delete;

		/**
		*	�X�V����
		*
		*	@param deltaTime	�X�V����
		*/
		void Update(float deltaTime) override;

		/**
		*	���S�ʒu�̐ݒ菈��
		*
		*	@param v	���S�̕����x�N�g��
		*/
		void CenterDirectiton(glm::vec3 v);

		/**
		*	�ő�e���̐ݒ�
		*
		*	@param m	�K�p����ő�e��
		*/
		void MaxBulletNum(int m) { maxBulletNum = m; }

		/**
		*	�p�x�Ԋu�̐ݒ�
		*
		*	@param a	�K�p����p�x�Ԋu 
		*/
		void AngleInterval(float r) { angleInterval = r; }

	private:

		float angleInterval;		/// �e�ƒe�̊Ԋu()
		int maxBulletNum;			/// �������˂ł���ő�e��
		bool isUseParentRotation;	/// �e�̉�]�ɉe�����󂯂邩
	};

	///��]���Ȃ���A���Œe�𔭎˂���
	class CircleShot : public BulletGenerator {
	public:

		CircleShot() = default;


		CircleShot(Entity::Entity& parent , int interval, float a = 15.f, float shotAngle = 0.f);
		~CircleShot() = default;
		const CircleShot& operator=(const CircleShot&) = delete;;

		/**
		*	�X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void Update(float delta) override;

	private:

		float angleInterval;	/// �e���m�̊p�x�̊Ԋu
		float shotAngle;		

	};

}
