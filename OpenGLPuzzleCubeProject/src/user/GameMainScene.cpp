/**
*	@file GameMainScene.cpp
*/
#include "GameMainScene.h"
#include "../GameState.h"
#include "../../Res/Audio/CueSheet_0.h"
#include "../../Res/Audio/testProject_acf.h"
#include "../Engine/GameEngine.h"

#include "Entity/Enemy.h"

#include "GameEndScene.h"
#include "TitleScene.h"

namespace GameState {

	/**
	*	コンストラクタ
	*
	*	@param i	ループするかどうかのフラグ
	*/
	Landscape::Landscape(bool l) :
		isLoop(l){
	}

	/// 背景
	void Landscape::Initialize() {
		entity->CastShadow(false);
		start = entity->Position();
	}

	void Landscape::Update(float delta) {

		timer += delta;
		glm::vec3 newPos = entity->Position() + glm::vec3(0, 0, -10.0f * delta);

		entity->Position(newPos);


		if (newPos.z < -80 && isLoop) {
			entity->Position(glm::vec3(glm::vec2(newPos), newPos.z + 200.0f));
		}
	}

	/// 背景(ステージ3用)

	/**
	*	背景の初期化処理
	*/
	void SpaceSphereMain::Initialize() {
		entity->CastShadow(false);
	}

	/**
	*	背景の更新処理
	*/
	void SpaceSphereMain::Update(float delta) {

		glm::vec3 rotSpace = glm::eulerAngles(entity->Rotation());
		rotSpace.x += glm::radians(2.5f) * delta;
		entity->Rotation(rotSpace);
	}

	
	///メインゲーム画面のクラス定義

	/**
	*	初期化処理
	*/
	bool MainGame::Initialize() {

		GameEngine& game = GameEngine::Instance();

		//リソースのロード
		const char* meshPassList[] = {
			"Res/Model/Player.fbx",
			"Res/Model/Blast.fbx",
			"Res/Model/Toroid.fbx",
			"Res/Model/MotherShip.fbx",
			"Res/Model/ItemBox.fbx",
			"Res/Model/sampleSphere.fbx",
			"Res/Model/Landscape.fbx",
			"Res/Model/City01.fbx",
			"Res/Model/SpaceSphere.fbx",
		};

		for (const char* meshPass : meshPassList) {
			if (!game.LoadMeshFromFile(meshPass)) {
				std::cout << "file: " << meshPass << "の読み込みに失敗" << std::endl;
			}
		}

		const char* texturePassList[] = {
			"Res/Model/Player.dds",
			"Res/Model/Toroid.dds",
			"Res/Model/Toroid.Normal.bmp",
			"Res/Model/ItemBoxSpeed.dds",
			"Res/Model/ItemBoxBullet.dds",
			"Res/Model/sphere.dds",
			"Res/Model/BG02.Diffuse.dds",
			"Res/Model/BG02.Normal.bmp",
			"Res/Model/City01.Diffuse.dds",
			"Res/Model/City01.Normal.bmp",
			"Res/Model/SpaceSphere.dds",
		};
		for (const char* texturePass : texturePassList) {
			if (!game.LoadTextureFromFile(texturePass)) {
				std::cout << "file: " << texturePass << "の読み込みに失敗" << std::endl;
			}
		}


		return true;
	}

	/**
	*	終了処理
	*/
	void MainGame::Finalize() {
	}

