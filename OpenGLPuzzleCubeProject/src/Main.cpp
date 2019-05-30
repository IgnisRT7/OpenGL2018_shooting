#include "GameEngine.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Uniform.h"
#include "GameState.h"
#include "../Res/Audio/testProject_acf.h"
#include "../Res/Audio/CueSheet_0.h"
#include <random>

#include "user/TitleScene.h"

int main() {

	GameEngine& game = GameEngine::Instance();
	if (!game.Init(WINDOW_SIZE_W,WINDOW_SIZE_H, "OpenGL Tutorial")) {
		return 1;
	}

	if (!game.InitAudio("Res/Audio/testProject.acf", "Res/Audio/CueSheet_0.acb",
		"Res/Audio/CueSheet_0.awb", CRI_TESTPROJECT_DSPSETTING_DSPBUSSETTING_0)) {
		
		return 1;
	}

	game.LoadTextureFromFile("Res/Model/Dummy.Normal.bmp");
	game.LoadFontFromFile("Res/Font.fnt");

	game.PushScene(std::make_shared<GameState::Title>());

	game.Run();

	return 0;
}