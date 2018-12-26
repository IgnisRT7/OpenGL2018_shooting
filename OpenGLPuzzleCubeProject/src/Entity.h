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

	/// 衝突解決ハンドラポインタ型
	typedef std::shared_ptr<Buffer> BufferPtr;	///< エンティティバッファ

												/// 衝突判定ハンドラ型
	using CollisionHandlerType = std::function<void(Entity&, Entity&) >;

	static const int maxGroupId = 31;	///< グループIDの最大値
										/**
										*	衝突判定形状
										*/
	struct CollisionData {
		glm::vec3 min;
		glm::vec3 max;
	};

	struct TransformData {
		glm::vec3 position;	///	座標
		glm::vec3 scale;	/// 拡縮
		glm::quat rotation;	/// 回転
	};

	/**
	* エンティティ.
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

		/// 状態更新関数型.
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

		Entity* parent = nullptr;				///< 親のエンティティ
		std::vector<Entity*> children;	///< 子のエンティティ

		int groupId = -1;			///< グループID
		Buffer* pBuffer = nullptr;	///< 生成元のBufferクラスへのポインタ

		TransformData transform;	///< トランスフォームデータ(ワールド空間)
		TransformData localTransform;///< トランスフォームデータ(ローカル空間)

		glm::mat4 transformMatrix;	

		glm::vec3 velocity;			///< 速度.
		glm::vec4 color = glm::vec4(1, 1, 1, 1);///< 色
		std::string name;
		UpdateFuncType updateFunc;	///< 状態更新関数.
		Mesh::MeshPtr mesh;			///< エンティティを描画するときに使われるメッシュデータ.
		TexturePtr texture[2];			///< エンティティを描画するときに使われるテクスチャ.
		Shader::ProgramPtr program; ///< エンティティを描画するときに使われるシェーダ.
		GLintptr uboOffset;			///< UBOのエンティティ用領域へのバイトオフセット.

		CollisionData colLocal;		///< ローカル座標系の衝突形状
		CollisionData colWorld;		///< ワールド座標系の衝突形状

		bool isActive = false; ///< アクティブなエンティティならtrue, 非アクティブならfalse.
	};

	/**
	*	エンティティバッファ
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

		///エンティティ用リンクリスト
		struct Link {
			void Insert(Link* e);
			void Remove();
			Link* prev = this;
			Link* next = this;
		};

		///リンク付きエンティティ
		struct LinkEntity : public Link, public Entity {};

		///リンク付きエンティティ配列の削除用関数オブジェクト
		struct EntityArrayDeleter { void operator()(LinkEntity* p) { delete[] p; } };

		std::unique_ptr<LinkEntity[], EntityArrayDeleter> buffer;	///< エンティティの配列
		size_t bufferSize;	///< エンティティの総数
		Link freeList;		///< 未使用のエンティティのリンクリスト
		Link activeList[maxGroupId + 1];	///< 使用中のエンティティのリンクリスト
		glm::u32 visibilityFlags[maxGroupId + 1];///<	
		GLsizeiptr ubSizePerEntity;	///< 各エンティティが使えるUniformBufferのバイト数
		UniformBufferPtr ubo;	///< エンティティ用UBO
		Link* itrUpdate = nullptr;	///< UpdateとRemoveEntityの相互作用に対応っするためのイテレータ
		Link* itrUpdateRhs = nullptr;

		struct CollisionHandlerInfo {
			int groupId[2];
			CollisionHandlerType handler;
		};
		std::vector<CollisionHandlerInfo> collisionHandlerList;

	};
}