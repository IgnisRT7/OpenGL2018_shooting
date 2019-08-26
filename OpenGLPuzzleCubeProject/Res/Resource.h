#pragma once

namespace Resource {

	const static char* titleName = "STAR FIGHTER";

//�摜�t�@�C��

	const static char* tex_defaultNormal = "Res/Model/Dummy.Normal.bmp";
	const static char* tex_font = "Res/Font.fnt";


//���f���t�@�C��

	
//�����t�@�C��
	const static char* audioAcfFile = "Res/Audio/testProject.acf";
	const static char* audioAcbFile = "Res/Audio/CueSheet_0.acb";
	const static char* audioAwbFile = "Res/Audio/CueSheet_0.awb";

//�V�F�[�_�t�@�C��	

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
	{ "Sprite"		,"Sprite.vert"		,"Sprite.frag"},
	};

}