#include "Engine/GameEngine.h"
#include "../Res/Audio/testProject_acf.h"
#include "user/TitleScene.h"
#include "../Res/Resource.h"
#include <Windows.h>

#include <memory>

#ifndef _DEBUG
//デバッグ時用コンソールウインドウの表示化
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

int main(){

	int cy;
	cy = GetSystemMetrics(SM_CYSCREEN) ;
	float aspectRatio = 800.0f / 600.0f;

	GameEngine& game = GameEngine::Instance();
	if (!game.Init(static_cast<int>(cy * aspectRatio), cy, Resource::titleName)) {
		
		return 1;
	}

	if (!game.InitAudio(Resource::audioAcfFile, Resource::audioAcbFile ,
		Resource::audioAwbFile, CRI_TESTPROJECT_ACF_DSPSETTING_DSPBUSSETTING_0)) {
		
		return 1;
	}

	//初期リソースの読み込み
	game.LoadTextureFromFile(Resource::tex_defaultNormal);
	game.LoadFontFromFile(Resource::tex_font);

	game.PushScene(std::make_shared<GameState::Title>());

	game.Run();

	return 0;
}