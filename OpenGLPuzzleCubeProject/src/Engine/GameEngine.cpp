/**
*	@file GameEngine.cpp
*/
#pragma once

#include "GameEngine.h"
#include "GLFWEW.h"
#include "Audio.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../../Res/Resource.h"
//#include "DebugLog.h"

///	�ŏI�o�͗p�̒��_�f�[�^�^
struct Vertex {
	glm::vec3 position;	///< ���W
	glm::vec4 color;	///< �F
	glm::vec2 texCoord;	///< �e�N�X�`�����W
};

/// �ŏI�o�͗p�̒��_�f�[�^
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

///�ŏI�o�͗p�̃C���f�b�N�X�f�[�^
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

	if (isInitalized) {
		return true;
	}

	windowSize = glm::vec2(w, h);
	windowAspect = w / h;
	viewportRect[0] = glm::vec2(0, 0);
	viewportRect[1] = windowSize;

	if (!GLFWEW::Window::Instance().Init(w, h, title)) {
		return false;
	}

	//�I�t�X�N���[���o�b�t�@�̒��_�f�[�^������
	vbo.Init(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	ibo.Init(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	vao.Init(vbo.Id(), ibo.Id());

	vao.Bind();
	vao.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
	vao.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color));
	vao.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));
	vao.UnBind();

	if (!ibo.Id() || !vbo.Id() || !vao.Id() ) {

		std::cerr << "[Error]: GameEngine::Init �������Ɏ��s(MainBufferFaild)" << std::endl;
		return false;
	}

	///�`��p�o�b�t�@�Q�쐬

	offscreen = OffscreenBuffer::Create(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), GL_RGBA16F);
	offDepth = OffscreenBuffer::Create(4096, 4096, GL_DEPTH_COMPONENT32);
	offStencil = OffscreenBuffer::Create(1024, 1024, GL_RGBA16F);
	
	bool offBloomFaild = false;
	for (int i = 0, scale = 4; i < bloomBufferCount; ++i, scale *= 4) {
		const int w = static_cast<int>(windowSize.x) / scale;
		const int h = static_cast<int>(windowSize.y) / scale;
		offBloom[i] = OffscreenBuffer::Create(w, h, GL_RGBA16F);
		if (!offBloom[i]) {
			offBloomFaild = true;
			break;
		}
	}

	//�e�N�X�`���T�C�Y�擾
	int offWidth, offHeight;
	glBindTexture(GL_TEXTURE_2D, offBloom[bloomBufferCount - 1]->GetTexture());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &offWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &offHeight);
	glBindTexture(GL_TEXTURE_2D, 0);

	//PBO���쐬
	const int pboByteSize = offWidth * offHeight * sizeof(GLfloat) * 4;
	for (auto& e : pbo) {
		e.Init(GL_PIXEL_PACK_BUFFER, pboByteSize, nullptr, GL_DYNAMIC_READ);
	}

	if (!offscreen || !offDepth || !offStencil || offBloomFaild || !pbo->Id() || !pbo->Id()) {
		std::cerr << "[Error]: GameEngine::Init �������Ɏ��s(OffscreenBuffersFaild)" << std::endl;
		return false;
	}

	//�V�F�[�_�̃R���p�C��
	shaderMap.reserve(sizeof(Resource::shaderNameList) / sizeof(Resource::shaderNameList[0]));
	for (auto& e : Resource::shaderNameList) {

		std::string vPass = (std::string(Resource::shaderFolderPass) + e[1]);
		std::string fPass = (std::string(Resource::shaderFolderPass) + e[2]);

		Shader::ProgramPtr program = Shader::Program::Create(vPass.c_str(), fPass.c_str());
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

	uboLight = UniformBuffer::Create(sizeof(Uniform::LightData), 1, "LightData");
	uboPostEffect = UniformBuffer::Create(sizeof(Uniform::PostEffectData), 2, "PostEffectData");

	//meshBuffer�쐬
	meshBuffer = Mesh::Buffer::Create(1000 * 1024, 1000 * 1024);
	if (!meshBuffer) {
		std::cerr << "[Error]: GameEngine::Init �������Ɏ��s" << std::endl;
	}

	textureStack.push_back(TextureLevel());

	//�G���e�B�e�B�o�b�t�@�̍쐬
	entityBuffer = Entity::Buffer::Create(1024, sizeof(Uniform::VertexData), 0, "VertexData");
	if (!entityBuffer) {
		std::cerr << "[Error]: GameEngine�̏������Ɏ��s" << std::endl;
		return false;
	}

	//�X�v���C�g�����_���̃o�b�t�@�쐬
	spriteRenderer.Init(1000, shaderMap["Sprite"]);

	//TODO : ����p�J�����R���|�[�l���g�쐬
	mainCamera = std::make_shared<CameraComponent>();
	mainCamera->Aspect(windowSize.x / windowSize.y);

	rand.seed(std::random_device()());

	fontRenderer.Init(1024, windowSize);

	isInitalized = true;
	return true;
}

