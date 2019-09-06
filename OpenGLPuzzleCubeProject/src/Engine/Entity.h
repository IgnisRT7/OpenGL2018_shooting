/**
*	@file Entity.h
*	@brief	シーンで動くエンティティのベース
*	@author	takuya Yokoyama , tn-mai(講義資料製作者)
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

	typedef std::shared_ptr<Buffer> BufferPtr;	///< エンティティバッファ

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
		friend class EntityDataBase;

	public:
		
		/**
		*	座標の設定
		*
		*	@param v	座標
		*/
		void Position(const glm::vec3& v) { transform.position = v; }

		/**
		*	座標の取得
		*
		*	@return 座標
		*/
		const glm::vec3& Position() const { return transform.position; }

		/**
		*	回転情報の設定
		*
		*	@param q	回転情報
		*/
		void Rotation(const glm::quat& q) { transform.rotation = q; }

		/**
		*	回転情報の取得
		*
		*	@return 回転情報
		*/
		const glm::quat& Rotation() const { return transform.rotation; }

		/**
		*	拡縮情報の設定
		*
		*	@param s	拡縮情報
		*/
		void Scale(const glm::vec3& s) { transform.scale = s; }

		/**
		*	拡縮情報の取得
		*
		*	@return 拡縮情報
		*/
		const glm::vec3& Scale() const { return transform.scale; }

		/**
		*	トランスフォームデータの設定
		*
		*	@param t	トランスフォームデータ
		*/
		void Transform(const TransformData t) { transform = t; }

		/**
		*	トランスフォームデータの取得
		*
		*	@return	トランスフォームデータ
		*/
		const TransformData Transform() const { return transform; }

		/**
		*	移動・回転・拡縮行列を取得する
		*
		*	@return TRS行列
		*/
		glm::mat4 CalcModelMatrix() const;

		/**
		*	エンティティ制御データの取得
		*
		*	@return	エンティティ制御データ
		*/
		EntityDataBasePtr EntityData() { return entityData; }

		/**
		*	エンティティ制御データにキャストする
		*
		*	@tparam T キャストする型
		*	@return nullptr	無効なキャストデータ
		*	@return EntityDatabasePtr キャストされたデータ
		*/
		template<typename T>
		const std::shared_ptr<T> CastTo() {

			return std::dynamic_pointer_cast<T>(entityData);
		}

		/**
		*	衝突形状データの設定
		*
		*	@param c	衝突形状データ
		*/
		void Collision(const CollisionData& c) { colLocal = c; UpdateCollisionData(); }

		/**
		*	衝突形状データの取得
		*
		*	@return 衝突形状データ
		*/
		const CollisionData& Collision() const { return colLocal; }
		void UpdateCollisionData() {

			//this->colWorld.min = this->colLocal.min + this->transform.position;
			//this->colWorld.max = this->colLocal.max + this->transform.position;
		}

		/**
		*	移動量の設定
		*
		*	@param v	移動量
		*/
		void Velocity(const glm::vec3& v) { velocity = v; }

		/**
		*	移動量の取得
		*
		*	@return 移動量
		*/
		const glm::vec3& Velocity() const { return velocity; }

		/**
		*	エンティティのベース色の設定
		*
		*	@param c	色
		*/
		void Color(const glm::vec4& c) { color = c; }

		/**
		*	エンティティのベース色の取得
		*	
		*	@return 色
		*/
		const glm::vec4& Color() const { return color; }

		/**
		*	所属するグループIDの取得
		*
		*	@return グループID
		*/
		int GroupId() const { return groupId; }

		/**
		*	エンティティを破棄する
		*
		*	この関数を呼び出した後は、エンティティを操作してはならない
		*/
		void Destroy();

		/**
		*	シャドウマップの描画切り替え
		*
		*	@param b	切替フラグ
		*				tips true = シャドウマップ有効化, false = シャドウマップ無効化
		*/
		void CastShadow(bool b) { castShadow = b; }

		/**
		*	シャドウマップ切り替え情報の取得
		*	
		*	@return 現在の切り替え状態
		*/
		bool CastShadow() const { return castShadow; }

		/**
		*	ステンシルマップの描画切り替え
		*
		*	@param b	切り替えフラグ
		*				tips true = ステンシルマップ有効化, false = ステンシルマップ無効化
		*/
		void CastStencil(bool b) { castStencil = b; }

		/**
		*	ステンシルマップ切り替え情報の取得
		*
		*	@return 現在の切り替え状態
		*/
		bool CastStencil() const { return castStencil; }

		/**
		*	ステンシルカラーの設定
		*
		*	@param v	ステンシルカラー値
		*/
		void StencilColor(glm::vec4 v) { stencilColor = v; }

		/**
		*	ステンシルカラーの取得
		*
		*	@return	ステンシルカラー
		*/
		glm::vec4 StencilColor() const { return stencilColor; }

	private:

		Entity() = default;
		~Entity() = default;
		Entity(const Entity&) = default;
		Entity& operator=(const Entity&) = default;

	private:

		int groupId = -1;			///< グループID
		Buffer* pBuffer = nullptr;	///< 生成元のBufferクラスへのポインタ

		TransformData transform;	///< トランスフォームデータ(ワールド空間)

		glm::vec3 forward = glm::vec3(1, 0, 0);
		glm::vec3 velocity;			///< 速度.
		glm::vec4 color = glm::vec4(1, 1, 1, 1);///< 色
		std::string name;
		Mesh::MeshPtr mesh;			///< エンティティを描画するときに使われるメッシュデータ.
		TexturePtr texture[2];			///< エンティティを描画するときに使われるテクスチャ.
		Shader::ProgramPtr program; ///< エンティティを描画するときに使われるシェーダ.
		GLintptr uboOffset;			///< UBOのエンティティ用領域へのバイトオフセット.

		CollisionData colLocal;		///< ローカル座標系の衝突形状
		CollisionData colWorld;		///< ワールド座標系の衝突形状

		bool isActive = false; ///< アクティブなエンティティならtrue, 非アクティブならfalse.
		bool castShadow = true;		///< このエンティティは影を落とすかどうか
		bool castStencil = false;	///< ステンシルバッファに描画するかどうか
		glm::vec4 stencilColor = glm::vec4(1);	///< ステンシルバッファを描画する際の描画色

		EntityDataBasePtr entityData;	///< 外部でユーザーが定義するエンティティ付属データ

		//SceneComponentPtr& component;	///< コンポーネントのデータ
	};

	/**
	*	エンティティバッファ
	*/
	class Buffer {
	public:

		/**
		*	エンティティバッファを作成する
		*
		*	@param maxEntityCount	扱えるエンティティの最大数
		*	@param ubSizePerEntity	エンティティごとのUniform Bufferのバイト数
		*	@param bindingPoint		エンティティ用UBOのバインディングポイント
		*	@param ubName			エンティティ用Uniform Bufferの名前
		*
		*	@return 作成したエンティティバッファへのポインタ
		*/
		static BufferPtr Create(size_t maxEntityCount, GLsizeiptr ubSizePerEntity, int bindingPoint, const char* name);

		/**
		*	エンティティを追加する
		*
		*	@param position	エンティティの座標
		*	@param mesh		エンティティの表示に使用するメッシュ
		*	@param texture	エンティティの表示に使うテクスチャ
		*	@param program	エンティティの表示に使用するシェーダプログラム
		*	@param func		エンティティの状態を更新する関数(または関数オブジェクト)
		*
		*	@return 追加したエンティティへのポインタ
		*			これこれ以上エンティティを追加できない場合はnullptrが返される
		*			回転や拡大率を設定する場合は個のポインタ経由で行う
		*			このポインタをアプリケーション側で保持する必要はない
		*/
		Entity* AddEntity(int groupId, const glm::vec3& pos, const Mesh::MeshPtr& m, const TexturePtr t[2], const Shader::ProgramPtr& p, EntityDataBasePtr ed);

		/**
		*	エンティティを削除する
		*
		*	@param 削除するエンティティのポインタ
		*/
		void RemoveEntity(Entity* entity);

		/**
		*	全てのエンティティを削除する
		*/
		void RemoveAllEntity();

		/**
		*	アクティブなエンティティの状態を更新する
		*
		*	@param delta	前回の更新からの経過時間
		*	@param matView	View行列
		*	@param matProj	Projection行列
		*	@param matDepthVP	View行列(深度バッファ用)
		*/
		void Update(float delta, const glm::mat4* matView, const glm::mat4& matProj, const glm::mat4& matDepthVP);

		/**
		*	アクティブなエンティティを描画する
		*
		*	@param meshBuffer 描画に使用するメッシュバッファへのポインタ
		*/
		void Draw(const Mesh::BufferPtr& meshBuffer) const;

		/**
		*	アクティブなエンティティを深度情報を描画する
		*
		*	@param meshBuffer 描画に使用するメッシュバッファへのポインタ
		*	tips シェーダの設定は外部で行うこと
		*/
		void DrawDepth(const Mesh::BufferPtr& meshBuffer)const;

		/**
		*	ステンシルバッファの描画
		*
		*	@param meshBuffer	メッシュバッファ
		*	@param prograrm		使用するプログラムオブジェクト
		*/
		void DrawStencil(const Mesh::BufferPtr& meshBuffer, const Shader::ProgramPtr& program)const;

		/**
		*	表示するグループIDの設定
		*
		*	@param groupId		適用するGroupID
		*	@param cameraIndex	適用するカメラのインデックス番号
		*	@param isVisible	表示フラグ
		*/
		void GroupVisibility(int groupId, int cameraIndex, bool isVisible);

		/**
		*	グループIDから表示/非表示状態の取得
		*
		*	@param groupId		取得するグループID
		*	@param cameraIndex	取得するカメラのインデックス番号
		*
		*	@return 表示フラグ
		*/
		bool GroupVisibility(int groupId, int cameraIndex) { return visibilityFlags[groupId] & (1U << cameraIndex); }

		/**
		*	衝突解決ハンドラを設定する
		*
		*	@param gid0		衝突対象のグループID
		*	@param gid1		衝突対象のグループID
		*	@param handler	衝突解決ハンドラ
		*
		*	衝突が発生し衝突ハンドラが呼び出されるとき、
		*	より小さいグループIDを持つエンティティから先に渡される。
		*	ココで指定したグループIDの順序とは無関係であることに注意
		*
		*	CollisionHandler(10,1,Func)
		*	というコードでハンドラを登録したとする、衝突が発生すると、
		*	Func(グループID=1のエンティティ,グループID=10のエンティティ)
		*	のように呼び出される
		*/
		void CollisionHandler(int gid0, int gid1);

		/**
		*	全ての衝突解決ハンドラを削除する
		*/
		void ClearCollisionHanderList();

		/**
		*	エンティティの検索を行う
		*
		*	@param name	エンティティ名
		*	
		*	@return 見つけたエンティティ(無い場合はnullptr)
		*/
		Entity* FindEntity(FindEntityFunc f);

		/**
		*	エンティティ制御データからエンティティの検索を行う
		*
		*	@tparam T キャストする型
		*	@retval nullptr エンティティが存在しなかった
		*	@retval Entity* エンティティデータ
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
							//指定した型のエンティティが存在していた

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

		///エンティティ用リンクリスト
		struct Link {

			/**
			*	リンクオブジェクトを自分の手前に追加する
			*
			*	@param p	追加するリンクオブジェクトへのポインタ
			*
			*	pを所属元のリンクリストから切り離し、自分の手前に追加する
			*/
			void Insert(Link* e);

			/**
			*	自分自身をリンクリストから切り離す
			*	自分はどこにも接続されていない状態になる
			*/
			void Remove();

			Link* prev = this;
			Link* next = this;
		};

		///リンク付きエンティティ
		struct LinkEntity : public Link, public Entity {};

		///リンク付きエンティティ配列の削除用関数オブジェクト
		struct EntityArrayDeleter { void operator()(LinkEntity* p) { delete[] p; } };

		std::unique_ptr<LinkEntity[], EntityArrayDeleter> buffer;	///< エンティティの配列
		size_t bufferSize;					///  エンティティの総数
		Link freeList;						/// 未使用のエンティティのリンクリスト
		Link activeList[maxGroupId + 1];	///使用中のエンティティのリンクリスト
		glm::u32 visibilityFlags[maxGroupId + 1];///
		GLsizeiptr ubSizePerEntity;			/// 各エンティティが使えるUniformBufferのバイト数
		UniformBufferPtr ubo;				/// エンティティ用UBO
		Link* itrUpdate = nullptr;			/// UpdateとRemoveEntityの相互作用に対応っするためのイテレータ
		Link* itrUpdateRhs = nullptr;

		///衝突判定用ハンドラ
		struct CollisionHandlerInfo {
			int groupId[2];
		};
		std::vector<CollisionHandlerInfo> collisionHandlerList;	/// 

	};

	//エンティティに付属する情報の管理クラス
	class EntityDataBase {
	public:

		EntityDataBase() = default;
		~EntityDataBase() = default;
		EntityDataBase(const EntityDataBase&) = delete;
		const EntityDataBase& operator=(const EntityDataBase&) = delete;

		/*
		*	期化処理
		*/
		virtual void Initialize() = 0;

		/*
		*	更新処理
		*
		*	@param d	経過時間
		*/
		virtual void Update(float d) = 0;

		/*
		*	ダメージ処理
		*	
		*	@param p	ダメージ値
		*/
		virtual void Damage(float p) {}

		/*
		*	衝突判定処理
		*
		*	@param e	衝突対象のエンティティ
		*/
		virtual void CollisionEnter(Entity& e) {}

		/**
		*	エンティティのカラー設定
		*
		*	@param c	色情報
		*/
		void Color(glm::vec4& c) { entity->Color(c); }

		/**
		*	エンティティの移動量設定
		*
		*	@param v	移動量
		*/
		void Velocity(glm::vec3& v) { entity->Velocity(v); }

		/**
		*	エンティティの座標設定
		*
		*	@param p	座標情報
		*/
		void Position(glm::vec3& p) { entity->Position(p); }

		/**
		*	エンティティの設定
		*
		*	@param e	適用するエンティティ
		*/
		void SetEntity(Entity* e) { entity = e; }

	protected:

		Entity* entity;		/// 制御するエンティティデータ
		
	};
}