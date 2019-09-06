/**
*	@file Entity.h
*	@brief	�V�[���œ����G���e�B�e�B�̃x�[�X
*	@author	takuya Yokoyama , tn-mai(�u�`���������)
*/

#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <functional>
#include <iostream>
#include <string>

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include "../Component/CameraComponent.h"


namespace Entity {

	class Entity;
	class Buffer;
	class EntityDataBase;

	using BufferPtr = std::shared_ptr<Buffer>;
	using EntityDataBasePtr = std::shared_ptr<EntityDataBase>;
	using FindEntityFunc = std::function<bool(Entity*)>;

	typedef std::shared_ptr<Buffer> BufferPtr;	///< �G���e�B�e�B�o�b�t�@

	static const int maxGroupId = 31;	///< �O���[�vID�̍ő�l

	/**
	*	�Փ˔���`��
	*/
	struct CollisionData {
		glm::vec3 min;
		glm::vec3 max;
	};

	struct TransformData {
		glm::vec3 position;	///	���W
		glm::vec3 scale;	/// �g�k
		glm::quat rotation;	/// ��]
	};

	/**
	* �G���e�B�e�B.
	*/
	class Entity
	{
		friend class Buffer;
		friend class EntityDataBase;

	public:
		
		/**
		*	���W�̐ݒ�
		*
		*	@param v	���W
		*/
		void Position(const glm::vec3& v) { transform.position = v; }

		/**
		*	���W�̎擾
		*
		*	@return ���W
		*/
		const glm::vec3& Position() const { return transform.position; }

		/**
		*	��]���̐ݒ�
		*
		*	@param q	��]���
		*/
		void Rotation(const glm::quat& q) { transform.rotation = q; }

		/**
		*	��]���̎擾
		*
		*	@return ��]���
		*/
		const glm::quat& Rotation() const { return transform.rotation; }

		/**
		*	�g�k���̐ݒ�
		*
		*	@param s	�g�k���
		*/
		void Scale(const glm::vec3& s) { transform.scale = s; }

		/**
		*	�g�k���̎擾
		*
		*	@return �g�k���
		*/
		const glm::vec3& Scale() const { return transform.scale; }

		/**
		*	�g�����X�t�H�[���f�[�^�̐ݒ�
		*
		*	@param t	�g�����X�t�H�[���f�[�^
		*/
		void Transform(const TransformData t) { transform = t; }

		/**
		*	�g�����X�t�H�[���f�[�^�̎擾
		*
		*	@return	�g�����X�t�H�[���f�[�^
		*/
		const TransformData Transform() const { return transform; }

		/**
		*	�ړ��E��]�E�g�k�s����擾����
		*
		*	@return TRS�s��
		*/
		glm::mat4 CalcModelMatrix() const;

		/**
		*	�G���e�B�e�B����f�[�^�̎擾
		*
		*	@return	�G���e�B�e�B����f�[�^
		*/
		EntityDataBasePtr EntityData() { return entityData; }

		/**
		*	�G���e�B�e�B����f�[�^�ɃL���X�g����
		*
		*	@tparam T �L���X�g����^
		*	@return nullptr	�����ȃL���X�g�f�[�^
		*	@return EntityDatabasePtr �L���X�g���ꂽ�f�[�^
		*/
		template<typename T>
		const std::shared_ptr<T> CastTo() {

			return std::dynamic_pointer_cast<T>(entityData);
		}

		/**
		*	�Փˌ`��f�[�^�̐ݒ�
		*
		*	@param c	�Փˌ`��f�[�^
		*/
		void Collision(const CollisionData& c) { colLocal = c; UpdateCollisionData(); }

		/**
		*	�Փˌ`��f�[�^�̎擾
		*
		*	@return �Փˌ`��f�[�^
		*/
		const CollisionData& Collision() const { return colLocal; }
		void UpdateCollisionData() {

			//this->colWorld.min = this->colLocal.min + this->transform.position;
			//this->colWorld.max = this->colLocal.max + this->transform.position;
		}

