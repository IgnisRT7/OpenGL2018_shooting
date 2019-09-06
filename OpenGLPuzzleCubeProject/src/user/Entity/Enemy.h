/**
*	@file Enemy.h
*	@brief	�G�̐���p
*	@author	Takuya Yokoyama
*/

#pragma once

#include "../../Engine/Entity.h"
#include "../BulletGenerator.h"
#include "../EnemyMoveController.h"
#include "../../Engine/Sprite.h"

namespace Application {

	/// �G
	class Toroid : public Entity::EntityDataBase {
	public:

		Toroid(int typeID = 0, int health = 2, bool itemDrop = false) :
			enemyType(typeID), isItemDrop(itemDrop), hp(health) {} 

		virtual ~Toroid() = default;
		Toroid(const Toroid&) = delete;
		const Toroid& operator=(const Toroid&) = delete;
	
		/**
		*	�G�̏���������
		*/
		void Initialize() override;

		/**
		*	�G�̍X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void Update(float deltaTime) override;

		/**
		*	�_���[�W����
		*
		*	@param p	�_���[�W��
		*/
		void Damage(float p) override;

		/**
		*	�Փ˔��菈��
		*
		*	@param e	�Փ˂��Ă����G���e�Bt�e�B
		*/
		void CollisionEnter(Entity::Entity& e) override;

		/**
		*	�ړ��p�R���g���[���̐ݒ�
		*
		*	@param m	�ړ��p�R���g���[��
		*/
		void MoveController(MoveControllerPtr m) { moveController = m; }

		/**
		*	�ړ��p�R���g���[���[�̎擾
		*	
		*	@return �ړ��p�R���g���[���[
		*/
		MoveControllerPtr MoveController() const { return moveController; }

		/**
		*	�e�̐������ݒ�
		*
		*	@param b	�e�̐�����
		*/
		void BulletGenerator(BulletGeneratorPtr b) { bulletGenerator = b; }

		/**
		*	�e�̐�������擾
		*
		*	@return �e�̐�����
		*/
		BulletGeneratorPtr BulletGenerator() const { return bulletGenerator; }
		void DestroyByScreenOut(bool v) { isDestroyByScreenOut = v; }

		/**
		*	�^�[�Q�b�g�̐ݒ�
		*
		*	@param t	�^�[�Q�b�g�ƂȂ�G���e�B�e�B
		*/
		void Target(Entity::Entity* t);

		/**
		*	�e�̐F��ݒ肷��
		*/
		void BulletColor(glm::vec4 c) { bulletColor = c; }

		/**
		*	�ݒ肵�Ă���G���e�B�e�B�̎擾
		*
		*	@return ���p���Ă���G���e�B�e�B
		*/
		Entity::Entity* Entity() { return entity; }

	private:

		float timer = 0;	/// 
		int enemyType = 0;	/// �ړ��^�C�v
		int hp = 2;			/// �̗�

		bool isItemDrop = false;
		bool isDestroyByScreenOut = true;
		glm::vec4 bulletColor = glm::vec4(1);

		BulletGeneratorPtr bulletGenerator;	///< �e�̐����Ǘ��N���X
		MoveControllerPtr moveController;	///< �ړ��Ǘ��N���X
		
		Entity::Entity* playerEntity;
	};
	
	//�{�X�p�G
	class BossEnemy : public Entity::EntityDataBase {
	public:

		/**
		*	����������
		*/
		void Initialize() override;

		/**
		*	�X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void Update(float deltaTime) override;

		/**
		*	�_���[�W����
		*
		*	@param p	�_���[�W��
		*/
		void Damage(float) override;

		/**
		*	�Փ˔��菈��
		*
		*	@param e	�Փ˂����G���e�B�e�B
		*/
		void CollisionEnter(Entity::Entity& e) override;

		/**
		*	�^���b�g�̍X�V����
		*/
		void UpdateTurret();

		/**
		*	�ړ��p�R���g���[���̐ݒ�
		*
		*	@param m	�ݒ肷��R���g���[���[
		*/
		void MoveController(MoveControllerPtr m) { moveController = m; }

		/**
		*	�^�[�Q�b�g�̐ݒ�
		*
		*	@param t	�^�[�Q�b�g�̃G���e�B�e�B
		*/
		void Target(Entity::Entity* t);

	private:

		float timer = 0;	/// 
		int hp = 0;		/// �̗�
		const int maxHp = 1500;
		SpritePtr hpGuage;

		MoveControllerPtr moveController;
		std::vector<std::shared_ptr<Toroid> > turrets;

		Entity::Entity* playerEntity;
	};

}