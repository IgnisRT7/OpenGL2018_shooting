/**
*	@file GameMainScene.h
*/
#pragma once
#include "../Entity.h"


namespace GameState {

	///�w�i(�X���C�h)
	class Landscape : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(double delta) override;
	};

	///�w�i�̍X�V(��])
	class SpaceSphereMain : public Entity::EntityDataBase {

		void Initialize() override;

		void Update(double delta) override;
	};
	
	/**
	*	�����̍X�V
	*/
	class Blast : public Entity::EntityDataBase{
	public:

		void Initialize() override;

		void Update(double delta) override;

	private:

		double timer = 0;
		bool isInitialized = false;
	};

	/**
	*	���@�̒e�̍X�V
	*/
	class PlayerShot : public Entity::EntityDataBase{
	public:

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& e) override;

	private:

		//glm::vec3 movVec = { 0,0,80 };
	};

	/// �A�C�e��
	class Item : public Entity::EntityDataBase {
	public:

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& e) override ;

		int ItemType() { return itemType; }

	private:

		int itemType = 1;
	};


	/// �v���C���[
	class Player : public Entity::EntityDataBase{
	public:

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& entity) override;

		void damage();

		void StartMove(double delta);

	private:

		bool isStartingMove = true;		/// �X�^�[�g����̈ړ�����
		float startMovValue = 20;		/// �X�^�[�g����̈ړ���
		float moveSpeed = 5.0f;

		float timer = 0;
		float damageTimer;

		glm::vec3 moveBox[2] =
		{ {-25, -120, -1},{25, 100, 80} };		/// �v���C���[�̉���

		double shotInterval = 0;	/// ���˂����܂ł̃N�[���^�C��
		int multiShotNum = 1;			/// ��x�ɔ��˂ł���e��


	};

	/// �G
	class Toroid : public Entity::EntityDataBase{
	public:

		Toroid(int typeID = 0) :enemyType(typeID) {}

		void Initialize() override;

		void Update(double delta) override;

		void CollisionEnter(Entity::Entity& e) override;

	private:

		float timer = 0;
		int enemyType = 0;
		int hp = 2;

	};

	/// �G�X�|�i�[
	class EnemyLaunchType : public Entity::EntityDataBase{
	public:

		void Initialize() override {}

		void Update(double delta) override;

		float spawnInterval = 0.5;	//�X�|�[������Ԋu
		float spawnMax = 5;			//�X�|�[����
		float time;					//�o�ߎ���
		int launchIndex = -1;		//�o�����Ă���G�̐�
		int enemyType = -1;			//�o������G�̎��
	};



	///���C���Q�[�����
	class MainGame {
	public:

		MainGame();
		void operator()(double delta);

	private:

		//UpdatePlayer playerData;

		double interval = 0;
		int stageNo = 0;
		double stageTimer = -1;

	};

}