		/**
		*	�ړ��ʂ̐ݒ�
		*
		*	@param v	�ړ���
		*/
		void Velocity(const glm::vec3& v) { velocity = v; }

		/**
		*	�ړ��ʂ̎擾
		*
		*	@return �ړ���
		*/
		const glm::vec3& Velocity() const { return velocity; }

		/**
		*	�G���e�B�e�B�̃x�[�X�F�̐ݒ�
		*
		*	@param c	�F
		*/
		void Color(const glm::vec4& c) { color = c; }

		/**
		*	�G���e�B�e�B�̃x�[�X�F�̎擾
		*	
		*	@return �F
		*/
		const glm::vec4& Color() const { return color; }

		/**
		*	��������O���[�vID�̎擾
		*
		*	@return �O���[�vID
		*/
		int GroupId() const { return groupId; }

		/**
		*	�G���e�B�e�B��j������
		*
		*	���̊֐����Ăяo������́A�G���e�B�e�B�𑀍삵�Ă͂Ȃ�Ȃ�
		*/
		void Destroy();

		/**
		*	�V���h�E�}�b�v�̕`��؂�ւ�
		*
		*	@param b	�ؑփt���O
		*				tips true = �V���h�E�}�b�v�L����, false = �V���h�E�}�b�v������
		*/
		void CastShadow(bool b) { castShadow = b; }

		/**
		*	�V���h�E�}�b�v�؂�ւ����̎擾
		*	
		*	@return ���݂̐؂�ւ����
		*/
		bool CastShadow() const { return castShadow; }

		/**
		*	�X�e���V���}�b�v�̕`��؂�ւ�
		*
		*	@param b	�؂�ւ��t���O
		*				tips true = �X�e���V���}�b�v�L����, false = �X�e���V���}�b�v������
		*/
		void CastStencil(bool b) { castStencil = b; }

		/**
		*	�X�e���V���}�b�v�؂�ւ����̎擾
		*
		*	@return ���݂̐؂�ւ����
		*/
		bool CastStencil() const { return castStencil; }

		/**
		*	�X�e���V���J���[�̐ݒ�
		*
		*	@param v	�X�e���V���J���[�l
		*/
		void StencilColor(glm::vec4 v) { stencilColor = v; }

		/**
		*	�X�e���V���J���[�̎擾
		*
		*	@return	�X�e���V���J���[
		*/
		glm::vec4 StencilColor() const { return stencilColor; }

	private:

		Entity() = default;
		~Entity() = default;
		Entity(const Entity&) = default;
		Entity& operator=(const Entity&) = default;

	private:

		int groupId = -1;			///< �O���[�vID
		Buffer* pBuffer = nullptr;	///< ��������Buffer�N���X�ւ̃|�C���^

		TransformData transform;	///< �g�����X�t�H�[���f�[�^(���[���h���)

		glm::vec3 forward = glm::vec3(1, 0, 0);
		glm::vec3 velocity;			///< ���x.
		glm::vec4 color = glm::vec4(1, 1, 1, 1);///< �F
		std::string name;
		Mesh::MeshPtr mesh;			///< �G���e�B�e�B��`�悷��Ƃ��Ɏg���郁�b�V���f�[�^.
		TexturePtr texture[2];			///< �G���e�B�e�B��`�悷��Ƃ��Ɏg����e�N�X�`��.
		Shader::ProgramPtr program; ///< �G���e�B�e�B��`�悷��Ƃ��Ɏg����V�F�[�_.
		GLintptr uboOffset;			///< UBO�̃G���e�B�e�B�p�̈�ւ̃o�C�g�I�t�Z�b�g.

		CollisionData colLocal;		///< ���[�J�����W�n�̏Փˌ`��
		CollisionData colWorld;		///< ���[���h���W�n�̏Փˌ`��

		bool isActive = false; ///< �A�N�e�B�u�ȃG���e�B�e�B�Ȃ�true, ��A�N�e�B�u�Ȃ�false.
		bool castShadow = true;		///< ���̃G���e�B�e�B�͉e�𗎂Ƃ����ǂ���
		bool castStencil = false;	///< �X�e���V���o�b�t�@�ɕ`�悷�邩�ǂ���
		glm::vec4 stencilColor = glm::vec4(1);	///< �X�e���V���o�b�t�@��`�悷��ۂ̕`��F

		EntityDataBasePtr entityData;	///< �O���Ń��[�U�[����`����G���e�B�e�B�t���f�[�^

		//SceneComponentPtr& component;	///< �R���|�[�l���g�̃f�[�^
	};

	/**
	*	�G���e�B�e�B�o�b�t�@
	*/
	class Buffer {
	public:

		/**
		*	�G���e�B�e�B�o�b�t�@���쐬����
		*
		*	@param maxEntityCount	������G���e�B�e�B�̍ő吔
		*	@param ubSizePerEntity	�G���e�B�e�B���Ƃ�Uniform Buffer�̃o�C�g��
		*	@param bindingPoint		�G���e�B�e�B�pUBO�̃o�C���f�B���O�|�C���g
		*	@param ubName			�G���e�B�e�B�pUniform Buffer�̖��O
		*
		*	@return �쐬�����G���e�B�e�B�o�b�t�@�ւ̃|�C���^
		*/
		static BufferPtr Create(size_t maxEntityCount, GLsizeiptr ubSizePerEntity, int bindingPoint, const char* name);

		/**
		*	�G���e�B�e�B��ǉ�����
		*
		*	@param position	�G���e�B�e�B�̍��W
		*	@param mesh		�G���e�B�e�B�̕\���Ɏg�p���郁�b�V��
		*	@param texture	�G���e�B�e�B�̕\���Ɏg���e�N�X�`��
		*	@param program	�G���e�B�e�B�̕\���Ɏg�p����V�F�[�_�v���O����
		*	@param func		�G���e�B�e�B�̏�Ԃ��X�V����֐�(�܂��͊֐��I�u�W�F�N�g)
		*
		*	@return �ǉ������G���e�B�e�B�ւ̃|�C���^
		*			���ꂱ��ȏ�G���e�B�e�B��ǉ��ł��Ȃ��ꍇ��nullptr���Ԃ����
		*			��]��g�嗦��ݒ肷��ꍇ�͌̃|�C���^�o�R�ōs��
		*			���̃|�C���^���A�v���P�[�V�������ŕێ�����K�v�͂Ȃ�
		*/
		Entity* AddEntity(int groupId, const glm::vec3& pos, const Mesh::MeshPtr& m, const TexturePtr t[2], const Shader::ProgramPtr& p, EntityDataBasePtr ed);

		/**
		*	�G���e�B�e�B���폜����
		*
		*	@param �폜����G���e�B�e�B�̃|�C���^
		*/
		void RemoveEntity(Entity* entity);

		/**
		*	�S�ẴG���e�B�e�B���폜����
		*/
		void RemoveAllEntity();

		/**
		*	�A�N�e�B�u�ȃG���e�B�e�B�̏�Ԃ��X�V����
		*
		*	@param delta	�O��̍X�V����̌o�ߎ���
		*	@param matView	View�s��
		*	@param matProj	Projection�s��
		*	@param matDepthVP	View�s��(�[�x�o�b�t�@�p)
		*/
		void Update(float delta, const glm::mat4* matView, const glm::mat4& matProj, const glm::mat4& matDepthVP);

		/**
		*	�A�N�e�B�u�ȃG���e�B�e�B��`�悷��
		*
		*	@param meshBuffer �`��Ɏg�p���郁�b�V���o�b�t�@�ւ̃|�C���^
		*/
		void Draw(const Mesh::BufferPtr& meshBuffer) const;