/**
*	�Q�[�������s����
*/
void GameEngine::Run() {
	
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	//���C�����[�v
	while (!window.ShouldClose()) {

		int w, h;
		if (window.GetWindowSize(&w, &h)) {
			//�E�C���h�E�T�C�Y���ύX���ꂽ

			CalculateViewPortByAspect(w, h, (800.0f / 600.0f));
		}

		//�E�C���h�E�V�X�e�����̍X�V
		window.UpdateDeltaTime();
		deltaTime = window.DeltaTime();
		UpdateFps();
		window.UpdateGamePad();

		//�A�v���P�[�V�������̍X�V����
		spriteRenderer.BeginUpdate();

		if (!Update(window.DeltaTime())) {
			break;
		}

		spriteRenderer.EndUpdate();

		//�`�揈��
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
*	�t�H���g��ǂݍ���
*
*	@param filename �t�H���g�t�@�C����
*
*	@retval true	�ǂݍ��ݐ���
*	@retval false	�ǂݍ��ݎ��s
*/
bool GameEngine::LoadFontFromFile(const char* filename){

	return fontRenderer.LoadFromFile(filename); 
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
Entity::Entity* GameEngine::AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, Entity::EntityDataBasePtr eData, const char* shader) {

	return AddEntity(groupId, pos, meshName, texName, nullptr, eData, shader);
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
Entity::Entity* GameEngine::AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, const char* normalName, Entity::EntityDataBasePtr eData, const char* shader) {
	
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
	if (meshName) {
		tex[0] = GetTexture(texName);
		if (normalName) {
			tex[1] = GetTexture(normalName);
		}
		else {
			tex[1] = GetTexture(Resource::tex_defaultNormal);
		}
	}
	return entityBuffer->AddEntity(groupId, pos, mesh, tex, itr->second, eData);
}

/**
*	�X�v���C�g�̒ǉ�����
*
*	@param s	�ǉ�����X�v���C�g
*/
void GameEngine::AddSprite(Sprite &s){

	spriteRenderer.AddVertices(s);
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
		std::cerr << "[Warning]: GameEngine::Light '" << index << "'�͕s���ȃ��C�g�C���f�b�N�X�ł�" << std::endl;
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
		std::cerr << "[Warning]: '" << index << "'�͕s���ȃ��C�g�C���f�b�N�X�ł�" << std::endl;
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
void GameEngine::CollisionHandler(int gid0, int gid1) {
	entityBuffer->CollisionHandler(gid0, gid1);
}

/**
*	�Փ˃n���h���̃��X�g���N���A����
*/
void GameEngine::ClearCollisionHandlerList() {
	entityBuffer->ClearCollisionHanderList();
}

/**
*	�e�N�X�`���̎擾
*
*	@param filename �e�N�X�`����
*
*	@return true	�擾�����e�N�X�`��
*	@return false	�_�~�[�̃e�N�X�`��
*/
const TexturePtr& GameEngine::GetTexture(const char* filename) const {

	for (const auto& e : textureStack) {
		const auto itr = e.find(filename);
		if (itr != e.end()) {
			return itr->second;
		}
	}

	//std::cerr << "GameEngine::GetTexture filename: "<<filename << " �e�N�X�`�������݂��܂���ł����B" << std::endl;
	static const TexturePtr dummy;
	return dummy;
}

/**
*	�A�X�y�N�g��ɉ������r���[�|�[�g�T�C�Y�̕ύX
*
*	@param width	
*/
void GameEngine::CalculateViewPortByAspect(int width, int height,float aspect){

	windowAspect = aspect;
	float theoreticalWidth = height * windowAspect;	/// �E�C���h�E�T�C�Y�̍�����1�Ƃ����Ƃ��̉����̃T�C�Y(���_�l)

	viewportRect[1].y = (theoreticalWidth > width) ? width * (1 / aspect) : height;
	viewportRect[1].x = viewportRect[1].y * aspect;

	viewportRect[0].x = (width - viewportRect[1].x) * 0.5f;
	viewportRect[0].y = (height - viewportRect[1].y) * 0.5f;
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
*	@copydoc Audio::StopAll
*/
void GameEngine::StopAllAudio() {

	Audio::StopAll();
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

	textureStack.back().clear();
	meshBuffer->ClearLevel();
}

/**
*	�V�[�����v�b�V�����܂�
*/
void GameEngine::PushScene(ScenePtr s){
	
	SceneStack::Instance().Push(s);
}

/**
*	�V�[�����|�b�v���܂�
*/
void GameEngine::PopScene(){

	SceneStack::Instance().Pop();
}

/**
*	�V�[������芷���܂�
*/
void GameEngine::ReplaceScene(ScenePtr s){
	
	//meshBuffer->ClearLevel();
	SceneStack::Instance().Replace(s);
}

/**
*	�f�X�g���N�^
*/
GameEngine::~GameEngine() {

	Audio::Destroy();

	if (vao.Id()) {
		vao.Destroy();
	}
	if (ibo.Id()) {
		ibo.Destroy();
	}
	if (vbo.Id()) {
		vbo.Destroy();
	}
}


/**
*	�Q�[���̏�Ԃ��X�V����
*
*	@param delta	�O��̍X�V����̌o�ߎ���(�b)
*/
bool GameEngine::Update(float delta) {

	if (GetGamePad().buttonDown & GamePad::ESCAPE) {
		return false;
	}

	float ratedDelta = delta * timeScale;

	SceneStack& sceneStack = SceneStack::Instance();

	//�V�[���̃t�F�[�h���o����
	if (isSceneFadeStart){
		if ((sceneFadeTimer -= ratedDelta) < 0) {

			isSceneFadeStart = false;
			sceneFadeTimer = 3.0f;
		}
	}

	fontRenderer.MapBuffer();

	sceneStack.Update(ratedDelta);

	glm::mat4 matProj;
	glm::mat4 matView[4];

	if (mainCamera) {
		//���C���J��������r���[�E�ˉe�ϊ��s��̏����擾����

		mainCamera->Update(ratedDelta);

		matProj = mainCamera->ProjctionMatrix();
		matView[0] = mainCamera->ViewMatrix();
	}

	//�V���h�E�̐ݒ�
	const glm::vec2 range = shadowParameter.range * 0.5f;
	const glm::mat4 matDepthProj = glm::ortho<float>(
		-range.x, range.x, -range.y, range.y, shadowParameter.near, shadowParameter.far);

	const glm::mat4 matDepthView = glm::lookAt(
		shadowParameter.lightPos, shadowParameter.lightPos + shadowParameter.lightDir, shadowParameter.lightUp);

	//�G���e�B�e�B�o�b�t�@�̍X�V
	entityBuffer->Update(ratedDelta, matView, matProj, matDepthProj * matDepthView);

	fontRenderer.UnmapBuffer();

	return true;
}

/**
*	Fps�̍X�V����
*/
void GameEngine::UpdateFps(){

	// fps �̍X�V����
	fpsBuffer[++bufferCount % 60] = deltaTime;

	fps = 0;
	for (int i = 0; i < 60; i++) {
		fps += fpsBuffer[i];
	}

	fps /= 60.0f;
	fps = 1 / fps;
}

/**
*	�f�v�X�V���h�E�}�b�v��`�悷��
*/
void GameEngine::RenderShadow() const {

	glBindFramebuffer(GL_FRAMEBUFFER, offDepth->GetFramebuffer());
	glClearDepth(1);
	glClear(GL_DEPTH_BUFFER_BIT);

	if (!isEnableShadow) {
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glDisable(GL_BLEND);
	glViewport(0, 0, offDepth->Width(), offDepth->Height());
	glScissor(0, 0, offDepth->Width(), offDepth->Height());

	const Shader::ProgramPtr& progDepth = shaderMap.find("RenderDepth")->second;
	progDepth->UseProgram();
	entityBuffer->DrawDepth(meshBuffer);

	glCullFace(GL_BACK);
}

/**
*	�X�e���V���o�b�t�@��`�悷��
*/
void GameEngine::RenderStencil() const {

	glBindFramebuffer(GL_FRAMEBUFFER, offStencil->GetFramebuffer());
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!isDrawOutline) {
		//�X�e���V���o�b�t�@�͌���A�E�g���C���݂̂Ȃ̂ŕ`�悵�Ȃ��ꍇ�͐؂��Ă���
		return;
	}

	glViewport(0, 0, offStencil->Width(), offStencil->Height());
	glScissor(0, 0, offStencil->Width(), offStencil->Height());

	const Shader::ProgramPtr& progStencil = shaderMap.find("RenderStencil")->second;

	progStencil->UseProgram();
	progStencil->SetVectorParameter(glm::vec3(1, 0, 0), "stencilColor");
	entityBuffer->DrawStencil(meshBuffer,progStencil);

}

/**
*	�G���e�B�e�B��`�悷��
*/
void GameEngine::RenderEntity() const {

	glBindFramebuffer(GL_FRAMEBUFFER, offscreen->GetFramebuffer());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, offscreen->Width(), offscreen->Height());
	glScissor(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));

	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderMap.find("Tutorial")->second->BindShadowTexture(GL_TEXTURE_2D, offDepth->GetTexture());

	uboLight->BufferSubData(&lightData);

	//�G���e�B�e�B���Ƃɕ`��Ɏg�p����V�F�[�_���قȂ邽�߂����ł͐ݒ肵�Ȃ�
	entityBuffer->Draw(meshBuffer);
}

/**
*	�u���[���G�t�F�N�g��`�悷��
*/
void GameEngine::RenderBloomEffect() const {

	///���邢�����̒��o����

	glBindFramebuffer(GL_FRAMEBUFFER, offBloom[0]->GetFramebuffer());

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, offBloom[0]->Width(), offBloom[0]->Height());

	vao.Bind();

	const Shader::ProgramPtr& progHiLumExtract = shaderMap.find("HiLumExtract")->second;
	progHiLumExtract->UseProgram();

	progHiLumExtract->BindTexture(GL_TEXTURE0, offscreen->GetTexture());
	glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);

	///���o�����f�[�^���ڂ���

	const Shader::ProgramPtr& progShrink = shaderMap.find("Shrink")->second;
	progShrink->UseProgram();

	for (int i = 1; i < bloomBufferCount; ++i) {

		glBindFramebuffer(GL_FRAMEBUFFER, offBloom[i]->GetFramebuffer());
		glViewport(0, 0, offBloom[i]->Width(), offBloom[i]->Height());
		progShrink->BindTexture(GL_TEXTURE0, offBloom[i - 1]->GetTexture());
		glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);

	}

	///�t���[���o�b�t�@�ɑ΂��Ăڂ������H���s���܂�

	const Shader::ProgramPtr& progBlur3x3 = shaderMap.find("Blur3x3")->second;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	progBlur3x3->UseProgram();

	for (int i = bloomBufferCount - 1; i > 0; --i) {

		glBindFramebuffer(GL_FRAMEBUFFER, offBloom[i - 1]->GetFramebuffer());
		glViewport(0, 0, offBloom[i - 1]->Width(), offBloom[i - 1]->Height());
		progBlur3x3->BindTexture(GL_TEXTURE0, offBloom[i]->GetTexture());
		glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);
	}

	vao.UnBind();
}

