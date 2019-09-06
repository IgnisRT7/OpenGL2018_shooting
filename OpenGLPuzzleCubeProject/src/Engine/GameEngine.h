/**
*	@file GameEngine.h
*	@brief	�G���W���̐���p
*	@brief	Takuya Yokoyama , tn-mai
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

/**
*	�Q�[���G���W���N���X
*/
class GameEngine {
	friend class SceneStack;
public:

	/**
	*	�Q�[���G���W���̃C���X�^���X���擾����
	*
	*	@return �Q�[���G���W���̃C���X�^���X
	*/
	static GameEngine& Instance();

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
	*/
	bool Init(int w, int h, const char* title);

	/**
	*	�Q�[�������s����
	*/
	void Run();

	/**
	*	@copydoc Audio::Initialize
	*/
	bool InitAudio(const char* acfPath, const char* acbPath, const char* awbPath, const char* dspBusName);

	/**
	*	@copydoc Audio::Play
	*/
	void PlayAudio(int playerId, int cueId);

	/**
	*	@copydoc Audio::Stop
	*/
	void StopAudio(int playerId);

	/**
	*	@copydoc Audio::StopAll
	*/
	void StopAllAudio();

	/**
	*	�e�N�X�`����ǂݍ���
	*
	*	@param filename �e�N�X�`���t�@�C����
	*
	*	@retval true	�ǂݍ��ݐ���
	*	@retval false	�ǂݍ��ݎ��s
	*/
	bool LoadTextureFromFile(const char* filename);

	/**
	*	���b�V����ǂݍ���
	*
	*	@param filename ���b�V���t�@�C����
	*
	*	@retval true	�ǂݍ��ݐ���
	*	@retval false	�ǂݍ��ݎ��s
	*/
	bool LoadMeshFromFile(const char* filename);

