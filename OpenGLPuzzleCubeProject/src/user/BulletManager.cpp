/**
*	@file BulletManager.cpp
*/
#include "BulletManager.h"
#include "../GameEngine.h"
#include "../GameState.h"
#include "Entity/Bullet.h"

#include "../../Res/Audio/testProject_acf.h"
#include "../../Res/Audio/CueSheet_0.h"

#include <glm/gtc/matrix_transform.hpp>


namespace GameState {

	BulletManager::BulletManager(Entity::Entity& p,int groupId, Entity::Entity* t) :
		parent(p), target(t), timer(0),groupId(groupId) {

		initBulletSpeed = 10.f;
		timer = shotInterval* 0.2f;
	}

	/**
	*	�X�V����
	*
	*	@param delta	�o�ߎ���
	*/
	void BulletManager::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		timer -= delta;

		if (timer < 0) {
			//game.PlayAudio(1, CRI_CUESHEET_0_ENEMYSHOT);

			glm::vec3 targetVel = target ? target->Position() - parent.Position() : glm::vec3(0, 0, -1);
			targetVel = glm::normalize(targetVel) * initBulletSpeed;

			if (Entity::Entity* p = game.AddEntity(groupId, parent.Position(),
				"Sphere", "Res/Model/sphere.dds", std::make_shared<Bullet>())) {

				p->Velocity(targetVel);
				p->CastStencil(true);
				p->StencilColor(glm::vec4(1, 0, 1, 1));
				p->Scale(glm::vec3(0.5f));
				timer = shotInterval;

			}
		}
	}

	/**
	*	�ړ������̍Čv�Z����
	*/
	void BulletManager::CalcVelocity() {
		
		glm::vec3 dir = glm::vec3(0, 0, -1);

		if (target) {
			dir = target->Position() - parent.Position();
			dir = glm::normalize(dir);
		}

		dir *= initBulletSpeed;
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
		BulletManager(p, i, t),angleInterval(a),maxBulletNum(m) {

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
				glm::quat rot = glm::angleAxis(angleInterval * (i - (int)(maxBulletNum / 2)), glm::vec3(0, 1, 0));
				glm::vec3 vel = rot * centerDir;
				vel *= initBulletSpeed;

				//�G���e�B�e�B�̒ǉ�����
				if (Entity::Entity* p = game.AddEntity(groupId, parent.Position(),
					"Sphere", "Res/Model/sphere.dds", std::make_shared<Bullet>())) {

					p->Velocity(vel);
					p->Color(glm::vec4(1, 1, 1, 1));
					p->Scale(glm::vec3(0.5f));
					timer = shotInterval;
				}
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
		BulletManager(p,i),angleInterval(angInterval),shotAngle(initAngle){

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

			//game.PlayAudio()
			shotAngle += angleInterval;
			timer = 0;
		}

	}


}