/**
*	�I�t�X�N���[���o�b�t�@�ɕ`�悷��
*/
void GameEngine::RenderFrameBuffer() const{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
	glViewport(static_cast<int>(viewportRect[0].x), static_cast<int>(viewportRect[0].y),
		static_cast<int>(viewportRect[1].x), static_cast<int>(viewportRect[1].y));

	vao.Bind();

	const Shader::ProgramPtr& progColorFilter = shaderMap.find("ColorFilter")->second;
	progColorFilter->UseProgram();

	progColorFilter->SetFloatParameter(isSceneFadeStart ? (sceneFadeTimer / 3) : 1, "scenFadeTimerRate");
	progColorFilter->SetBoolParameter(isDrawOutline, "isDrawOutline");

	Uniform::PostEffectData postEffect;
	postEffect.luminanceScale = luminanceScale;
	postEffect.bloomThreshold = 1.0f / luminanceScale;
	uboPostEffect->BufferSubData(&postEffect);
	progColorFilter->BindTexture(GL_TEXTURE0, offscreen->GetTexture());
	progColorFilter->BindTexture(GL_TEXTURE1, offBloom[0]->GetTexture());
	progColorFilter->BindTexture(GL_TEXTURE2, offDepth->GetTexture());

	glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);
	vao.UnBind();
}

/**
*	�Q�[���̏�Ԃ�`�悷��
*/
void GameEngine::Render() {

	RenderShadow(); 

	RenderStencil();

	RenderEntity();

	RenderBloomEffect();
	
	RenderFrameBuffer();

	spriteRenderer.Draw(this->windowSize);

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

			// �I�t�X�N���[���o�b�t�@�̓��e��PBO�ɕϊ��R�s�[
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

