/**
*	@file	GameMainScene.cpp
*	@brief	�Q�[���̃��C���V�[���̐���p
*	@author	Takuya Yokoyama
*/

#include "GameMainScene.h"
#include "../GameState.h"
#include "../../Res/Audio/CueSheet_0.h"
#include "../../Res/Audio/testProject_acf.h"
#include "../Engine/GameEngine.h"
#include "../../Res/Resource.h"

#include "Entity/Enemy.h"

#include "GameEndScene.h"
#include "TitleScene.h"

namespace Application {

	/**
	*	����������
	*/
	bool MainGame::Initialize() {

		GameEngine& game = GameEngine::Instance();

		//���f���f�[�^�̃��[�h
		const char* meshPassList[] = {
			Resource::fbx_player,
			Resource::fbx_playerType2,
			Resource::fbx_blast,
			Resource::fbx_toroid,
			Resource::fbx_motherShip,
			Resource::fbx_itembox,
			Resource::fbx_spaceSphere,
			Resource::fbx_landscape,
			Resource::fbx_city,
		};

		for (const char* meshPass : meshPassList) {
			if (!game.LoadMeshFromFile(meshPass)) {
				std::cout << "file: " << meshPass << "�̓ǂݍ��݂Ɏ��s" << std::endl;
			}
		}

		//�e�N�X�`���f�[�^�̃��[�h
		const char* texturePassList[] = {
			Resource::tex_player,
			Resource::tex_toroid,
			Resource::tex_toroid_normal,
			Resource::tex_itemboxSpeed,
			Resource::tex_itemboxBullet,
			Resource::tex_bullet,
			Resource::tex_bg02,
			Resource::tex_bg02_normal,
			Resource::tex_city01,
			Resource::tex_city01_normal,
			Resource::tex_spaceSphere,
		};

		for (const char* texturePass : texturePassList) {
			if (!game.LoadTextureFromFile(texturePass)) {
				std::cout << "file: " << texturePass << "�̓ǂݍ��݂Ɏ��s" << std::endl;
			}
		}

		return true;
	}

	/**
	*	�I������
	*/
	void MainGame::Finalize() {
	}

	/**
	*	�Đ�����
	*/
	void MainGame::Play() {

		GameEngine& game = GameEngine::Instance();

		game.UserVariable("score") = 0;

		///�Փ˔���p�n���h���̒�`
		game.CollisionHandler(GameState::EntityGroupId_PlayerShot, GameState::EntityGroupId_Enemy);
		game.CollisionHandler(GameState::EntityGroupId_EnemyShot, GameState::EntityGroupId_Player);
		game.CollisionHandler(GameState::EntityGroupId_Player, GameState::EntityGroupId_Enemy);
		game.CollisionHandler(GameState::EntityGroupId_Player, GameState::EntityGroupId_Item);

		/// ���C�g�E�P�x�̐ݒ�
		game.AmbientLight(glm::vec4(0.05f, 0.1f, 0.2f, 1));
		game.Light(0, { glm::vec4(1,100,1,1),glm::vec4(12000,12000,12000,1) });

		playerData = std::make_shared<Player>();

		//game.MainCamera(std::make_shared<CameraDebugComponent>());
		game.MainCamera(std::make_shared<CameraComponent>());
		game.MainCamera()->LookAt(glm::vec3(0, 50, -1), glm::vec3(0, 0, 0));

		game.TimeScale(1);

		StageLoad();

		sceneTimer = 0;

		game.EnableShadow(true);
		
		
	}

	/**
	*	��~����
	*/
	void MainGame::Stop() {

	}

