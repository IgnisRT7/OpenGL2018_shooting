#include "Engine/GameEngine.h"
#include "../Res/Audio/testProject_acf.h"
#include "user/TitleScene.h"
#include <Windows.h>

#include <memory>

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int main(){

	int cy;
	cy = GetSystemMetrics(SM_CYSCREEN) ;
	float aspectRatio = 800.0f / 600.0f;

	GameEngine& game = GameEngine::Instance();
	if (!game.Init(static_cast<int>(cy * aspectRatio), cy, "STAR FIGHTER")) {
		
		return 1;
	}

	const char* audioAcfFile = "Res/Audio/testProject.acf";
	const char* audioAcbFile = "Res/Audio/CueSheet_0.acb";
	const char* audioAwbFile = "Res/Audio/CueSheet_0.awb";

	if (!game.InitAudio(audioAcfFile,audioAcbFile ,
		audioAwbFile, CRI_TESTPROJECT_DSPSETTING_DSPBUSSETTING_0)) {
		
		return 1;
	}

	//èâä˙ÉäÉ\Å[ÉXÇÃì«Ç›çûÇ›
	game.LoadTextureFromFile("Res/Model/Dummy.Normal.bmp");
	game.LoadFontFromFile("Res/Font.fnt");

	game.PushScene(std::make_shared<GameState::Title>());

	game.Run();

	return 0;
}