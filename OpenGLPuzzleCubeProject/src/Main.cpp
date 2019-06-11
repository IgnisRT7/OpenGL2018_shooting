#include "GameEngine.h"
#include "../Res/Audio/testProject_acf.h"
#include "user/TitleScene.h"
#include <Windows.h>

#include <memory>

class classAtemp {
	
};
class classChild :public classAtemp {
};

using classAPtr = std::shared_ptr<classAtemp>;

int main() {

	classAPtr parent;

	parent = std::make_shared<classChild>();

	int cx, cy;
	cx = GetSystemMetrics(SM_CXSCREEN);
	cy = GetSystemMetrics(SM_CYSCREEN) ;
	float aspectRatio = 800.0f / 600.0f;

	GameEngine& game = GameEngine::Instance();
	if (!game.Init(static_cast<int>(cy * aspectRatio), cy, "OpenGL Tutorial")) {
		
		return 1;
	}

	if (!game.InitAudio("Res/Audio/testProject.acf", "Res/Audio/CueSheet_0.acb",
		"Res/Audio/CueSheet_0.awb", CRI_TESTPROJECT_DSPSETTING_DSPBUSSETTING_0)) {
		
		return 1;
	}

	//初期リソースの読み込み
	game.LoadTextureFromFile("Res/Model/Dummy.Normal.bmp");
	game.LoadFontFromFile("Res/Font.fnt");

	game.PushScene(std::make_shared<GameState::Title>());

	game.Run();

	return 0;
}