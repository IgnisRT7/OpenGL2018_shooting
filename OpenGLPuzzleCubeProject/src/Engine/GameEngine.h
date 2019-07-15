/**
*	@file GameEngine.h
*/
#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <random>
#include <GL/glew.h>

#include "Uniform.h"
#include "OffscreenBuffer.h"
#include "BufferObject.h"
#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"
#include "Entity.h"
#include "Uniform.h"
#include "GamePad.h"
#include "Font.h"
#include "Sprite.h"

#include "Scene.h"

#define COMPONENT_TYPEPTR(type) std::shared_ptr<type>			///�V�[���R���|�[�l���g�̌^�p�}�N��

#define FILEPASS_SHADER "Res/Shader/"

/**
*	�Q�[���G���W���N���X
*/
class GameEngine {
	friend class SceneStack;
public:

	static GameEngine& Instance();
	bool Init(int w, int h, const char* title);
	void Run();

	//�I�[�f�B�I�֌W�̃��b�p�[�֐�
	bool InitAudio(const char* acfPath, const char* acbPath, const char* awbPath, const char* dspBusName);
	void PlayAudio(int playerId, int cueId);
	void StopAudio(int playerId);
	void StopAllAudio();

	//���\�[�X�ǂݍ���
	bool LoadTextureFromFile(const char* filename);
	bool LoadMeshFromFile(const char* filename);
	bool LoadFontFromFile(const char* filename);

	//�G���e�B�e�B�ǉ�����
	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);
	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, const char* normalName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);

	//�X�v���C�g�֘A
	void AddSprite(Sprite& s);

	//�G���e�B�e�B�폜����
	void RemoveEntity(Entity::Entity*);
	void RemoveAllEntity();

	//�G���e�B�e�B��������
	template<typename T>
	Entity::Entity* FindEntityData() { return entityBuffer->FindEntityData<T>(); }

	//�t�H���g�֘A�̏���
	bool AddString(const glm::vec2& pos, const char* str, bool isCenter = false) {
		return fontRenderer.AddString(pos, str, isCenter);
	}
	void FontScale(const glm::vec2& scale) { fontRenderer.Scale(scale); }
	void FontColor(const glm::vec4& color) { fontRenderer.Color(color); }

	//�V�[���ǉ��E�폜����
	void PushScene(ScenePtr s);
	void PopScene();
	void ReplaceScene(ScenePtr s);

	const glm::vec2& WindowSize() const { return windowSize; }

	///�e�����p�����[�^
	struct ShadowParameter {
		glm::vec3 lightPos;	///< �e�𔭐������郉�C�g�̈ʒu
		glm::vec3 lightDir;	///< �e�𔭐������郉�C�g�̕���
		glm::vec3 lightUp;	///< �e�𔭐������郉�C�g�̏����
		glm::f32 near;		///< �`��͈͂̃j�A����
		glm::f32 far;		///< �`��͈͂̃t�@�[����
		glm::vec2 range;	///< �`��͈͂̕��ƍ���
	};

	//�V���h�E�p�����[�^�̎擾�E�ݒ菈��
	void Shadow(const ShadowParameter& param) { shadowParameter = param; }
	const ShadowParameter& Shadow() const { return shadowParameter; }

	//���C�g�p�����[�^�̎擾�E�ݒ菈��
	void Light(int index, const Uniform::PointLight& light);
	const Uniform::PointLight& Light(int index) const;

	//�����̎擾�E�ݒ菈��
	void AmbientLight(const glm::vec4& color);
	const glm::vec4& AmbientLight() const;

	//�P�x��臒l�̎擾�E�ݒ菈��
	void KeyValue(float k) { keyValue = k; }
	const float KeyValue() const { return keyValue; }

	//�\���E��\���̏��� ���ݖ��g�p
	void GroupVisibility(int groupId, int index, bool isVisible) { entityBuffer->GroupVisibility(groupId, index, isVisible); }
	bool GroupVisibility(int groupId, int index) const { return entityBuffer->GroupVisibility(groupId, index); }

	//�R���W�����̐ݒ�E�폜����
	void CollisionHandler(int gid0, int gid1);
	void ClearCollisionHandlerList();

	std::mt19937& Rand();
	const GamePad& GetGamePad() const;
	const TexturePtr& GetTexture(const char* filename) const;

	//�O���[�o���f�[�^�̎擾�ݒ菈��
	double& UserVariable(const char* name) { return userNumbers[name]; }

	//���C���J�����ݒ菈��
	void MainCamera(const std::shared_ptr<CameraComponent>& c) { mainCamera = c; }
	const std::shared_ptr<CameraComponent>& MainCamera() const { return mainCamera; }
	

	void TimeScale(float t) { timeScale = t; }
	float DeltaTime(float delta) const { return deltaTime; }
	float FPS() const { return fps; }
	void EnableShadow(bool b) { isEnableShadow = b; }
	void ToggleDrawOutline() { isDrawOutline = !isDrawOutline; }
	void SceneFadeStart(bool param) { isSceneFadeStart = param; }
	
private:

	GameEngine() = default;
	~GameEngine();
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;

	bool Update(float  delta);
	void UpdateFps();

	void Render();
	void RenderEntity() const;
	void RenderShadow() const;
	void RenderStencil() const;
	void RenderBloomEffect() const;
	void RenderFrameBuffer() const;

	void PushLevel();
	void PopLevel();
	void ClearLevel();

private:
	
	bool isInitalized = false;

	glm::vec2 windowSize;
	glm::vec2 viewportRect[2];

	//�ŏI�o�͗p�o�b�t�@
	BufferObject vbo, ibo;
	GLuint vao = 0;

	//��ʋP�x�𒲐����邽�߂�PBO
	BufferObject pbo[2];
	int pboIndexForWriting = -1;
	float luminanceScale = 1.0f;
	float keyValue = 0.18f;

	float deltaTime = 0;		///< �o�ߎ���

	//fps�J�E���g�p�ϐ�
	int bufferCount = 0;
	float fpsBuffer[60];
	float fps;

	bool isDrawOutline = false;		///< �A�E�g���C����`�悷�邩�ۂ�
	bool isEnableShadow = false;	///< �V���h�E�}�b�s���O���s����

	UniformBufferPtr uboLight;
	UniformBufferPtr uboPostEffect;
	std::unordered_map<std::string, Shader::ProgramPtr> shaderMap;

	static const int bloomBufferCount = 4;
	ShadowParameter shadowParameter;

	//�I�t�X�N���[���o�b�t�@�Q
	OffscreenBufferPtr offscreen;					/// �o�b�N�o�b�t�@	
	OffscreenBufferPtr offBloom[bloomBufferCount];	/// �u���[���o�b�t�@
	OffscreenBufferPtr offDepth;					/// �[�x�o�b�t�@				
	OffscreenBufferPtr offStencil;					/// �X�e���V���o�b�t�@

	//2D�\���p�X�v���C�g�����_��
	SpriteRenderer spriteRenderer;

	using TextureLevel = std::unordered_map<std::string, TexturePtr>;
	static const size_t minimalStackSize = 1;
	std::vector<TextureLevel> textureStack;

	Mesh::BufferPtr meshBuffer;
	Entity::BufferPtr entityBuffer;
	Font::Renderer fontRenderer;

	Uniform::LightData lightData;
	std::mt19937 rand;

	std::unordered_map<std::string, double> userNumbers;

	float timeScale = 1.0f;

	float sceneFadeTimer = 3.0f;
	bool isSceneFadeStart = false;

	//TODO : ����p���C���J�����R���|�[�l���g
	std::shared_ptr<CameraComponent> mainCamera;

};