	/**
	*	�t�H���g��ǂݍ���
	*
	*	@param filename �t�H���g�t�@�C����
	*
	*	@retval true	�ǂݍ��ݐ���
	*	@retval false	�ǂݍ��ݎ��s
	*/
	bool LoadFontFromFile(const char* filename);

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
	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);

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
	Entity::Entity* AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, const char* normalName, Entity::EntityDataBasePtr eData, const char* shader = nullptr);

	/**
	*	�X�v���C�g�̒ǉ�����
	*
	*	@param s	�ǉ�����X�v���C�g
	*/
	void AddSprite(Sprite& s);

	/**
	*	�G���e�B�e�B���폜����
	*
	*	@param �폜����G���e�B�e�B�̃|�C���^
	*/
	void RemoveEntity(Entity::Entity*);

	/**
	*	�S�ẴG���e�B�e�B���폜����
	*/
	void RemoveAllEntity();

	/*
	*	�G���e�B�e�B��������
	*
	*	@tparam T ��������^
	*	@retval nullptr �Y������G���e�B�e�B�f�[�^�����݂��Ȃ�����
	*	@retval Entity* �Y�������G���e�B�e�B�f�[�^
	*/
	template<typename T>
	Entity::Entity* FindEntityData() { return entityBuffer->FindEntityData<T>(); }

	/*
	*	�t�H���g�֘A�̏���
	*
	*	@param pos		�`��ʒu
	*	@param str		�`�悷�镶����
	*	@param isCenter	������̒��S��pos�̈ʒu�Ƃ��邩�ǂ���
	*
	*	@retval true	�ǉ�����
	*	@retval false	�ǉ����s
	*/
	bool AddString(const glm::vec2& pos, const char* str, bool isCenter = false) {
		return fontRenderer.AddString(pos, str, isCenter);
	}

	/**
	*	�����̊g�k���̐ݒ�
	*
	*	@param scale	�g�k���
	*/
	void FontScale(const glm::vec2& scale) { fontRenderer.Scale(scale); }

	/**
	*	�����̐F���̐ݒ�
	*	
	*	@param color	�F���
	*/
	void FontColor(const glm::vec4& color) { fontRenderer.Color(color); }

	/**
	*	�V�[�����v�b�V������
	*
	*	@param s	�V�[���I�u�W�F�N�g
	*/
	void PushScene(ScenePtr s);

	/**
	*	�V�[�����|�b�v����
	*/
	void PopScene();

	/**
	*	�V�[������芷������
	*
	*	@param s	�V�[���I�u�W�F�N�g
	*/
	void ReplaceScene(ScenePtr s);

	/**
	*	�t�H���g�ɐݒ肳��Ă���E�C���h�E�T�C�Y�̎擾
	*
	*	@return �E�C���h�E�T�C�Y
	*/
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

	/**
	*	�V���h�E�}�b�v�̃p�����[�^�̐ݒ�
	*
	*	@param s	�V���h�E�p�����[�^
	*/
	void Shadow(const ShadowParameter& s) { shadowParameter = s; }

	/**
	*	�V���h�E�}�b�v�̃p�����[�^�擾
	*
	*	@return �V���h�E�p�����[�^
	*/	
	const ShadowParameter& Shadow() const { return shadowParameter; }

	/**
	*	���C�g��ݒ肷��
	*
	*	@param indes	�ݒ肷�郉�C�g�̃C���f���N�X
	*	@param lilght	���C�g�f�[�^
	*/
	void Light(int index, const Uniform::PointLight& light);

	/**
	*	���C�g���擾����
	*
	*	@param inex �擾���郉�C�g�̃C���f�b�N�X
	*
	*	@return ���C�g�f�[�^
	*/
	const Uniform::PointLight& Light(int index) const;

	/**
	*	������ݒ肷��
	*
	*	@param color	�����̖��邳
	*/
	void AmbientLight(const glm::vec4& color);

	/**
	*	�������擾����
	*
	*	@return �����̖��邳
	*/
	const glm::vec4& AmbientLight() const;

	/**
	*	�P�x�𒲐����邽�߂̃L�[�l�̐ݒ�
	*
	*	@param k	�L�[�l
	*/
	void KeyValue(float k) { keyValue = k; }

	/**
	*	�P�x�𒲐߂��邽�߂̃L�[�l�̎擾
	*
	*	@return �L�[�l
	*/
	const float KeyValue() const { return keyValue; }
	
	/**
	*	�����t���O�̐ݒ�
	*
	*	@param groupId		�K�p����Ώۂ̃O���[�vID
	*	@param index		�J�����̃C���f�b�N�X
	*	@param isVisible	�����ǂ����̃t���O
	*/
	void GroupVisibility(int groupId, int index, bool isVisible) { entityBuffer->GroupVisibility(groupId, index, isVisible); }

	/**
	*	�����t���O�̎擾
	*
	*	@param groupId	�m�F����Ώۂ̃O���[�vID
	*	@param index	�J�����̃C���f�b�N�X
	*
	*	@retval true	���\
	*	@retval false	�s��
	*/
	bool GroupVisibility(int groupId, int index) const { return entityBuffer->GroupVisibility(groupId, index); }

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
	void CollisionHandler(int gid0, int gid1);

	/**
	*	�Փ˃n���h���̃��X�g���N���A����
	*/
	void ClearCollisionHandlerList();

	/**
	*	�����I�u�W�F�N�g���擾����
	*
	*	@return �����I�u�W�F�N�g
	*/
	std::mt19937& Rand();

	/**
	*	�Q�[���p�b�h�̏�Ԃ��擾����
	*/
	const GamePad& GetGamePad() const;

	/**
	*	�e�N�X�`���̎擾
	*
	*	@param filename �e�N�X�`����
	*
	*	@return true	�擾�����e�N�X�`��
	*	@return false	�_�~�[�̃e�N�X�`��
	*/
	const TexturePtr& GetTexture(const char* filename) const;

	//�O���[�o���f�[�^�̎擾�ݒ菈��
	double& UserVariable(const char* name) { return userNumbers[name]; }

	/**
	*	���C���J�����̐ݒ�
	*
	*	@param c	�J�����̃R���|�[�l���g
	*/
	void MainCamera(const std::shared_ptr<CameraComponent>& c) { mainCamera = c; }

	/**
	*	���C���J�����̎擾
	*
	*	@return �J�����R���|�[�l���g
	*/
	const std::shared_ptr<CameraComponent>& MainCamera() const { return mainCamera; }
	
	/**
	*	�A�X�y�N�g��ɉ������r���[�|�[�g�T�C�Y�̕ύX
	*
	*	@param width	��ʉ���
	*	@param height	��ʏc��
	*	@param aspect	�A�X�y�N�g��
	*/
	void CalculateViewPortByAspect(int width,int height,float aspect);

	//�V�F�[�_�擾����
	Shader::ProgramPtr& Shader(const char* pass) { return shaderMap[pass]; }

	/**
	*	�^�C���X�P�[���̐ݒ�
	*
	*	@param t	�^�C���X�P�[���l
	*/
	void TimeScale(float t) { timeScale = t; }

	/**
	*	�o�ߎ��Ԃ̎擾
	*
	*	@return �o�ߎ���
	*/
	float DeltaTime() const { return deltaTime; }

	/**
	*	FPS�̎擾
	*
	*	@retun FPS�l
	*/
	float FPS() const { return fps; }

	/**
	*	�V���h�E�`��̐؂�ւ�����
	*
	*	@param b	�ؑփt���O
	*				tips true = �V���h�E�`�悷��, false = �V���h�E�`�悵�Ȃ�
	*/
	void EnableShadow(bool b) { isEnableShadow = b; }

	/**
	*	�A�E�g���C���̕`��؂�ւ�����
	*/
	void ToggleDrawOutline() { isDrawOutline = !isDrawOutline; }

	/**
	*	�V�[���̈Ó]����
	*/
	void SceneFadeStart(bool param) { isSceneFadeStart = param; }

	