	/**
	*	��ʂɕ\��������̕`�揈��
	*
	*	@param deltaTime	�o�ߎ���
	*/
	void MainGame::DrawScreenInfo(float deltaTime) {

		GameEngine& game = GameEngine::Instance();

		//�X�e�[�W���̕\���X�V
		if (stageNameFadeTimer > 0 ) {
			static const float fadeTime = 2.0f;
			stageNameFadeTimer -= deltaTime;
			stageName.color = glm::vec4(stageName.color.r, stageName.color.g, stageName.color.b, 
				stageNameFadeTimer / fadeTime);
		}
		char str[16];
		snprintf(str, 16, "STAGE : %d", stageNo);
		game.FontScale(stageName.size);
		game.FontColor(stageName.color);
		game.AddString(glm::vec3(0), stageName.str.c_str(), true);
		game.AddString(glm::vec2(0.f, 0.15f), str, true);

		//�Q�[���N���A���̕����`�揈��
		if (isStageClear) {
			game.FontScale(stageClearString.size);
			game.FontColor(stageClearString.color);
			game.AddString(stageClearString.pos, stageClearString.str.c_str(), true);
		}

		if (sceneTimer <= 0) {
			//�X�R�A�\������

			char str[16];
			snprintf(str, 16, "SCORE :%08.0f", game.UserVariable("score"));
			scoreString.str = std::string(str);
			game.FontScale(scoreString.size);
			game.FontColor(scoreString.color);
			game.AddString(scoreString.pos, scoreString.str.c_str());

			game.FontColor(glm::vec4(0, 0.63f, 0.9f, 1.0f));
			snprintf(str, 16, "P: %01.0f", glm::max(0.0, static_cast<double>(playerData->RemainingPlayer())));
			game.AddString(glm::vec2(-0.95f, -0.85f), str);

			game.FontColor(glm::vec4(1));
			game.FontScale(glm::vec2(2));
			snprintf(str, 16, "%02.0f fps", game.FPS());
			game.AddString(glm::vec2(0.8f, -0.85f), str);
		}
	}

