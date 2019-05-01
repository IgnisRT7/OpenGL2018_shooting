/**
*	@file GameEngine.cpp
*/
#pragma once

#include "GameEngine.h"
#include "GLFWEW.h"
#include "Audio.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

///	���_�f�[�^�^
struct Vertex {
	glm::vec3 position;	///< ���W
	glm::vec4 color;	///< �F
	glm::vec2 texCoord;	///< �e�N�X�`�����W
	glm::vec3 normal;
};

/// ���_�f�[�^
const Vertex vertices[] = {
	{ { -0.5f, -0.3f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
{ { 0.3f, -0.3f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
{ { 0.3f,  0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
{ { -0.5f,  0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },

{ { -0.3f,  0.3f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },
{ { -0.3f, -0.5f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
{ { 0.5f, -0.5f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
{ { 0.5f, -0.5f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
{ { 0.5f,  0.3f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
{ { -0.3f,  0.3f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },

{ { -1.0f, -1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
{ { 1.0f, -1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
{ { 1.0f,  1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
{ { -1.0f,  1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },
};

const GLuint indices[] = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 7, 8, 9,
	10, 11, 12, 12, 13, 10,
};

/**
*	�����`��f�[�^
*/
struct RenderingPart {
	GLsizei size;	///< �`�悷��C���f�b�N�X��
	GLvoid* offset;	///< �`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g
};

/**
*	RenderingPart���쐬����
*
*	@param size �`�悷��C���f�b�N�X��
*	@param offset �`��J�n�C���f�b�N�X�̃I�t�Z�b�g(�C���f�b�N�X�P��)
*
*	@return �쐬���������`��I�u�W�F�N�g
*/
constexpr RenderingPart MakeRenderingPart(GLsizei size, GLsizei offset) {
	return { size,reinterpret_cast<GLvoid*>(offset * sizeof(GLuint)) };
}

/**
*	�����`��f�[�^���X�g
*/
static const RenderingPart renderingParts[] = {
	MakeRenderingPart(12,0),
	MakeRenderingPart(6,12),
};

/**
*	Vertex Buffer Object	���쐬����
*
*	@param size ���_�f�[�^�̃T�C�Y
*	@param data ���_�f�[�^�ւ̃|�C���^
*
*	@return �쐬����VBO
*/
GLuint CreateVBO(GLsizeiptr size, const GLvoid* data) {

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return vbo;

}

/**
*	Index Buffer Object���쐬����
*
*	@param size �C���f�b�N�X�f�[�^�̃T�C�Y
*	@param data �C���f�b�N�X�f�[�^�ւ̃|�C���^
*
*	@return �쐬����IBO
*/
GLuint CreateIBO(GLsizeiptr size, const GLvoid* data) {

	GLuint ibo = 0;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return ibo;
}

/**
*	���_�A�g���r���[�g��ݒ肷��
*
*	@param index ���_�A�g���r���[�g�̃C���f�b�N�X
*	@param cls	���_�f�[�^�^��
*	@param mbr	���_�A�g���r���[�g�ɐݒ肷��cls�̃����o�ϐ���
*/
#define SetVertexAttribPointer(index,cls,mbr) SetVertexAttribPointerI(\
	index,	\
sizeof(cls::mbr) / sizeof(float),	\
sizeof(cls),	\
reinterpret_cast<GLvoid*>(offsetof(cls,mbr)))

void SetVertexAttribPointerI(GLuint index, GLint size, GLsizei stride, const GLvoid* pointer) {

	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
}

/**
*	Vertex Array Object ���쐬����
*
*	@param vbo VAO�Ɋ֘A�t������VBO
*	@param ibo VAO�Ɋ֘A�t������IBO
*
*	@return �쐬����VAO
*/
GLuint CreateVAO(GLuint vbo, GLuint ibo) {

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	SetVertexAttribPointer(0, Vertex, position);
	SetVertexAttribPointer(1, Vertex, color);
	SetVertexAttribPointer(2, Vertex, texCoord);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBindVertexArray(0);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);

	return vao;
}


/**
*	�Q�[���G���W���̃C���X�^���X���擾����
*
*	@return �Q�[���G���W���̃C���X�^���X
*/
GameEngine& GameEngine::Instance() {
	static GameEngine instance;
	return instance;
}

/**
*	�Q�[���G���W��������������
*
*	@param w	�E�C���h�E�̕`��̕�(�s�N�Z��)
*	@param h	�E�C���h�E�̕`��̍���(�s�N�Z��)
*	@param title�E�C���h�E�^�C�g��
*
*	@retval true	����������
*	@retval false	���������s
*
*	Run�֐����Ăяo���O�ɁA��x�����Ăяo���Ă����K�v������
*	��x�������ɐ��������
*/
bool GameEngine::Init(int w, int h, const char* title) {

	windowSize = glm::vec2(w, h);

	if (isInitalized) {
		return true;
	}

	if (!GLFWEW::Window::Instance().Init(w, h, title)) {
		return false;
	}

	vbo = CreateVBO(sizeof(vertices), vertices);
	ibo = CreateIBO(sizeof(indices), indices);
	vao = CreateVAO(vbo, ibo);

	uboLight = UniformBuffer::Create(sizeof(Uniform::LightData), 1, "LightData");
	uboPostEffect = UniformBuffer::Create(sizeof(Uniform::PostEffectData), 2, "PostEffectData");

	//�o�b�N�o�b�t�@�쐬
	offscreen = OffscreenBuffer::Create(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), GL_RGBA16F);
	if (!offscreen) {
		return false;
	}


	//�u���[���G�t�F�N�g�p�k���o�b�t�@�쐬
	for (int i = 0, scale = 4; i < bloomBufferCount; ++i, scale *= 4) {
		const int w = static_cast<int>(windowSize.x) / scale;
		const int h = static_cast<int>(windowSize.y) / scale;
		offBloom[i] = OffscreenBuffer::Create(w, h, GL_RGBA16F);
		if (!offBloom[i]) {
			return false;
		}
	}

	//�f�v�X�V���h�E�o�b�t�@�쐬
	offDepth = OffscreenBuffer::Create(1024, 1024, GL_DEPTH_COMPONENT32);
	if (!offDepth) {
		return false;
	}

	//�X�e���V���}�X�N�p�o�b�t�@�쐬
	offStencil = OffscreenBuffer::Create(1024, 1024, GL_RGBA16F);
	if (!offStencil) {
		return false;
	}


	//�e�N�X�`���T�C�Y�擾
	int offWidth, offHeight;
	glBindTexture(GL_TEXTURE_2D, offBloom[bloomBufferCount - 1]->GetTexture());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &offWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &offHeight);
	glBindTexture(GL_TEXTURE_2D, 0);

	//PBO���쐬
	const int pboByteSize = offWidth * offHeight * sizeof(GLfloat) * 4;//tips :vec4 == GLfloat * 4
	for (auto& e : pbo) {
		e.Init(GL_PIXEL_PACK_BUFFER, pboByteSize, nullptr, GL_DYNAMIC_READ);
	}


	if (!vbo || !ibo || !vao || !uboLight || !uboPostEffect) {

		std::cerr << "ERROR: GameEngine�̏������Ɏ��s" << std::endl;
		return false;
	}

	//�V�F�[�_���X�g
	static const char* const shaderNameList[][3] = {
		{ "Tutorial", "Res/Tutorial.vert", "Res/Tutorial.frag" },
	{ "ColorFilter", "Res/ColorFilter.vert", "Res/ColorFilter.frag" },
	{ "NonLighting", "Res/NonLighting.vert", "Res/NonLighting.frag" },
	{ "HiLumExtract", "Res/TexCoord.vert", "Res/HiLumExtract.frag" },
	{ "Shrink", "Res/TexCoord.vert", "Res/Shrink.frag" },
	{ "Blur3x3", "Res/TexCoord.vert", "Res/Blur3x3.frag" },
	{ "RenderDepth", "Res/RenderDepth.vert", "Res/RenderDepth.frag" },
	};
	//�V�F�[�_�̃R���p�C��
	shaderMap.reserve(sizeof(shaderNameList) / sizeof(shaderNameList[0]));
	for (auto& e : shaderNameList) {
		Shader::ProgramPtr program = Shader::Program::Create(e[1], e[2]);
		if (!program) {
			return false;
		}
		shaderMap.insert(std::make_pair(std::string(e[0]), program));
	}
	//Uniform�u���b�N�̃o�C���h
	shaderMap["Tutorial"]->UniformBlockBinding("VertexData", 0);
	shaderMap["Tutorial"]->UniformBlockBinding("LightData", 1);
	shaderMap["ColorFilter"]->UniformBlockBinding("PostEffectData", 2);
	shaderMap["HiLumExtract"]->UniformBlockBinding("PostEffectData", 2);

	//meshBuffer�쐬
	meshBuffer = Mesh::Buffer::Create(100 * 1024, 100 * 1024);
	if (!meshBuffer) {
		std::cerr << "ERROR: GameEngine�̏������Ɏ��s" << std::endl;
	}

	textureStack.push_back(TextureLevel());

	//�G���e�B�e�B�o�b�t�@�̍쐬
	entityBuffer = Entity::Buffer::Create(1024, sizeof(Uniform::VertexData), 0, "VertexData");
	if (!entityBuffer) {
		std::cerr << "ERROR: GameEngine�̏������Ɏ��s" << std::endl;
		return false;
	}

	rand.seed(std::random_device()());

	fontRenderer.Init(1024, windowSize);

	isInitalized = true;
	return true;
}

/**
*	�Q�[�������s����
*/
void GameEngine::Run() {
	;

	const double delta = 1.0 / 60.0;
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	double prevTime = glfwGetTime();
	while (!window.ShouldClose()) {

		const double curTime = glfwGetTime();
		const double delta = curTime - prevTime;
		prevTime = curTime;

		window.UpdateGamePad();
		Update(glm::min(0.25, delta));
		Render();
		window.SwapBuffers();

		if (pboIndexForWriting < 0) {
			pboIndexForWriting = 0;
		}
		else {
			pboIndexForWriting ^= 1;
		}
	}
}

/**
*	��ԍX�V�֐���ݒ肷��
*
*	@param �ݒ肷��X�V�֐�
*/
void GameEngine::UpdateFunc(const UpdateFuncType& func) {

	updateFunc = func;
}

/**
*	��ԍX�V�֐����擾����
*
*	@return �ݒ肳��Ă���X�V�֐�
*/
const GameEngine::UpdateFuncType& GameEngine::UpdateFunc() const {

	return updateFunc;
}

/**
*	�e�N�X�`����ǂݍ���
*
*	@param filename �e�N�X�`���t�@�C����
*
*	@retval true	�ǂݍ��ݐ���
*	@retval false	�ǂݍ��ݎ��s
*/
bool GameEngine::LoadTextureFromFile(const char* filename) {

	if (GetTexture(filename)) {
		return true;
	}

	TexturePtr texture = Texture::LoadFromFile(filename);
	if (!texture) {
		return false;
	}

	textureStack.back().insert(std::make_pair(std::string(filename), texture));
	return true;
}

/**
*	���b�V����ǂݍ���
*
*	@param filename ���b�V���t�@�C����
*
*	@retval true	�ǂݍ��ݐ���
*	@retval false	�ǂݍ��ݎ��s
*/
bool GameEngine::LoadMeshFromFile(const char* filename) {
	return meshBuffer->LoadMeshFromFile(filename);
}

/**
*	�G���e�B�e�B��ǉ�����
*
*	@param groupId	�G���e�B�e�B�̃O���[�vID
*	@param pos		�G���e�B�e�B�̍��W
*	@param meshName	�G���e�B�e�B�̕\���Ɏg�p���郁�b�V����
*	@param texName	�G���e�B�e�B�̕\���Ɏg���e�N�X�`���t�@�C����
*	@param func		�G���e�B�e�B�̏�Ԃ��X�V����֐�(�܂��͊֐��I�u�W�F�N�g)
*	@param shader	�G���e�B�e�B�̕\���Ɏg���V�F�[�_��
*
*	@return �ǉ������G���e�B�e�B�ւ̃|�C���^
*			����ȏ�G���e�B�e�B��ǉ��ł��Ȃ��ꍇ��nullptr���Ԃ����
*			��]��g�嗦�͂��̃|�C���^�o�R�Őݒ肷��
*			�Ȃ��A���̃|�C���^���A�v���P�[�V�������ŕێ�����K�v�͂Ȃ�
*/
Entity::Entity* GameEngine::AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, Entity::Entity::UpdateFuncType func, const char* shader) {

	return AddEntity(groupId, pos, meshName, texName, nullptr, func, shader);
}

/**
*	�G���e�B�e�B��ǉ�����
*
*	@param groupId	�G���e�B�e�B�̃O���[�vID
*	@param pos		�G���e�B�e�B�̍��W
*	@param meshName	�G���e�B�e�B�̕\���Ɏg�p���郁�b�V����
*	@param texName	�G���e�B�e�B�̕\���Ɏg���e�N�X�`���t�@�C����
*	@param texNormal
*	@param func		�G���e�B�e�B�̏�Ԃ��X�V����֐�(�܂��͊֐��I�u�W�F�N�g)
*	@param shader	�G���e�B�e�B�̕\���Ɏg���V�F�[�_��
*
*	@return �ǉ������G���e�B�e�B�ւ̃|�C���^
*			����ȏ�G���e�B�e�B��ǉ��ł��Ȃ��ꍇ��nullptr���Ԃ����
*			��]��g�嗦�͂��̃|�C���^�o�R�Őݒ肷��
*			�Ȃ��A���̃|�C���^���A�v���P�[�V�������ŕێ�����K�v�͂Ȃ�
*/
Entity::Entity* GameEngine::AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, const char* normalName, Entity::Entity::UpdateFuncType func, const char* shader) {

	decltype(shaderMap)::const_iterator itr = shaderMap.end();
	if (shader) {
		itr = shaderMap.find(shader);
	}
	if (itr == shaderMap.end()) {
		itr = shaderMap.find("Tutorial");
		if (itr == shaderMap.end()) {
			return nullptr;
		}
	}

	const Mesh::MeshPtr& mesh = meshBuffer->GetMesh(meshName);
	TexturePtr tex[2];
	tex[0] = GetTexture(texName);
	if (normalName) {
		tex[1] = GetTexture(normalName);
	}
	else {
		tex[1] = GetTexture("Res/Model/Dummy.Normal.bmp");
	}
	return entityBuffer->AddEntity(groupId, pos, mesh, tex, itr->second, func);
}

/**
*	�G���e�B�e�B���폜����
*
*	@param �폜����G���e�B�e�B�̃|�C���^
*/
void GameEngine::RemoveEntity(Entity::Entity* e) {

	entityBuffer->RemoveEntity(e);
}

/**
*	�S�ẴG���e�B�e�B���폜����
*/
void GameEngine::RemoveAllEntity() {

	entityBuffer->RemoveAllEntity();
}

/**
*	���C�g��ݒ肷��
*
*	@param indes	�ݒ肷�郉�C�g�̃C���f���N�X
*	@param lilght	���C�g�f�[�^
*/
void GameEngine::Light(int index, const Uniform::PointLight& light) {

	if (index<0 || index>Uniform::maxLightCount) {
		std::cerr << "WARNING: '" << index << "'�͕s���ȃ��C�g�C���f�b�N�X�ł�" << std::endl;
		return;
	}
	lightData.light[index] = light;
}

/**
*	���C�g���擾����
*
*	@param inex �擾���郉�C�g�̃C���f�b�N�X
*
*	@return ���C�g�f�[�^
*/
const Uniform::PointLight& GameEngine::Light(int index)const {

	if (index<0 || index>Uniform::maxLightCount) {
		std::cerr << "WARNING: '" << index << "'�͕s���ȃ��C�g�C���f�b�N�X�ł�" << std::endl;
		static const Uniform::PointLight dummy;
		return dummy;
	}

	return lightData.light[index];
}

/**
*	������ݒ肷��
*
*	@param color	�����̖��邳
*/
void GameEngine::AmbientLight(const glm::vec4& color) {
	lightData.ambientColor = color;
}

/**
*	�������擾����
*
*	@return �����̖��邳
*/
const glm::vec4& GameEngine::AmbientLight() const {
	return lightData.ambientColor;
}

/**
*	���_�̈ʒu�Ǝp����ݒ肷��
*
*	@param index �ݒ肷��J�����̃C���f�b�N�X
*	@param cam �ݒ肷��J�����f�[�^
*/
void GameEngine::Camera(size_t index, const CameraData& cam) {
	camera[index] = cam;
	lightData.eyePos[index] = glm::vec4(cam.position, 0);
}

/**
*	���_�̈ʒu�Ǝp�����擾����
*
*	@param index �擾����J�����̃C���f�b�N�X
*
*	@rerturn �J�����f�[�^
*/
const GameEngine::CameraData& GameEngine::Camera(size_t index) const {
	return camera[index];
}

/**
*	�����I�u�W�F�N�g���擾����
*
*	@return �����I�u�W�F�N�g
*/
std::mt19937& GameEngine::Rand() {
	return rand;
}

/**
*	�Q�[���p�b�h�̏�Ԃ��擾����
*/
const GamePad& GameEngine::GetGamePad() const {
	return GLFWEW::Window::Instance().GetGamePad();
}

/**
*	�Փˉ����n���h����ݒ肷��
*
*	@param gid0		�ՓˑΏۂ̃O���[�vID
*	@param gid1		�ՓˑΏۂ̃O���[�vID
*	@param handler	�Փˉ����n���h��
*
*	�Փ˂��������Փˉ����n���h�����Ăяo�����Ƃ��A
*	��菬�����O���[�vID�����G���e�B�e�B�����ɓn�����B
*	�R�R�Ŏw�肵���O���[�vID�̏����Ƃ͖��֌W�ł��邱�Ƃɒ��ӂ��邱��
*
*	CollisionHandler(10,1,Func)
*	�Ƃ����R�[�h�Ńn���h����o�^�����Ƃ���A�Փ˂���������ƁA
*	Func(�O���[�vID=1�̃G���e�B�e�B�A�O���[�vID=10�̃G���e�B�e�B)
*	�̂悤�ɌĂяo�����
*/
void GameEngine::CollisionHandler(int gid0, int gid1, Entity::CollisionHandlerType handler) {
	entityBuffer->CollisionHandler(gid0, gid1, handler);
}

/**
*	�Փˉ����n���h�����擾����
*
*	@param gid0	�ՓˑΏۂ̃O���[�vID
*	@param gid1 �ՓˑΏۂ̃O���[�vID
*
*	@return �Փˉ����n���h��
*/
const Entity::CollisionHandlerType& GameEngine::CollisionHandler(int gid0, int gid1) const {

	return entityBuffer->CollisionHandler(gid0, gid1);
}

/**
*	�Փ˃n���hd��̃��X�g���N���A����
*/
void GameEngine::ClearCollisionHandlerList() {
	entityBuffer->ClearCollisionHanderList();
}

/**
*	@copydoc Audio::Initialize
*/
bool GameEngine::InitAudio(const char* acfPath, const char* acbPath, const char* awbPath, const char* dspBufName) {

	return Audio::Initialize(acfPath, acbPath, awbPath, dspBufName);
}

/**
*	@copydoc Audio::Play
*/
void GameEngine::PlayAudio(int playerId, int cueId) {

	Audio::Play(playerId, cueId);
}


/**
*	@copydoc Audio::Stop
*/
void GameEngine::StopAudio(int playerId) {
	Audio::Stop(playerId);
}

/**
*	���\�[�X�X�^�b�N�ɐV�������\�[�X���x�����쐬����
*/
void GameEngine::PushLevel() {

	meshBuffer->PushLevel();
	textureStack.push_back(TextureLevel());
}

/*
*	���\�[�X�X�^�b�N�̖����̃��\�[�X���x������������
*/
void GameEngine::PopLevel() {

	meshBuffer->PopLevel();
	if (textureStack.size() > minimalStackSize) {
		textureStack.pop_back();
	}
}

/**
*	���\�[�X�X�^�b�N�̖����̃��\�[�X���x������̏�Ԃɂ���
*/
void GameEngine::ClearLevel() {

	meshBuffer->ClearLevel();
	textureStack.back().clear();
}

/**
*	�G���e�B�e�B�o�b�t�@����G���e�B�e�B��T��
*
*	@param name	�G���e�B�e�B��
*/
Entity::Entity* GameEngine::FindEntity(std::string name) {

	return entityBuffer->FindEntity(name);
}

/**
*	�f�X�g���N�^
*/
GameEngine::~GameEngine() {

	updateFunc = nullptr;
	Audio::Destroy();

	if (vao) {
		glDeleteVertexArrays(1, &vao);
	}
	if (ibo) {
		glDeleteVertexArrays(1, &ibo);
	}
	if (vbo) {
		glDeleteVertexArrays(1, &vbo);
	}
}

/**
*	�Q�[���̏�Ԃ��X�V����
*
*	@param delta	�O��̍X�V����̌o�ߎ���(�b)
*/
void GameEngine::Update(double delta) {

	fontRenderer.MapBuffer();

	//�S�̂̍X�V����
	if (updateFunc) {
		updateFunc(delta);
	}

	//�s��v�Z����
	const glm::mat4x4 matProj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 1000.0f);
	glm::mat4x4 matView[Uniform::maxViewCount];
	for (int i = 0; i < Uniform::maxViewCount; ++i) {

		CameraData& cam = camera[i];
		matView[i] = glm::lookAt(cam.position, cam.target, cam.up);
	}

	//�V���h�E�̐ݒ�
	const glm::vec2 range = shadowParameter.range * 0.5f;
	const glm::mat4 matDepthProj = glm::ortho<float>(
		-range.x, range.x, -range.y, range.y, shadowParameter.near, shadowParameter.far);
	//	matDepthProj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 10, 200);

	const glm::mat4 matDepthView = glm::lookAt(
		shadowParameter.lightPos, shadowParameter.lightPos + shadowParameter.lightDir, shadowParameter.lightUp);

	//�G���e�B�e�B�o�b�t�@�̍X�V
	entityBuffer->Update(delta, matView, matProj, matDepthProj * matDepthView);

	fontRenderer.UnmapBuffer();
}

/**
*	�f�v�X�V���h�E�}�b�v��`�悷��
*/
void GameEngine::RenderShadow() const {

	glBindFramebuffer(GL_FRAMEBUFFER, offDepth->GetFramebuffer());
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glViewport(0, 0, offDepth->Width(), offDepth->Height());
	glScissor(0, 0, offDepth->Width(), offDepth->Height());
	glClearDepth(1);
	glClear(GL_DEPTH_BUFFER_BIT);

	const Shader::ProgramPtr& progDepth = shaderMap.find("RenderDepth")->second;
	progDepth->UseProgram();
	entityBuffer->DrawDepth(meshBuffer);
}

/**
*	�Q�[���̏�Ԃ�`�悷��
*/
void GameEngine::Render() {

	///Path1: Draw shadow.

	RenderShadow(); 

	///path ?: Draw stencil

	//glBindFramebuffer(GL_FRAMEBUFFER, offStencil->GetFramebuffer());

	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 0);
	glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
	glColorMask(0, 0, 0, 0);
	glDepthMask(0);

	glViewport(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
	glScissor(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));

	glClearStencil(0);


	glColorMask(1, 1, 1, 1);
	glDepthMask(1);

	glDisable(GL_STENCIL_TEST);

	///Path2: Draw entity.

	glBindFramebuffer(GL_FRAMEBUFFER, offscreen->GetFramebuffer());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
	glScissor(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderMap.find("Tutorial")->second->BindShadowTexture(GL_TEXTURE_2D, offDepth->GetTexture());

	//LightSetting
	uboLight->BUfferSubData(&lightData);
	//Draw entity
	entityBuffer->Draw(meshBuffer);

	///Path3: Extract only brightness

	glBindVertexArray(vao);

	const Shader::ProgramPtr& progHiLumExtract = shaderMap.find("HiLumExtract")->second;
	progHiLumExtract->UseProgram();

	glBindFramebuffer(GL_FRAMEBUFFER, offBloom[0]->GetFramebuffer());

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, offBloom[0]->Width(), offBloom[0]->Height());
	progHiLumExtract->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, offscreen->GetTexture());
	glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);

	///Path4: Bokasi

	const Shader::ProgramPtr& progShrink = shaderMap.find("Shrink")->second;
	progShrink->UseProgram();

	for (int i = 1; i < bloomBufferCount; ++i) {

		glBindFramebuffer(GL_FRAMEBUFFER, offBloom[i]->GetFramebuffer());
		glViewport(0, 0, offBloom[i]->Width(), offBloom[i]->Height());
		progShrink->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, offBloom[i - 1]->GetTexture());
		glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);

	}

	///Path5: Blend with data of frame buffer with addition synthesis

	const Shader::ProgramPtr& progBlur3x3 = shaderMap.find("Blur3x3")->second;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	progBlur3x3->UseProgram();

	for (int i = bloomBufferCount - 1; i > 0; --i) {

		glBindFramebuffer(GL_FRAMEBUFFER, offBloom[i - 1]->GetFramebuffer());
		glViewport(0, 0, offBloom[i - 1]->Width(), offBloom[i - 1]->Height());
		progBlur3x3->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, offBloom[i]->GetTexture());
		glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);
	}

	///final output:

	const Shader::ProgramPtr& progColorFilter = shaderMap.find("ColorFilter")->second;
	progColorFilter->UseProgram();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
	glViewport(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));

	glBindVertexArray(vao);

	progColorFilter->UseProgram();

	Uniform::PostEffectData postEffect;
	postEffect.luminanceScale = luminanceScale;
	postEffect.bloomThreshold = 1.0f / luminanceScale;
	uboPostEffect->BUfferSubData(&postEffect);
	progColorFilter->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, offscreen->GetTexture());
	progColorFilter->BindTexture(GL_TEXTURE1, GL_TEXTURE_2D, offBloom[0]->GetTexture());

	glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);

	fontRenderer.Draw();

	{
		// �I�t�X�N���[���o�b�t�@�̑傫�����擾.
		int width, height;
		glBindTexture(GL_TEXTURE_2D, offBloom[bloomBufferCount - 1]->GetTexture());
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
		glBindTexture(GL_TEXTURE_2D, 0);

		// ����(pboIndexForWriting<0�̏ꍇ)�͂܂��f�[�^���Ȃ����߁A
		// �ϊ��R�s�[���邾���ŋP�x�v�Z�͂��Ȃ�.
		if (pboIndexForWriting < 0) {

			// �I�t�X�N���[���o�b�t�@�̓��e��PBO�ɕϊ��R�s�[.
			const GLuint pboWrite = pbo[1].Id();
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pboWrite);
			glBindFramebuffer(GL_FRAMEBUFFER, offBloom[bloomBufferCount - 1]->GetFramebuffer());
			glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, 0);

		}
		else {

			// �I�t�X�N���[���o�b�t�@�̓��e��PBO�ɕϊ��R�s�[.
			const GLuint pboWrite = pbo[pboIndexForWriting].Id();
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pboWrite);
			glBindFramebuffer(GL_FRAMEBUFFER, offBloom[bloomBufferCount - 1]->GetFramebuffer());
			glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, 0);

			// PBO�̓��e��ǂݎ���ċP�x�X�P�[�����v�Z.
			const GLuint pboRead = pbo[pboIndexForWriting ^ 1].Id();
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pboRead);
			const GLfloat* p = static_cast<GLfloat*>(glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
			float totalLum = 0;
			for (int i = 0; i < width * height; ++i) {
				totalLum += p[i * 4 + 3];
			}
			luminanceScale = 0;
			luminanceScale = keyValue / std::exp(totalLum / static_cast<float>(width * height));
			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	}
}