private:

	GameEngine() = default;
	~GameEngine();
	GameEngine(const GameEngine&) = delete;
	GameEngine& operator=(const GameEngine&) = delete;

	/**
	*	�Q�[���̏�Ԃ��X�V����
	*
	*	@param delta	�O��̍X�V����̌o�ߎ���(�b)
	*/
	bool Update(float  delta);

	/**
	*	Fps�̍X�V����
	*/
	void UpdateFps();

	/**
	*	�Q�[���̏�Ԃ�`�悷��
	*/
	void Render();

	/**
	*	�G���e�B�e�B��`�悷��
	*/
	void RenderEntity() const;

	/**
	*	�f�v�X�V���h�E�}�b�v��`�悷��
	*/
	void RenderShadow() const;

	/**
	*	�X�e���V���o�b�t�@��`�悷��
	*/
	void RenderStencil() const;

	/**
	*	�u���[���G�t�F�N�g��`�悷��
	*/
	void RenderBloomEffect() const;

	/**
	*	�I�t�X�N���[���o�b�t�@�ɕ`�悷��
	*/
	void RenderFrameBuffer() const;

	/**
	*	���\�[�X�X�^�b�N�ɐV�������\�[�X���x�����쐬����
	*/
	void PushLevel();

	/*
	*	���\�[�X�X�^�b�N�̖����̃��\�[�X���x������������
	*/
	void PopLevel();

	/**
	*	���\�[�X�X�^�b�N�̖����̃��\�[�X���x������̏�Ԃɂ���
	*/
	void ClearLevel();

private:
	
	bool isInitalized = false;

	glm::vec2 windowSize;
	glm::vec2 viewportRect[2];
	float windowAspect = 1.0f;

	//�ŏI�o�͗p�o�b�t�@
	BufferObject vbo, ibo;
	VertexArrayObject vao;

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
