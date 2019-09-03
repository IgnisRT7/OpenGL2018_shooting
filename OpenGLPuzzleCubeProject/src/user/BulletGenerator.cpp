/**
*	@file	BulletGenerator.cpp
*	@brief	�G�̒e�����V�X�e���̐���p
*	@author	Takuya Yokoyama
*/

#include "BulletGenerator.h"
#include "../Engine/GameEngine.h"
#include "../GameState.h"
#include "Entity/Bullet.h"

#include "../../Res/Audio/testProject_acf.h"
#include "../../Res/Audio/CueSheet_0.h"

#include <glm/gtc/matrix_transform.hpp>


namespace Application {

	/*
	*	����������
	*
	*	@param p		���ˌ��̃G���e�B�e�B
	*	@param groupId	�e����������O���[�vID
	*	@param t		�^�[�Q�b�g
	*/
	BulletGenerator::BulletGenerator(Entity::Entity& p,int groupId, Entity::Entity* t) :
		parent(p), target(t), timer(0),groupId(groupId) {

		initBulletSpeed = 10.f;
		timer = shotInterval* 0.2f;

		if (target) {
			CalcVelocity();
		}
	}

	/**
	*	�e�̔��ˏ���
	*
	*	tips	���̏������Ă΂��ƌ����E���x�E�e�̐F��ݒ肵�������Ŕ��˂��鎖
	*			���ł��܂�
	*/
	void BulletGenerator::ShotBullet(){

		auto b = std::make_shared<Bullet>();

		//�G���e�B�e�B�̒ǉ�����
		if (Entity::Entity* p = GameEngine::Instance().AddEntity(groupId, parent.Position(),
			"Sphere", "Res/Model/sphere.dds", b)) {

			b->Velocity(direction * initBulletSpeed);
			b->Color(color);
			
			p->Scale(glm::vec3(0.5f));
			timer = shotInterval;
		}
	}

	/**
	*	�ړ������̍Čv�Z����
	*/
	void BulletGenerator::CalcVelocity() {
		
		direction = glm::vec3(0, 0, -1);

		if (target) {
			direction = target->Position() - parent.Position();
			direction = glm::normalize(direction);
		}
	}

	/**
*	�R���X�g���N�^
*
*	@param p	�e���̃G���e�B�e�B
*	@param t	�^�[�Q�b�g�̃G���e�B�e�B
*	@param id	�e�̃O���[�vID
*/
	NormalShot::NormalShot(Entity::Entity& p, int id, Entity::Entity* t) :
		BulletGenerator(p, id, t) {

		initBulletSpeed = 10.0f;

	}

	/**
	*	�X�V����
	*
	*	@param delta	�o�ߎ���
	*/
	void NormalShot::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		timer -= delta;

		if (timer < 0) {

			if (target) {
				CalcVelocity();
			}
	
			game.PlayAudio(1, CRI_CUESHEET_0_LASER2);

			ShotBullet();
		}
	}

	/**
	*	����������
	*
	*	@param p	���ˌ��̃G���e�B�e�B
	*	@param i	��������O���[�vID
	*	@param t	�^�[�Q�b�g
	*	@param a	�e�ƒe�̊Ԋu�̊p�x
	*	@param m	�����ɔ��˂ł���e��
	*/
	MultiWayShot::MultiWayShot(Entity::Entity& p,int i, Entity::Entity* t, float a, int m) :
		BulletGenerator(p, i, t),angleInterval(a),maxBulletNum(m) {

		initBulletSpeed = 10.f;
	}

	/**
	*	�X�V����
	*/
	void MultiWayShot::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		timer -= delta;

		if (timer < 0) {
			game.PlayAudio(1, CRI_CUESHEET_0_LASER2);

			if (target) {
				CalcVelocity();
			}

			glm::vec3 centerDir = direction;
			
			for (int i = 0; i < maxBulletNum; i++) {

				int settingIndex = (i - (int)(maxBulletNum / 2));

				//�e�̊p�x�E���x�ݒ菈��
				glm::quat rot = glm::angleAxis(glm::radians(angleInterval * settingIndex), glm::vec3(0, 1, 0));
				direction = rot * centerDir;

				ShotBullet();
			}
		}

	}

	/**
	*	���ˊp��ݒ肵�܂�
	*/
	void MultiWayShot::CenterDirectiton(glm::vec3 v){

		direction =v;
	}

	/**
	*	����������
	*
	*	@param p	�e�̃G���e�B�e�B
	*	@param i	��������O���[�vID
	*	@param angInterval	���˂���p�x�̊Ԋu
	*	@param initAngle	�ŏ��ɔ��˂���e�̊p�x
	*/
	CircleShot::CircleShot(Entity::Entity& p, int i,float angInterval,float initAngle) :
		BulletGenerator(p,i),angleInterval(angInterval),shotAngle(initAngle){

		shotInterval = 0.2f;
		timer = 0;
	}

	void CircleShot::Update(float delta){

		GameEngine& game = GameEngine::Instance();

		timer += delta;

		if ( timer > shotInterval) {
			///�e�̔��ˏ���

			if (target) {
				CalcVelocity();
			}

			game.PlayAudio(1, CRI_CUESHEET_0_LASER2);

			GameEngine& game = GameEngine::Instance();

			std::shared_ptr<Bullet> b = std::make_shared<Bullet>();
			Entity::Entity* p = game.AddEntity(groupId, parent.Position(),
				"Sphere", "Res/Model/sphere.dds", b);

			if (shotAngle > glm::pi<float>() * 2.0f) {
				shotAngle -= glm::pi<float>() * 2.0f;
			}
			glm::quat rot = glm::angleAxis(shotAngle, glm::vec3(0,1,0));
			glm::vec3 newVel = rot * glm::vec3(1, 0, 0);

			b->Velocity(newVel * initBulletSpeed);
			p->Scale(glm::vec3(0.5));
			p->Color(color);

			shotAngle += glm::radians(angleInterval);
			timer = 0;
		}

	}



}