	/**
	*	�X�e�[�W�̓ǂݍ��ݏ���
	*/
	void MainGame::StageLoad(){

		GameEngine& game = GameEngine::Instance();
		
		game.StopAllAudio();
		game.RemoveAllEntity();

		++stageNo; stageNo = 3;
		
		launchController = std::make_shared<EnemyLaunchController>();
		launchController->Init(stageNo);

		auto playerEntity = game.AddEntity(GameState::EntityGroupId_Player, glm::vec3(0, 0, 0),
			"Aircraft", Resource::tex_player, playerData);

		playerData->StartMoveSet();

		stageTimer = 0;
		stageNameFadeTimer = 5.0f;
		isStageClear = false;
		stageName.size = glm::vec2(4);
		stageName.color = glm::vec4(1, 0, 0, 1);

		//�X�e�[�W���Ƃ̃��[�h����
		switch (stageNo) {
		case 1: {
			game.PlayAudio(0, CRI_CUESHEET_0_MAINSCENE);

			game.KeyValue(0.16f);

			//�V���h�E�̐ݒ�
			GameEngine::ShadowParameter shadowParam;
			shadowParam.lightPos = glm::vec3(20, 50, 50);
			shadowParam.lightDir = glm::normalize(glm::vec3(-25, -150, 25));
			shadowParam.lightUp = glm::vec3(0, 1, 0);
			shadowParam.near = 10;
			shadowParam.far = 200;
			shadowParam.range = glm::vec2(500, 500);
			game.Shadow(shadowParam);

			//�X�e�[�W���̐ݒ�
			stageName.pos = glm::vec2(-1,0);
			stageName.color = glm::vec4(1, 0, 0, 1);
			stageName.str = "The beginning of the forest";
				
			//�w�i�G���e�B�e�B�̍쐬����
			for (int z = 0; z < 5; ++z) {
				const float offsetZ = static_cast<float>(z * 40 * 5);

				for (int x = 0; x < 5; ++x) {
					const float offsetX = static_cast<float>(x * 40 - 80) * 5.0f;
					game.AddEntity(GameState::EntityGroupId_Background, glm::vec3(offsetX, -100.0, offsetZ),
						"Landscape01", Resource::tex_bg02, Resource::tex_bg02_normal, std::make_shared<Landscape>());
				}
			}
			break;
		}
		case 2: {
			game.PlayAudio(0, CRI_CUESHEET_0_MAIN2SCENE);

			game.KeyValue(0.24f);

			//�V���h�E�̐ݒ�
			GameEngine::ShadowParameter shadowParam;
			shadowParam.lightPos = glm::vec3(20, 50, 50);
			shadowParam.lightDir = glm::normalize(glm::vec3(-25, -50, 25));
			shadowParam.lightUp = glm::vec3(0, 1, 0);
			shadowParam.near = 10;
			shadowParam.far = 200;
			shadowParam.range = glm::vec2(500, 500);
			game.Shadow(shadowParam);

			//�X�e�[�W���̐ݒ�
			stageName.pos = glm::vec2(-1, 0);
			stageName.str = "The town that got stuck";

			//�w�i�G���e�B�e�B�̍쐬����
			for (int z = 0; z < 5; ++z) {
				const float offsetZ = static_cast<float>(z * 40);

				for (int x = 0; x < 5; ++x) {
					const float offsetX = static_cast<float>(x * 40 - 80);
					game.AddEntity(GameState::EntityGroupId_Background, glm::vec3(offsetX, -10, offsetZ),
						"City01", Resource::tex_city01, Resource::tex_city01_normal, std::make_shared<Landscape>(true));
					game.AddEntity(GameState::EntityGroupId_Background, glm::vec3(offsetX, -10, offsetZ),
						"City01.Shadow", Resource::tex_city01, Resource::tex_city01_normal, std::make_shared<Landscape>(true));
				}
			}
			break;
			
		}
		case 3: {
			game.MainCamera()->LookAt(glm::vec3(0, 50, -10), glm::vec3(0, 0, 0));
			game.PlayAudio(0, CRI_CUESHEET_0_BOSSSCENE);

			game.KeyValue(0.02f);

			///�V���h�E�̐ݒ�
			GameEngine::ShadowParameter shadowParam;
			shadowParam.lightPos = glm::vec3(0, 100, -50);
			shadowParam.lightDir = glm::normalize(glm::vec3(0,0, 25));
			shadowParam.lightUp = glm::vec3(0, 1, 0);
			shadowParam.near = 10;
			shadowParam.far = 200;
			shadowParam.range = glm::vec2(500, 500);
			game.Shadow(shadowParam);

			//�X�e�[�W���̐ݒ�
			stageName.pos = glm::vec2(-1,0);
			stageName.str = "To SPACE";

			//�w�i�G���e�B�e�B�쐬
			game.AddEntity(GameState::EntityGroupId_Background, glm::vec3(0, 0, 0),
				"SpaceSphere", Resource::tex_spaceSphere, std::make_shared<SpaceSphereMain>(), "NonLighting");

			break;
		}
		}
	}
	
	void MainGame::StageClear(float nextStageTimer){

		stageTimer = nextStageTimer; 
	}

	/**
	*	���C���Q�[����ʂ̍X�V
	*/
	void MainGame::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		//�X�e�[�W�J�ڏ���
		if (stageTimer > 0 ){
			stageTimer -= delta;
			if (stageTimer < 0) {
				//�X�e�[�W�J�ڂ̗v���𖞂�����

				if (stageNo == 3) {
					game.ReplaceScene(std::make_shared<GameEnd>(true));

					return;
				}

				StageLoad();
			}
			else if (stageTimer < launchController->LastSpawnedTime() * 0.3f) {
				isStageClear = true;
			}
		}
		

		//�Q�[���I�[�o�[���V�[���J�ڏ���
		if (sceneTimer == 0 && playerData->RemainingPlayer() < 0) {

			if (sceneTimer == 0) sceneTimer = 3;
			return;
		}
		if (sceneTimer > 0 && (sceneTimer -= delta) <= 0) {
			game.ReplaceScene(std::make_shared<GameEnd>());
			return;
		}

		//�G�o���Ǘ��̍X�V����
		if (launchController) {
			launchController->Update(delta);

			if (stageTimer == 0 && launchController->IsFinish() && stageNo != 3) {

				StageClear(launchController->LastSpawnedTime());
			}
		}

		DrawScreenInfo(delta);
	}
}