	/**
	*	再生処理
	*/
	void MainGame::Play() {

		GameEngine& game = GameEngine::Instance();

		game.UserVariable("score") = 0;

		stageName.size = glm::vec2(4);
		stageName.color = glm::vec4(0,0,1,1);

		///衝突判定用ハンドラの定義
		game.CollisionHandler(EntityGroupId_PlayerShot, EntityGroupId_Enemy);
		game.CollisionHandler(EntityGroupId_EnemyShot, EntityGroupId_Player);
		game.CollisionHandler(EntityGroupId_Player, EntityGroupId_Enemy);
		game.CollisionHandler(EntityGroupId_Player, EntityGroupId_Item);

		/// ライト・輝度の設定
		game.AmbientLight(glm::vec4(0.05f, 0.1f, 0.2f, 1));
		game.Light(0, { glm::vec4(1,100,1,1),glm::vec4(12000,12000,12000,1) });

		playerData = std::make_shared<Player>();

		//game.MainCamera(std::make_shared<CameraDebugComponent>());
		game.MainCamera(std::make_shared<CameraComponent>());
		game.MainCamera()->LookAt(glm::vec3(0, 50, -1), glm::vec3(0, 0, 0));

		///シャドウの設定
		GameEngine::ShadowParameter shadowParam;
		shadowParam.lightPos = glm::vec3(20, 50, 50);
		shadowParam.lightDir = glm::normalize(glm::vec3(-25, -150, 25)); 
		shadowParam.lightUp = glm::vec3(0, 1, 0);
		shadowParam.near = 10;
		shadowParam.far = 200;
		shadowParam.range = glm::vec2(500, 500);
		game.Shadow(shadowParam);

		game.TimeScale(1);

		StageLoad();

		sceneTimer = 0;
	}

	/**
	*	停止処理
	*/
	void MainGame::Stop() {

	}

	/**
	*	画面に表示する情報の描画処理
	*/
	void MainGame::DrawScreenInfo() {

		GameEngine& game = GameEngine::Instance();

		if (sceneTimer <= 0) {
			//スコア表示処理

			char str[16];
			snprintf(str, 16, "SCORE :%08.0f", game.UserVariable("score"));
			game.FontScale(glm::vec2(3));
			game.FontColor(glm::vec4(1, 1, 1, 1));
			game.AddString(glm::vec2(-0.95f, 0.95f), str);

			snprintf(str, 16, "P :%02.0f", glm::max(0.0, static_cast<double>(playerData->RemainingPlayer())));
			game.AddString(glm::vec2(-0.95f, 0.8f), str);

			game.FontScale(glm::vec2(2));
			snprintf(str, 16, "%02.0f fps", game.FPS());
			game.AddString(glm::vec2(-0.95f, -0.85f), str);
		}

		char str[16];
		snprintf(str, 16, "STAGE : %d", stageNo);

		game.FontScale(stageName.size);
		game.FontColor(stageName.color);
		game.AddString(glm::vec3(0),stageName.str.c_str(),true);
		game.AddString(glm::vec2(0.f, 0.15f), str,true);
	}

