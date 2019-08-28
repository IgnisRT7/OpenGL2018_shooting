/**
*	@file GameMainScene.h
*/
#pragma once
#include "../Engine/Entity.h"
#include "Entity/Player.h"
#include "../Engine/Scene.h"
#include "EnemyLaunchController.h"
#include "../Engine/Sprite.h"

namespace GameState {


	///�w�i(�X���C�h)
	class Landscape : public Entity::EntityDataBase {
	public:

		//Landscape() = default;
		Landscape(bool l = false);

		void Initialize() override;
		void Update(float delta) override;
	private:

		float timer = 0;
		glm::vec3 start;
		bool isLoop = false;
	};

	///�w�i�̍X�V(��])
	class SpaceSphereMain : public Entity::EntityDataBase {

		void Initialize() override;

		void Update(float delta) override;
	};
	


	///���C���Q�[�����
	class MainGame : public Scene{
	public:

		MainGame() :Scene("MainGame") {}

		bool Initialize() override;
		void Update(float d) override;
		void Finalize() override;
		void Play() override;
		void Stop() override ;
		void Hide() override {};

		void SelectPlayerType(int t) { playerType = t; }
		void DrawScreenInfo(float dt);
		void StageLoad();
		void StageClear(float nextStageTimer);

	private:

		float stageNameFadeTimer = 0;	///< �X�e�[�W����\�����邽�߂̃^�C�}�[
		float stageTimer = -1;			///< �X�e�[�W���J�ڂ���܂ł̃^�C�}�[
		float sceneTimer=0;				///< �V�[�����J�ڂ���܂ł̃^�C�}�[
		
		bool isStageClear = false;		///< �X�e�[�W���N���A�������ǂ����̃t���O

		int stageNo = 0;				///< ���݂̃X�e�[�W�ԍ�
		int playerType = 0;				///< �v���C���[�̃^�C�v(�V�[������̈����p���p)

		struct FontDrawInfo {
			glm::vec2 pos;
			glm::vec2 size;
			glm::vec4 color;
			std::string str;

		};

		FontDrawInfo stageName;
		FontDrawInfo stageClearString = {
			{0,0},{4,4},{0,0.3,1.0,1.0},"STAGE CLEAR!!"
		};
		FontDrawInfo scoreString = {
			{-0.95,-0.85f}, {3,3}, {1.0,0.5,0,1.0},""
		};
		

		std::shared_ptr<Player> playerData;	///< �v���C���[�̃G���e�B�e�B�f�[�^
		std::shared_ptr<Sprite> hpguage;	///< �f�o�b�O�pHP�Q�[�W�p�X�v���C�g

		EnemyLaunchControllerPtr launchController;	
	};

}