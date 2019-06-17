/**
*	@file BulletGenerator.cpp
*/
#include "BulletGenerator.h"
#include "../GameEngine.h"
#include "../GameState.h"
#include "Entity/Bullet.h"

#include "../../Res/Audio/testProject_acf.h"
#include "../../Res/Audio/CueSheet_0.h"

#include <glm/gtc/matrix_transform.hpp>


namespace GameState {

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

		b->Velocity(direction * initBulletSpeed);
		b->Color(color);

		//�G���e�B�e�B�̒ǉ�����
		if (Entity::Entity* p = GameEngine::Instance().AddEntity(groupId, parent.Position(),
			"Sphere", "Res/Model/sphere.dds", std::make_shared<Bullet>())) {

			direction = glm::normalize(direction);

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
			//game.PlayAudio(1, CRI_CUESHEET_0_ENEMYSHOT);

			glm::vec3 centerDir = target ? target->Position() - parent.Position() : glm::vec3(0, 0, -1);
			centerDir = glm::normalize(centerDir);
			
			for (int i = 0; i < maxBulletNum; i++) {

				//�e�̊p�x�E���x�ݒ菈��
				glm::quat rot = glm::angleAxis(glm::radians(angleInterval * (i - (int)(maxBulletNum / 2))), glm::vec3(0, 1, 0));
				direction = rot * centerDir;

				ShotBullet();
			}
		}

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

		timer += delta;

		if ( timer > shotInterval) {
			///�e�̔��ˏ���

			GameEngine& game = GameEngine::Instance();

			std::shared_ptr<Bullet> b = std::make_shared<Bullet>();
			Entity::Entity* p = game.AddEntity(groupId, parent.Position(),
				"Sphere", "Res/Model/sphere.dds", b);

			if (shotAngle > glm::pi<float>() * 2.0f) {
				shotAngle -= glm::pi<float>() * 2.0f;
			}
			glm::quat rot = glm::angleAxis(shotAngle, glm::vec3(0,1,0));
			glm::vec3 newVel = rot * glm::vec3(0, 0, 1);

			p->Velocity(newVel * initBulletSpeed);
			p->Scale(glm::vec3(0.5));
			p->Color(color);

			//game.PlayAudio()
			shotAngle += angleInterval;
			timer = 0;
		}

	}

	/**
	*	�R���X�g���N�^
	*
	*	@param p	�e���̃G���e�B�e�B
	*	@param t	�^�[�Q�b�g�̃G���e�B�e�B
	*	@param id	�e�̃O���[�vID
	*/
	NormalShot::NormalShot(Entity::Entity& p,int id, Entity::Entity* t) :
	BulletGenerator(p,id,t){

	}

	void NormalShot::Update(float delta){

		GameEngine& game = GameEngine::Instance();

		timer -= delta;

		if (timer < 0) {

			if (target) {
				CalcVelocity();
			}


			//game.PlayAudio(1, CRI_CUESHEET_0_ENEMYSHOT);

			ShotBullet();

			/*glm::vec3 targetVel = target ? target->Position() - parent.Position() : glm::vec3(0, 0, -1);
			targetVel = glm::normalize(targetVel) * initBulletSpeed;

			if (Entity::Entity* p = game.AddEntity(groupId, parent.Position(),
				"Sphere", "Res/Model/sphere.dds", std::make_shared<Bullet>())) {

				p->Velocity(targetVel);
				p->CastStencil(true);
				p->StencilColor(glm::vec4(1, 0, 1, 1));
				p->Scale(glm::vec3(0.5f));
				p->Color(color);
				timer = shotInterval;

			}*/
		}
	}

}