		/**
		*	�A�N�e�B�u�ȃG���e�B�e�B��[�x����`�悷��
		*
		*	@param meshBuffer �`��Ɏg�p���郁�b�V���o�b�t�@�ւ̃|�C���^
		*	tips �V�F�[�_�̐ݒ�͊O���ōs������
		*/
		void DrawDepth(const Mesh::BufferPtr& meshBuffer)const;

		/**
		*	�X�e���V���o�b�t�@�̕`��
		*
		*	@param meshBuffer	���b�V���o�b�t�@
		*	@param prograrm		�g�p����v���O�����I�u�W�F�N�g
		*/
		void DrawStencil(const Mesh::BufferPtr& meshBuffer, const Shader::ProgramPtr& program)const;

		/**
		*	�\������O���[�vID�̐ݒ�
		*
		*	@param groupId		�K�p����GroupID
		*	@param cameraIndex	�K�p����J�����̃C���f�b�N�X�ԍ�
		*	@param isVisible	�\���t���O
		*/
		void GroupVisibility(int groupId, int cameraIndex, bool isVisible);

		/**
		*	�O���[�vID����\��/��\����Ԃ̎擾
		*
		*	@param groupId		�擾����O���[�vID
		*	@param cameraIndex	�擾����J�����̃C���f�b�N�X�ԍ�
		*
		*	@return �\���t���O
		*/
		bool GroupVisibility(int groupId, int cameraIndex) { return visibilityFlags[groupId] & (1U << cameraIndex); }

		/**
		*	�Փˉ����n���h����ݒ肷��
		*
		*	@param gid0		�ՓˑΏۂ̃O���[�vID
		*	@param gid1		�ՓˑΏۂ̃O���[�vID
		*	@param handler	�Փˉ����n���h��
		*
		*	�Փ˂��������Փ˃n���h�����Ăяo�����Ƃ��A
		*	��菬�����O���[�vID�����G���e�B�e�B�����ɓn�����B
		*	�R�R�Ŏw�肵���O���[�vID�̏����Ƃ͖��֌W�ł��邱�Ƃɒ���
		*
		*	CollisionHandler(10,1,Func)
		*	�Ƃ����R�[�h�Ńn���h����o�^�����Ƃ���A�Փ˂���������ƁA
		*	Func(�O���[�vID=1�̃G���e�B�e�B,�O���[�vID=10�̃G���e�B�e�B)
		*	�̂悤�ɌĂяo�����
		*/
		void CollisionHandler(int gid0, int gid1);

		/**
		*	�S�Ă̏Փˉ����n���h�����폜����
		*/
		void ClearCollisionHanderList();

		/**
		*	�G���e�B�e�B�̌������s��
		*
		*	@param name	�G���e�B�e�B��
		*	
		*	@return �������G���e�B�e�B(�����ꍇ��nullptr)
		*/
		Entity* FindEntity(FindEntityFunc f);

		/**
		*	�G���e�B�e�B����f�[�^����G���e�B�e�B�̌������s��
		*
		*	@tparam T �L���X�g����^
		*	@retval nullptr �G���e�B�e�B�����݂��Ȃ�����
		*	@retval Entity* �G���e�B�e�B�f�[�^
		*/
		template<typename T>
		Entity* FindEntityData() {

			for (int viewIndex = 0; viewIndex < Uniform::maxViewCount; ++viewIndex) {
				for (int groupId = 0; groupId <= maxGroupId; ++groupId) {

					for (Link* itr = activeList[groupId].next; itr != &activeList[groupId]; itr = itr->next) {
						
						LinkEntity* e = static_cast<LinkEntity*>(itr);
						Entity* e1 = dynamic_cast<Entity*>(e);

						std::shared_ptr<T> eData = nullptr;
						eData = e1->CastTo<T>();

						if (eData != nullptr) {
							//�w�肵���^�̃G���e�B�e�B�����݂��Ă���

							return e1;
						}

					}
				}
			}
			return nullptr;
		}


