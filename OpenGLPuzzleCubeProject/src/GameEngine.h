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

#include "../Scene.h"

#define COMPONENT_TYPEPTR(type) std::shared_ptr<type>			///�V�[���R���|�[�l���g�̌^�p�}�N��


/**
*	�Q�[���G���W���N���X
*/
class GameEngine {
public:
	typedef std::function<void(double)> UpdateFuncType;	//�Q�[����Ԃ��X�V����֐��̌^

	static GameEngine& Instance();
	bool Init(int w, int h, const char* title);
	void Run();
	void UpdateFunc(const UpdateFuncType& func,float t=0);
	const UpdateFuncType& UpdateFunc() const;

	bool LoadTextureFromFile(const char* filename);
	bool LoadMeshFromFile(const char* filename);

	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);
	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, const char* normalName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);
	void RemoveEntity(Entity::Entity*);
	void RemoveAllEntity();
	void PushLevel();
	void PopLevel();
	void ClearLevel();


	///�e�����p�����[�^
	struct ShadowParameter {
		glm::vec3 lightPos;	///< �e�𔭐������郉�C�g�̈ʒu
		glm::vec3 lightDir;	///< �e�𔭐������郉�C�g�̕���
		glm::vec3 lightUp;	///< �e�𔭐������郉�C�g�̏����
		glm::f32 near;		///< �`��͈͂̃j�A����
		glm::f32 far;		///< �`��͈͂̃t�@�[����
		glm::vec2 range;	///< �`��͈͂̕��ƍ���
	};
	void Shadow(const ShadowParameter& param) {
		shadowParameter = param;
	}
	const ShadowParameter& Shadow() const { return shadowParameter; }

	void Light(int index, const Uniform::PointLight& light);
	const Uniform::PointLight& Light(int index) const;
	void AmbientLight(const glm::vec4& color);
	const glm::vec4& AmbientLight() const;
	void KeyValue(float k) { keyValue = k; }
	const float KeyValue() const { return keyValue; }

	void GroupVisibility(int groupId, int index, bool isVisible) {
		entityBuffer->GroupVisibility(groupId, index, isVisible);
	}
	bool GroupVisibility(int groupId, int index) const {
		return entityBuffer->GroupVisibility(groupId,index);
	}

	std::mt19937& Rand();
	const GamePad& GetGamePad() const;

	bool InitAudio(const char* acfPath, const char* acbPath, const char* awbPath, const char* dspBusName);
	void PlayAudio(int playerId, int cueId);
	void StopAudio(int playerId);

	void CollisionHandler(int gid0, int gid1);
	void ClearCollisionHandlerList();

	const TexturePtr& GetTexture(const char* filename) const;
	bool LoadFontFromFile(const char* filename) {
		return fontRenderer.LoadFromFile(filename);
	}
	bool AddString(const glm::vec2& pos, const char* str) {
		return fontRenderer.AddString(pos, str);
	}
	void FontScale(const glm::vec2& scale) { fontRenderer.Scale(scale); }
	void FontColor(const glm::vec4& color) { fontRenderer.Color(color); }

	double& UserVariable(const char* name) { return userNumbers[name]; }

	void ToggleDrawOutline() { isDrawOutline = !isDrawOutline; }

	void MainCamera(COMPONENT_TYPEPTR(CameraComponent)& c) { mainCamera = c; }
	const COMPONENT_TYPEPTR(CameraComponent)& MainCamera() const { return mainCamera; }
	
	template<typename T>
	Entity::Entity* FindEntityData() {
		return entityBuffer->FindEntityData<T>();
	}

	void TimeScale(float t) { timeScale = t; }

	void SceneFadeStart(bool param) {
		isSceneFadeStart = param; 
	}

	void PushScene(ScenePtr s) { SceneStack::Instance().Push(s); }

	void PopScene() { SceneStack::Instance().Pop(); }

	void ReplaceScene(ScenePtr s) { SceneStack::Instance().Replace(s); }
	
private:

	GameEngine() = default;
	~GameEngine();
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;
	void Update(float  delta);
	void Render();
	void RenderEntity() const;
	void RenderShadow() const;
	void RenderStencil() const;
	void RenderBloomEffect() const;
	void RenderOffscreen() const;

private:
	
	bool isInitalized = false;

	//TODO : ��ɃV�[���N���X�Ɉڍs�\��
	UpdateFuncType updateFunc;

	glm::vec2 windowSize;

	//�ŏI�o�͗p�o�b�t�@
	GLuint vbo = 0;
	GLuint ibo = 0;
	GLuint vao = 0;

	//��ʋP�x�𒲐����邽�߂�PBO
	BufferObject pbo[2];
	int pboIndexForWriting = -1;
	float luminanceScale = 1.0f;
	float keyValue = 0.18f;

	bool isDrawOutline = false;		///< �A�E�g���C����`�悷�邩�ۂ�

	UniformBufferPtr uboLight;
	UniformBufferPtr uboPostEffect;
	std::unordered_map<std::string, Shader::ProgramPtr> shaderMap;

	//�I�t�X�N���[���o�b�t�@�Q
	OffscreenBufferPtr offscreen;					/// �o�b�N�o�b�t�@
	static const int bloomBufferCount = 4;			
	OffscreenBufferPtr offBloom[bloomBufferCount];	/// �u���[���o�b�t�@
	OffscreenBufferPtr offDepth;					/// �[�x�o�b�t�@
	ShadowParameter shadowParameter;				

	OffscreenBufferPtr offStencil;					/// �X�e���V���o�b�t�@

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
	COMPONENT_TYPEPTR(CameraComponent) mainCamera;

	//SceneStack sceneStack;

};
