/**
*	@file MainGameState.cpp
*/
#include "GameState.h"
#include "GameEngine.h"
#include "../Res/Audio/SampleCueSheet.h"
#include <algorithm>

namespace GameState {

	///�Փˌ`�󃊃X�g
	static const Entity::CollisionData collisionDataList[] = {
		{ glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f,1.0f,1.0f) },
	{ glm::vec3(-0.5f,-0.5f,-1.0f), glm::vec3(0.5f,0.5f,1.0f) },
	{ glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f,1.0f,1.0f) },
	{ glm::vec3(-0.25f,-0.25f,-0.25f), glm::vec3(0.25f,0.25f,0.25f) },
	};

	/**
	*	�G�̉~�Ղ̏�Ԃ��X�V����
	*/
	struct UpdateToroid {

		//explicit UpdateToroid(const Entity::BufferPtr& buffer) : entityBuffer(buffer) {}
		void operator()(Entity::Entity& entity, double delta) {

			// �͈͊O�ɏo����폜����.
			const glm::vec3 pos = entity.Position();
			if (std::abs(pos.x) > 40.0f || std::abs(pos.z) > 40.0f) {
				GameEngine::Instance().RemoveEntity(&entity);
				return;
			}

			//TODO : �f�o�b�O�p�A�G�𓧖���������
			//auto colorValue = entity.Color();
			//colorValue.a = 0;
			//entity.Color(colorValue);

			//entity.Scale({ 2,5,2 });
			entity.Color({ 1,1,1,0.4f });

			// �~�Ղ���]������.
			float rot = glm::angle(entity.Rotation());
			rot += glm::radians(15.0f) * static_cast<float>(delta);
			if (rot > glm::pi<float>() * 2.0f) {
				rot -= glm::pi<float>() * 2.0f;
			}
			entity.Rotation(glm::angleAxis(rot, glm::vec3(0, 1, 0)));

		}

	};

	/**
	*	���@�̒e�̍X�V
	*/
	struct UpdatePlayerShot {
		void operator()(Entity::Entity& entity, double delta) {
			const glm::vec3 pos = entity.Position();
			if (std::abs(pos.x) > 40 || pos.z < -4 || pos.z > 40) {
				entity.Destroy();
				return;
			}
		}
	};

	/**
	*	�����̍X�V
	*/
	struct UpdateBlast {

		void operator()(Entity::Entity& entity, double delta) {
			timer += delta;

			if (timer >= 0.5) {
				entity.Destroy();
				return;
			}
			const float variation = static_cast<float>(timer * 4);	//�ω���
			entity.Scale(glm::vec3(static_cast<float>(1 + variation)));	//���X�Ɋg�債�Ă���

																		//���Ԍo�߂ŐF�Ɠ����x��ω�������
			static const glm::vec4 color[] = {
				glm::vec4(1.0f,1.0f,0.75f,1),
				glm::vec4(1.0f,0.5f,0.1f,1),
				glm::vec4(0.25f,0.1f,0.1f,0),
			};
			const glm::vec4 col0 = color[static_cast<int>(variation)];
			const glm::vec4 col1 = color[static_cast<int>(variation) + 1];
			const glm::vec4 newColor = glm::mix(col0, col1, std::fmod(variation, 1));
			entity.Color(newColor);

			//Y����]������
			glm::vec3 euler = glm::eulerAngles(entity.Rotation());
			euler.y += glm::radians(60.0f) + static_cast<float>(delta);
			entity.Rotation(glm::quat(euler));
		}

		double timer = 0;
	};


	/**
	*	���@�̍X�V
	*/
	struct UpdatePlayer {
		void operator()(Entity::Entity& entity, double delta) {
			
			static float timer = 0;
			timer += delta * 100;

			/*TODO : �v���C���[�̃_���[�W���̓_�ŃA�N�V�����e�X�g
			auto color = entity.Color();
			color.a = (glm::sin(timer) + 1) * 0.5;
			entity.Color(color);
			*/

			GameEngine& game = GameEngine::Instance();

			const GamePad gamepad = game.GetGamePad();
			glm::vec3 vec;

			float rotZ = 0;
			if (gamepad.buttons & GamePad::DPAD_LEFT) {
				vec.x = 1;
				rotZ = -glm::radians(30.0f);
			}
			else if (gamepad.buttons & GamePad::DPAD_RIGHT) {
				vec.x = -1;
				rotZ = glm::radians(30.0f);
			}
			if (gamepad.buttons & GamePad::DPAD_UP) {
				vec.z = 1;
			}
			else if (gamepad.buttons & GamePad::DPAD_DOWN) {
				vec.z = -1;
			}
			if (vec.x || vec.z) {
				vec = glm::normalize(vec) * 5.0f;
			}
			
			glm::vec3 rt = glm::vec3(-25, -100, 1);
			glm::vec3 ld = glm::vec3(25, 100, 80)
				;
			entity.Velocity(vec);
			entity.Rotation(glm::quat(glm::vec3(0, 0, rotZ)));
			glm::vec3 pos = entity.Position();
			pos = glm::min(ld, glm::max(pos, rt));
			entity.Position(pos);

			if (gamepad.buttons & GamePad::A) {
				shotInterval -= delta;
				glm::vec3 pos = entity.Position();
				pos.x -= 0.3f;

				game.PlayAudio(0, 0);
				for (int i = 0; i < 2; ++i) {
					if (Entity::Entity* p = game.AddEntity(EntityGroupId_PlayerShot, pos, "NormalShot", "Res/Player.bmp", UpdatePlayerShot())) {
						p->Velocity(glm::vec3(0, 0, 80));
						p->Collision(collisionDataList[EntityGroupId_PlayerShot]);
					}
					pos.x += 0.25f;
				}
			}
			else {
				shotInterval = 0;
			}

		}
	private:
		double shotInterval = 0;
	};

	void UpdateLandscape(Entity::Entity& entity, double delta) {

		entity.Position(entity.Position() + glm::vec3(0, 0, -4.0f * delta));
	}

	/**
	*	�w�i�̍X�V
	*/
	void UpdateSpaceSphereMainGame(Entity::Entity& entity, double delta) {

		glm::vec3 rotSpace = glm::eulerAngles(entity.Rotation());
		rotSpace.x += static_cast<float>(glm::radians(2.5) * delta);
		entity.Rotation(rotSpace);
	}


	/**
	*	���@�̒e�ƓG�̏Փ˔���
	*/
	void PlayerShotAndEnemyCollisionHandler(Entity::Entity& lhs, Entity::Entity& rhs) {

		GameEngine& game = GameEngine::Instance();

		//�����G�t�F�N�g
		if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, rhs.Position(), "Blast", "Res/Toroid.dds", UpdateBlast())) {
			const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
			p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
			p->Color(glm::vec4(1.0f, 0.75f, 0.5f, 1.0f));
			game.UserVariable("score") += 100;
		}

		//������
		game.PlayAudio(1, CRI_SAMPLECUESHEET_BOMB);
		lhs.Destroy();
		rhs.Destroy();
	}

	/**
	*	���@�ƓG�̏Փ˔���
	*/
	void PlayyerAndEnemyCollisionHandler(Entity::Entity& lhs, Entity::Entity& rhs) {

		GameEngine& game = GameEngine::Instance();

		//�����G�t�F�N�g
		if (Entity::Entity* p = game.AddEntity(EntityGroupId_Others, rhs.Position(), "Blast", "Res/Toroid.dds", UpdateBlast())) {
			const std::uniform_real_distribution<float> rotRange(0.0f, glm::pi<float>() * 2);
			p->Rotation(glm::quat(glm::vec3(0, rotRange(game.Rand()), 0)));
			game.UserVariable("score") += 100;
		}

		//������
		game.PlayAudio(1, CRI_SAMPLECUESHEET_BOMB);

		if (lhs.GroupId() == EntityGroupId_Player) {
			rhs.Destroy();			
		}
		else {
			lhs.Destroy();	
		}

		game.UserVariable("player") -= 1;

	}

	/**
	*	���C���Q�[����ʂ̃R���X�g���N�^
	*//*
	MainGame::MainGame(Entity::Entity* p) : pSpaceSphere(p) {

		GameEngine& game = GameEngine::Instance();
		game.CollisionHandler(EntityGroupId_PlayerShot, EntityGroupId_Enemy, &PlayerShotAndEnemyCollisionHandler);
		game.CollisionHandler(EntityGroupId_Player, EntityGroupId_Enemy, &PlayyerAndEnemyCollisionHandler);

		game.UserVariable("player") = 3;	//�v���C���[�c�@��
	}*/

	/**
	*	���C���Q�[����ʂ̍X�V
	*/
	void MainGame::operator()(double delta) {
		
		GameEngine& game = GameEngine::Instance();
		static const float stageTime = 90;

		if (stageTimer < 0) {
			++stageNo;
			stageTimer = stageTime;
			game.Camera(0, { glm::vec4(0,20,-8,1),glm::vec3(0,0,12),glm::vec3(0,0,1) });
			game.Camera(2, { glm::vec4(0,20,-8,1),glm::vec3(0,0,12),glm::vec3(0,0,1) });
			game.GroupVisibility(EntityGroupId_Background, 0, false);
			game.GroupVisibility(EntityGroupId_Background, 1, true);
			game.AmbientLight(glm::vec4(0.05f, 0.1f, 0.2f, 1));
			game.Light(0, { glm::vec4(40,100,10,1),glm::vec4(12000,12000,12000,1) });
			game.RemoveAllEntity();
			game.ClearLevel();

			game.LoadMeshFromFile("Res/Model/Player.fbx");
			game.LoadMeshFromFile("Res/Model/Toroid.fbx");
			game.LoadMeshFromFile("Res/Model/Blast.fbx");
			game.LoadTextureFromFile("Res/Model/Player.dds");
			game.LoadTextureFromFile("Res/Model/Toroid.dds");
			game.LoadTextureFromFile("Res/Model/Toroid.Normal.bmp");

			switch (stageNo % 3) {
			case 1:		

				game.KeyValue(0.16f);
				game.LoadMeshFromFile("Res/Model/Landscape.fbx");
				game.LoadTextureFromFile("Res/Model/BG02.Diffuse.dds");
				game.LoadTextureFromFile("Res/Model/BG02.Normal.bmp");

				for (int z = 0; z < 5; ++z) {
					
					const float offsetZ = static_cast<float>(z * 40 * 5);

					for (int x = 0; x < 5; ++x) {

						const float offsetX = static_cast<float>(x * 40 - 80)*5.0f;
						game.AddEntity(EntityGroupId_Others, glm::vec3(offsetX, -100, offsetZ), "Landscape01", "Res/Model/BG02.Diffuse.dds", "Res/Model/BG02.Normal.bmp", &UpdateLandscape);
					}
				}

				break;

			case 2:

				game.KeyValue(0.24f);
				game.LoadMeshFromFile("Res/Model/City01.fbx");
				game.LoadTextureFromFile("Res/Model/City01.Diffuse.dds");
				game.LoadTextureFromFile("Res/Model/City01.Normal.bmp");

				for (int z = 0; z < 12; ++z) {

					const float offsetZ = static_cast<float>(z * 40);

					for (int x = 0; x < 5; ++x) {

						const float offsetX = static_cast<float>(x * 40 - 80);
						game.AddEntity(EntityGroupId_Others, glm::vec3(offsetX, -10, offsetZ),
							"City01", "Res/ModelCity01.Diffuse.dds", "Res/Model/City01.Normal.bmp", &UpdateLandscape);
						game.AddEntity(EntityGroupId_Others, glm::vec3(offsetX, -10, offsetZ),
							"City01.Shadow", "Res/Model/City01.Diffuse.dds", "Res/Model/City01.Normal.bmp", &UpdateLandscape);
					}
				}

				break;

			case 0:

				game.KeyValue(0.02f);
				game.LoadMeshFromFile("Res/Model/SpadeSphere.fbx");
				game.LoadTextureFromFile("Res/Model/SpaceSphere.dds");
				game.AddEntity(EntityGroupId_Others, glm::vec3(0, 0, 0),
					"SpaceSphere", "Res/Model/SpaceSphere.dds", &UpdateSpaceSphereMainGame, "NonLighting");

				break;
			}

			auto pPlayer = game.AddEntity(EntityGroupId_Player, glm::vec3(0, 0, 2),
				"Aircraft", "Res/Model/Player.dds", UpdatePlayer());
			pPlayer->Collision(collisionDataList[EntityGroupId_Player]);
		}

		stageTimer -= delta;	

		//�X�R�A�̍X�V����
		char str[16];
		snprintf(str, 16, "%08.0f", game.UserVariable("score"));
		game.FontScale(glm::vec2(1.5));
		game.AddString(glm::vec2(-0.3f, 1.0f), str);

		snprintf(str, 16, "%02.0f", game.UserVariable("player"));
		std::string player = std::string("p - 0") + std::to_string((int)game.UserVariable("player"));
		game.AddString(glm::vec2(-0.9, 1.0f), player.c_str());

		//�J����1�̍X�V���� cos�ł̍��E�ړ�
		GameEngine::CameraData camera = game.Camera(1);
		float cameraMoveValue = fmod(static_cast<float>(stageTimer), 45.0f) * (glm::radians(360.0f) / 45.0f);
		camera.position.x = glm::cos(cameraMoveValue * 5.0f);
		game.Camera(1, camera);
	}

}