	private:

		Buffer() = default;
		~Buffer() = default;
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

	private:

		///�G���e�B�e�B�p�����N���X�g
		struct Link {

			/**
			*	�����N�I�u�W�F�N�g�������̎�O�ɒǉ�����
			*
			*	@param p	�ǉ����郊���N�I�u�W�F�N�g�ւ̃|�C���^
			*
			*	p���������̃����N���X�g����؂藣���A�����̎�O�ɒǉ�����
			*/
			void Insert(Link* e);

			/**
			*	�������g�������N���X�g����؂藣��
			*	�����͂ǂ��ɂ��ڑ�����Ă��Ȃ���ԂɂȂ�
			*/
			void Remove();

			Link* prev = this;
			Link* next = this;
		};

		///�����N�t���G���e�B�e�B
		struct LinkEntity : public Link, public Entity {};

		///�����N�t���G���e�B�e�B�z��̍폜�p�֐��I�u�W�F�N�g
		struct EntityArrayDeleter { void operator()(LinkEntity* p) { delete[] p; } };

		std::unique_ptr<LinkEntity[], EntityArrayDeleter> buffer;	///< �G���e�B�e�B�̔z��
		size_t bufferSize;					///  �G���e�B�e�B�̑���
		Link freeList;						/// ���g�p�̃G���e�B�e�B�̃����N���X�g
		Link activeList[maxGroupId + 1];	///�g�p���̃G���e�B�e�B�̃����N���X�g
		glm::u32 visibilityFlags[maxGroupId + 1];///
		GLsizeiptr ubSizePerEntity;			/// �e�G���e�B�e�B���g����UniformBuffer�̃o�C�g��
		UniformBufferPtr ubo;				/// �G���e�B�e�B�pUBO
		Link* itrUpdate = nullptr;			/// Update��RemoveEntity�̑��ݍ�p�ɑΉ������邽�߂̃C�e���[�^
		Link* itrUpdateRhs = nullptr;

		///�Փ˔���p�n���h��
		struct CollisionHandlerInfo {
			int groupId[2];
		};
		std::vector<CollisionHandlerInfo> collisionHandlerList;	/// 

	};

	//�G���e�B�e�B�ɕt��������̊Ǘ��N���X
	class EntityDataBase {
	public:

		EntityDataBase() = default;
		~EntityDataBase() = default;
		EntityDataBase(const EntityDataBase&) = delete;
		const EntityDataBase& operator=(const EntityDataBase&) = delete;

		/*
		*	��������
		*/
		virtual void Initialize() = 0;

		/*
		*	�X�V����
		*
		*	@param d	�o�ߎ���
		*/
		virtual void Update(float d) = 0;

		/*
		*	�_���[�W����
		*	
		*	@param p	�_���[�W�l
		*/
		virtual void Damage(float p) {}

		/*
		*	�Փ˔��菈��
		*
		*	@param e	�ՓˑΏۂ̃G���e�B�e�B
		*/
		virtual void CollisionEnter(Entity& e) {}

		/**
		*	�G���e�B�e�B�̃J���[�ݒ�
		*
		*	@param c	�F���
		*/
		void Color(glm::vec4& c) { entity->Color(c); }

		/**
		*	�G���e�B�e�B�̈ړ��ʐݒ�
		*
		*	@param v	�ړ���
		*/
		void Velocity(glm::vec3& v) { entity->Velocity(v); }

		/**
		*	�G���e�B�e�B�̍��W�ݒ�
		*
		*	@param p	���W���
		*/
		void Position(glm::vec3& p) { entity->Position(p); }

		/**
		*	�G���e�B�e�B�̐ݒ�
		*
		*	@param e	�K�p����G���e�B�e�B
		*/
		void SetEntity(Entity* e) { entity = e; }

	protected:

		Entity* entity;		/// ���䂷��G���e�B�e�B�f�[�^
		
	};
}