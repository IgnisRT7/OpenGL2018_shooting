#include "GameEngine.h"
#include "glm/gtc/matrix_transform.hpp"

#include "Uniform.h"
#include "GameState.h"
#include "../Res/Audio/SampleSound_acf.h"
#include "../Res/Audio/SampleCueSheet.h"
#include <random>

#include "user/TitleScene.h"

int main() {


	GameEngine& game = GameEngine::Instance();
	if (!game.Init(WINDOW_SIZE_W,WINDOW_SIZE_H, "OpenGL Tutorial")) {
		return 1;
	}

	if (!game.InitAudio("Res/Audio/SampleSound.acf", "Res/Audio/SampleCueSheet.acb",
		nullptr, CRI_SAMPLESOUND_ACF_DSPSETTING_DSPBUSSETTING_0)) {
		return 1;
	}

	game.LoadTextureFromFile("Res/Model/Dummy.Normal.bmp");
	game.LoadFontFromFile("Res/Font.fnt");

	game.PushLevel();
	game.UpdateFunc(GameState::Title());
	game.Run();

	return 0;
}