	/**
	*	ステージの読み込み処理
	*/
	void MainGame::StageLoad(){

		GameEngine& game = GameEngine::Instance();
		
		game.StopAllAudio();
		game.RemoveAllEntity();

		++stageNo; 
		
		launchController = std::make_shared<EnemyLaunchController>();
		launchController->Init(stageNo);

		auto playerEntity = game.AddEntity(EntityGroupId_Player, glm::vec3(0, 0, 0),
			"Aircraft", "Res/Model/Player.dds", playerData);

		playerData->StartMoveSet();

		stageTimer = 0;
		stageNameFadeTimer = 5.f;

		//ステージごとのロード処理
		switch (stageNo) {
		case 1: {
			game.PlayAudio(0, CRI_CUESHEET_0_MAINSCENE);

			game.KeyValue(0.16f);

			stageName.pos = glm::vec2(-1,0);
			stageName.color = glm::vec4(1, 0, 0, 1);
			stageName.str = "The beginning of the forest";
				
			//背景の更新処理
			for (int z = 0; z < 5; ++z) {
				const float offsetZ = static_cast<float>(z * 40 * 5);

				for (int x = 0; x < 5; ++x) {
					const float offsetX = static_cast<float>(x * 40 - 80) * 5.0f;
					game.AddEntity(EntityGroupId_Background, glm::vec3(offsetX, -100.0, offsetZ),
						"Landscape01", "Res/Model/BG02.Diffuse.dds", "Res/Model/BG02.Normal.bmp", std::make_shared<Landscape>());
				}
			}
			break;
		}
		case 2: {
			game.PlayAudio(0, CRI_CUESHEET_0_MAIN2SCENE);

			game.KeyValue(0.24f);

			stageName.pos = glm::vec2(-1,0);
			stageName.str = "The town that got stuck";

			///シャドウの設定
			GameEngine::ShadowParameter shadowParam;
			shadowParam.lightPos = glm::vec3(20, 50, 50);
			shadowParam.lightDir = glm::normalize(glm::vec3(-25, -50, 25));
			shadowParam.lightUp = glm::vec3(0, 1, 0);
			shadowParam.near = 10;
			shadowParam.far = 200;
			shadowParam.range = glm::vec2(500, 500);
			game.Shadow(shadowParam);

			//背景の初期化処理
			for (int z = 0; z < 5; ++z) {
				const float offsetZ = static_cast<float>(z * 40);

				for (int x = 0; x < 5; ++x) {
					const float offsetX = static_cast<float>(x * 40 - 80);
					game.AddEntity(EntityGroupId_Background, glm::vec3(offsetX, -10, offsetZ),
						"City01", "Res/Model/City01.Diffuse.dds", "Res/Model/City01.Normal.bmp", std::make_shared<Landscape>(true));
					game.AddEntity(EntityGroupId_Background, glm::vec3(offsetX, -10, offsetZ),
						"City01.Shadow", "Res/Model/City01.Diffuse.dds", "Res/Model/City01.Normal.bmp", std::make_shared<Landscape>(true));
				}
			}
			break;
			
		}
		case 3: {
			game.MainCamera()->LookAt(glm::vec3(0, 50, -10), glm::vec3(0, 0, 0));
			game.PlayAudio(0, CRI_CUESHEET_0_BOSSSCENE);

			game.KeyValue(0.02f);

			///シャドウの設定
			GameEngine::ShadowParameter shadowParam;
			shadowParam.lightPos = glm::vec3(0, 100, -50);
			shadowParam.lightDir = glm::normalize(glm::vec3(0,0, 25));
			shadowParam.lightUp = glm::vec3(0, 1, 0);
			shadowParam.near = 10;
			shadowParam.far = 200;
			shadowParam.range = glm::vec2(500, 500);
			game.Shadow(shadowParam);

			stageName.pos = glm::vec2(-1,0);
			stageName.str = "To SPACE";
			game.AddEntity(EntityGroupId_Background, glm::vec3(0, 0, 0),
				"SpaceSphere", "Res/Model/SpaceSphere.dds", std::make_shared<SpaceSphereMain>(), "NonLighting");

			break;
		}
		}
	}

	/**
	*	メインゲーム画面の更新
	*/
	void MainGame::Update(float delta) {

		GameEngine& game = GameEngine::Instance();

		//デバッグ用シャドウマッピング切り替え処理
		GamePad gamepad = game.GetGamePad();
		if (gamepad.buttonDown & GamePad::START) {
			static bool shadowMapping = false;
			shadowMapping = !shadowMapping;
			game.EnableShadow(shadowMapping);
		}

		//ステージ遷移処理
		if (stageTimer > 0 && (stageTimer -= delta) < 0) {

			if (stageNo == 3) {
				game.ReplaceScene(std::make_shared<GameEnd>(true));

				return;
			}

			StageLoad();
		}

		//敵出撃管理の更新処理
		if (launchController) {
			launchController->Update(delta);

			if (stageTimer == 0 && launchController->IsFinish() && stageNo != 3) {

				StageClear(10);
			}
		}

		//
		if (stageNameFadeTimer) {
			static const float fadeTime = 3.f - 1.f;
			stageNameFadeTimer -= delta;
			stageName.color = glm::vec4(glm::vec3(stageName.color), stageNameFadeTimer / fadeTime);
		}

		DrawScreenInfo();

		//ゲームオーバー時シーン遷移処理
		if (sceneTimer == 0 && playerData->RemainingPlayer() < 0) {

			if (sceneTimer == 0) sceneTimer = 3;
			return;
		}
		if (sceneTimer > 0 && (sceneTimer -= delta) <= 0) {
			game.ReplaceScene(std::make_shared<GameEnd>());
			return;
		}
	}
}
