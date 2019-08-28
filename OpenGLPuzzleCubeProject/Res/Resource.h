#pragma once

namespace Resource {

	const static char* titleName = "STAR FIGHTER";

	//画像ファイル

	const static char* tex_defaultNormal = "Res/Model/Dummy.Normal.bmp";
	const static char* tex_font = "Res/Font.fnt";

	const static char* tex_itemboxSpeed = "Res/Model/ItemBoxSpeed.dds";
	const static char* tex_itemboxPower = "Res/Model/ItemBoxBullet.dds";
	const static char* tex_toroid = "Res/Model/Toroid.dds";


//モデルファイル
	
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