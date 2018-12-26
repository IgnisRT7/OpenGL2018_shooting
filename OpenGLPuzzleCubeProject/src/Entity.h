/**
*	@file Entity.h
*/
#pragma once
#include <GL/glew.h>
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "UniformBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <functional>
#include <iostream>
#include <string>

namespace Entity {

	class Entity;
	class Buffer;
	using BufferPtr = std::shared_ptr<Buffer>;

	/// �Փˉ����n���h���|�C���^�^
	typedef std::shared_ptr<Buffer> BufferPtr;	///< �G���e�B�e�B�o�b�t�@

												/// �Փ˔���n���h���^
	using CollisionHandlerType = std::function<void(Entity&, Entity&) >;

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

	public:
		///transfom parameters getter and setter

		void Position(const glm::vec3& v) { transform.position = v; }
		const glm::vec3& Position() const { return transform.position; }
		void LocalPosition(const glm::vec3& v) { localTransform.position = v; }
		const glm::vec3& WorldPosition() const { return localTransform.position; }

		void Rotation(const glm::quat& q) { transform.rotation = q; }
		const glm::quat& Rotation() const { return transform.rotation; }
		void LocalRotation(const glm::vec3& q) { localTransform.rotation = q; }
		const glm::quat& LocalRotation() const { return localTransform.rotation; }

		void Scale(const glm::vec3& v) { transform.scale = v; }
		const glm::vec3& Scale() const { return transform.scale; }
		void LocalScale(const glm::vec3& v) { localTransform.scale = v; }
		const glm::vec3& LocalScale() const { return localTransform.scale; }

		void Transform(const TransformData t) { transform = t; }
		const TransformData Transform() const { return transform; }
		void LocalTransform(const TransformData t) { localTransform = t; }
		const TransformData LocalTransform() const { return localTransform; }

		glm::mat4 CalcModelMatrix();
		glm::mat4 TransformMatrix() const { return transformMatrix; }

	public:

		/// ��ԍX�V�֐��^.
		using UpdateFuncType = std::function<void(Entity&, double)>;

		void UpdateFunc(const UpdateFuncType& func) { updateFunc = func; }
		const UpdateFuncType& UpdateFunc() const { return updateFunc; }
		void Collision(const CollisionData& c) { colLocal = c; }
		const CollisionData& Collision() const { return colLocal; }

		void Velocity(const glm::vec3& v) { velocity = v; }
		const glm::vec3& Velocity() const { return velocity; }
		void Color(const glm::vec4& c) { color = c; }
		const glm::vec4& Color() const { return color; }

		int GroupId() const { return groupId; }
		void Destroy();

		void AddChild(Entity& e);

	private:
		Entity() = default;
		~Entity() = default;
		Entity(const Entity&) = default;
		Entity& operator=(const Entity&) = default;

	private:

		Entity* parent = nullptr;				///< �e�̃G���e�B�e�B
		std::vector<Entity*> children;	///< �q�̃G���e�B�e�B

		int groupId = -1;			///< �O���[�vID
		Buffer* pBuffer = nullptr;	///< ��������Buffer�N���X�ւ̃|�C���^

		TransformData transform;	///< �g�����X�t�H�[���f�[�^(���[���h���)
		TransformData localTransform;///< �g�����X�t�H�[���f�[�^(���[�J�����)

		glm::mat4 transformMatrix;	

		glm::vec3 velocity;			///< ���x.
		glm::vec4 color = glm::vec4(1, 1, 1, 1);///< �F
		std::string name;
		UpdateFuncType updateFunc;	///< ��ԍX�V�֐�.
		Mesh::MeshPtr mesh;			///< �G���e�B�e�B��`�悷��Ƃ��Ɏg���郁�b�V���f�[�^.
		TexturePtr texture[2];			///< �G���e�B�e�B��`�悷��Ƃ��Ɏg����e�N�X�`��.
		Shader::ProgramPtr program; ///< �G���e�B�e�B��`�悷��Ƃ��Ɏg����V�F�[�_.
		GLintptr uboOffset;			///< UBO�̃G���e�B�e�B�p�̈�ւ̃o�C�g�I�t�Z�b�g.

		CollisionData colLocal;		///< ���[�J�����W�n�̏Փˌ`��
		CollisionData colWorld;		///< ���[���h���W�n�̏Փˌ`��

		bool isActive = false; ///< �A�N�e�B�u�ȃG���e�B�e�B�Ȃ�true, ��A�N�e�B�u�Ȃ�false.
	};

	/**
	*	�G���e�B�e�B�o�b�t�@
	*/
	class Buffer {
	public:
		static BufferPtr Create(size_t maxEntityCount, GLsizeiptr ubSizePerEntity, int bindingPoint, const char* name);

		Entity* AddEntity(int groupId, const glm::vec3& pos, const Mesh::MeshPtr& m, const TexturePtr t[2], const Shader::ProgramPtr& p, Entity::UpdateFuncType func);
		void RemoveEntity(Entity* entity);
		void RemoveAllEntity();
		void Update(double delta, const glm::mat4* matView, const glm::mat4& matProj, const glm::mat4& matDepthVP);

	//	Entity& GetEntityByName(std::string name);

		void Draw(const Mesh::BufferPtr& meshBuffer) const;
		void DrawDepth(const Mesh::BufferPtr& meshBuffer)const;
		void GroupVisibility(int groupId, int cameraIndex, bool isVisible) {
			if (isVisible) {
				visibilityFlags[groupId] |= (1U << cameraIndex);
			}
			else {
				visibilityFlags[groupId] &= ~(1U << cameraIndex);
			}
		}
		bool GroupVisibility(int groupId, int cameraIndex) {
			return visibilityFlags[groupId] & (1U << cameraIndex);
		}

		void CollisionHandler(int gid0, int gid1, CollisionHandlerType hander);
		const CollisionHandlerType& CollisionHandler(int gid0, int gid1) const;
		void ClearCollisionHanderList();

	private:

		Buffer() = default;
		~Buffer() = default;
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

	private:

		///�G���e�B�e�B�p�����N���X�g
		struct Link {
			void Insert(Link* e);
			void Remove();
			Link* prev = this;
			Link* next = this;
		};

		///�����N�t���G���e�B�e�B
		struct LinkEntity : public Link, public Entity {};

		///�����N�t���G���e�B�e�B�z��̍폜�p�֐��I�u�W�F�N�g
		struct EntityArrayDeleter { void operator()(LinkEntity* p) { delete[] p; } };

		std::unique_ptr<LinkEntity[], EntityArrayDeleter> buffer;	///< �G���e�B�e�B�̔z��
		size_t bufferSize;	///< �G���e�B�e�B�̑���
		Link freeList;		///< ���g�p�̃G���e�B�e�B�̃����N���X�g
		Link activeList[maxGroupId + 1];	///< �g�p���̃G���e�B�e�B�̃����N���X�g
		glm::u32 visibilityFlags[maxGroupId + 1];///<	
		GLsizeiptr ubSizePerEntity;	///< �e�G���e�B�e�B���g����UniformBuffer�̃o�C�g��
		UniformBufferPtr ubo;	///< �G���e�B�e�B�pUBO
		Link* itrUpdate = nullptr;	///< Update��RemoveEntity�̑��ݍ�p�ɑΉ������邽�߂̃C�e���[�^
		Link* itrUpdateRhs = nullptr;

		struct CollisionHandlerInfo {
			int groupId[2];
			CollisionHandlerType handler;
		};
		std::vector<CollisionHandlerInfo> collisionHandlerList;

	};
}