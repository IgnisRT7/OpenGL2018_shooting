#pragma once

namespace Resource {

	const static char* titleName = "STAR FIGHTER";

	//画像ファイル

	const static char* tex_defaultNormal =	"Res/Model/Dummy.Normal.bmp";
	const static char* tex_font =			"Res/Font.fnt";

	const static char* tex_player =			"Res/Model/Player.dds";
	const static char* tex_toroid =			"Res/Model/Toroid.dds";
	const static char* tex_toroid_normal =	"Res/Model/Toroid_Normal.bmp";
	const static char* tex_itemboxSpeed =	"Res/Model/ItemBoxSpeed.dds";
	const static char* tex_itemboxBullet =	"Res/Model/ItemBoxBullet.dds";
	const static char* tex_bullet =			"Res/Model/sphere.dds";
	const static char* tex_healthMeter =	"Res/HealthMeter.dds";
	
	const static char* tex_bg02 =			"Res/Model/BG02.Diffuse.dds";
	const static char* tex_bg02_normal =	"Res/Model/BG02.Normal.bmp";
	const static char* tex_city01 =			"Res/Model/City01.Diffuse.dds";
	const static char* tex_city01_normal =	"Res/Model/City01.Normal.bmp";
	const static char* tex_spaceSphere =	"Res/Model/SpaceSphere.dds";

//モデルファイル
	const static char* fbx_player =			"Res/Model/Player.fbx";
	const static char* fbx_playerType2 =	"Res/Model/Player2.fbx";
	const static char* fbx_blast =			"Res/Model/Blast.fbx";
	const static char* fbx_toroid =			"Res/Model/Toroid.fbx";
	const static char* fbx_motherShip =		"Res/Model/MotherShip.fbx";
	const static char* fbx_itembox =		"Res/Model/ItemBox.fbx";
	const static char* fbx_bullet =			"Res/Model/sampleSphere.fbx";

	const static char* fbx_spaceSphere =	"Res/Model/SpaceSphere.fbx";
	const static char* fbx_landscape =		"Res/Model/Landscape.fbx";
	const static char* fbx_city =			"Res/Model/City01.fbx";

	
//音声ファイル
	const static char* audioAcfFile = "Res/Audio/testProject.acf";
	const static char* audioAcbFile = "Res/Audio/CueSheet_0.acb";
	const static char* audioAwbFile = "Res/Audio/CueSheet_0.awb";

//シェーダファイル	

	const static char* shaderFolderPass = "Res/Shader/";
	
	const static char* shaderNameList[][3] = {
	{ "Tutorial"	, "Tutorial.vert"	, "Tutorial.frag" },
	{ "ColorFilter"	, "ColorFilter.vert", "ColorFilter.frag" },
	{ "NonLighting"	, "NonLighting.vert", "NonLighting.frag" },
	{ "HiLumExtract", "TexCoord.vert"	, "HiLumExtract.frag" },
	{ "Shrink"		, "TexCoord.vert"	, "Shrink.frag" },
	{ "Blur3x3"		, "TexCoord.vert"	, "Blur3x3.frag" },
	{ "RenderDepth"	, "RenderDepth.vert", "RenderDepth.frag" },
	{ "RenderStencil","RenderStencil.vert","RenderStencil.frag"},
	{ "Sprite"		, "Sprite.vert"		, "Sprite.frag"},
	{ "Font"		, "Font.vert"		, "Font.frag"},
	};

}