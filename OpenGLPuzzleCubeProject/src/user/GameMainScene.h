/**
*	@file	GameMainScene.h
*	@brief	�Q�[���̃��C���V�[������p
*	@author	Takuya Yokoyama
*/
#pragma once
#include "../Engine/Entity.h"
#include "Entity/Player.h"
#include "../Engine/Scene.h"
#include "EnemyLaunchController.h"
#include "../Engine/Sprite.h"
#include "Entity/LandScape.h"

namespace Application {

	///���C���Q�[�����
	class MainGame : public Scene{
	public:

		MainGame() :Scene("MainGame") {}
		~MainGame() = default;
		MainGame(const MainGame&) = delete;
		const MainGame& operator=(const MainGame&) = delete;

		/**
		*	����������
		*
		*	@return true	����������
		*	@return false	���������s
		*/
		bool Initialize() override;

		/**
		*	�X�V����
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void Update(float deltaTime) override;

		/**
		*	�I������
		*/
		void Finalize() override;

		/**
		*	�J�n����
		*/
		void Play() override;

		/**
		*	��~����
		*/
		void Stop() override ;

		/**
		*	��\������
		*/
		void Hide() override {};

		/**
		*	�v���C���[�̃^�C�v��ݒ肷��
		*
		*	@param t	�^�C�v
		*/	
		void SelectPlayerType(int t) { playerType = t; }

		/**
		*	�X�N���[���̕`�����ݒ肷��
		*
		*	@param deltaTime	�o�ߎ���
		*/
		void DrawScreenInfo(float deltaTime);

		/**
		*	�X�e�[�W�̃��[�h����
		*/
		void StageLoad();

		/**
		*	�X�e�[�W�N���A��̏���
		*
		*	@param nextStageTimer	���̃X�e�[�W�̊J�n�^�C�}
		*/
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
			{-0.75,-0.85f}, {3,3}, {0.8f,0.8f,0,1.0f },""
		};
		
		

		std::shared_ptr<Player> playerData;	///< �v���C���[�̃G���e�B�e�B�f�[�^
		std::shared_ptr<Sprite> hpguage;	///< �f�o�b�O�pHP�Q�[�W�p�X�v���C�g

		Application::EnemyLaunchControllerPtr launchController;	
	};

}