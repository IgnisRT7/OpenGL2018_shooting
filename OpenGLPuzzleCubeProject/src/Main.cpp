#include "GameEngine.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Uniform.h"
#include "GameState.h"
#include "../Res/Audio/SampleSound_acf.h"
#include "../Res/Audio/SampleCueSheet.h"
#include <random>

///エンティティの衝突グループID
enum EntityGroupId {
	EntityGroupId_Player,
	EntityGroupId_PlayerShot,
	EntityGroupId_Enemy,
	EntityGroupId_EnemyShot,
	EntityGroupId_Others,
};

int main() {

	GameEngine& game = GameEngine::Instance();
	if (!game.Init(800, 600, "OpenGL Tutorial")) {
		return 1;
	}

	if (!game.InitAudio("Res/Audio/SampleSound.acf", "Res/Audio/SampleCueSheet.acb",
		nullptr, CRI_SAMPLESOUND_ACF_DSPSETTING_DSPBUSSETTING_0)) {
		return 1;
	}

//	game.LoadTextureFromFile("Res/SpaceSphere.bmp");
//	game.LoadTextureFromFile("Res/Toroid.dds");
//	game.LoadTextureFromFile("Res/Toroid.Normal.bmp");
//	game.LoadTextureFromFile("Res/Player.bmp");

//	game.LoadMeshFromFile("Res/SpaceSphere.fbx");
//	game.LoadMeshFromFile("Res/Toroid.fbx");
//	game.LoadMeshFromFile("Res/Player.fbx");
//	game.LoadMeshFromFile("Res/Blast.fbx");

	game.LoadTextureFromFile("Res/Model/Dummy.Normal.bmp");
	game.LoadFontFromFile("Res/Font.fnt");

	game.PushLevel();
	
	game.UpdateFunc(GameState::Title());
	game.Run();

	return 0;
}