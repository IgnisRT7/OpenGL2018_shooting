#include "GameEngine.h"
#include "../Res/Audio/testProject_acf.h"
#include "user/TitleScene.h"
#include <Windows.h>

int main() {

	int cx, cy;
	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN);
	float aspectRatio = 800.0f / 600.0f;

	GameEngine& game = GameEngine::Instance();
	if (!game.Init(cy * aspectRatio, cy, "OpenGL Tutorial")) {
		
		return 1;
	}

	if (!game.InitAudio("Res/Audio/testProject.acf", "Res/Audio/CueSheet_0.acb",
		"Res/Audio/CueSheet_0.awb", CRI_TESTPROJECT_DSPSETTING_DSPBUSSETTING_0)) {
		
		return 1;
	}

	//èâä˙ÉäÉ\Å[ÉXÇÃì«Ç›çûÇ›
	game.LoadTextureFromFile("Res/Model/Dummy.Normal.bmp");
	game.LoadFontFromFile("Res/Font.fnt");

	game.PushScene(std::make_shared<GameState::Title>());

	game.Run();

	return 0;
}