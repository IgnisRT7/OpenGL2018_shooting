/**
*	@file Entity.cpp
*/
#include "Entity.h"
#include "Uniform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>

/**
*	エンティティに関するコードを格納する名前空間
*/
namespace Entity {

	/**
	*	VertexDataをUBOに転送する
	*
	*	@param entity
	*	@param ubo
	*	@param matViewProjection
	*	@param viewFlags
	*/
	void UpdateUniformVertexData(Entity& entity, void*ubo, const glm::mat4* matVP, const glm::mat4& matDepthVP, glm::u32 viewFlags) {

		Uniform::VertexData data;

		//TODO: ここで行列計算させずに別で更新処理を設ける。計算済みの行列を受け取るだけにする
//		data.matModel = entity.TransformMatrix();
		data.matModel = entity.CalcModelMatrix();
		data.matNormal = glm::mat4_cast(entity.Rotation());

		//ビュー x 射影 行列の計算
		for (int i = 0; i < Uniform::maxViewCount; ++i) {
			//viewFlag == falseの時は更新しない
			if (viewFlags & (1U << i)) {
				data.matMVP[i] = matVP[i] * data.matModel;
			}
		}

		data.matDepthMVP = matDepthVP * data.matModel;
		data.color = entity.Color();
		memcpy(ubo, &data, sizeof(data));
	}

	/**
	*	移動・回転・拡縮行列を取得する
	*
	*	@return TRS行列
	*/
	glm::mat4 Entity::CalcModelMatrix()
	{
		//親のトランスフォーム行列の取得(無い場合は単位行列)
		glm::mat4 parentMatrix = glm::mat4(1);
		if (parent) {
			parentMatrix = parent->transformMatrix;
		}

		///トランスフォーム行列計算処理
		if (0) {
			//old

			//TODO:仕様変更のため後々廃止
//			const glm::mat4 tw = glm::translate(glm::mat4(), transform.position);
//			const glm::mat4 rw = glm::mat4_cast(transform.rotation);
//			const glm::mat4 sw = glm::scale(glm::mat4(), transform.scale);
//			transformMatrix = tw * rw * sw;
		}
		else{
			//new

			//自身(ローカル空間)のトランスフォーム取得と計算
			const glm::mat4 t = glm::translate(glm::mat4(), localTransform.position);
			const glm::mat4 r = glm::mat4_cast(localTransform.rotation);
			const glm::mat4 s = glm::scale(glm::mat4(), localTransform.scale);

			auto localMatrix = t * r * s;
			//ワールド空間のトランスフォーム行列計算
			transformMatrix = localMatrix * parentMatrix;
		}



		//
		for (auto& e : children) {

			e->CalcModelMatrix();
		}

		//TODO: 仕様変更のため後々廃止
		return transformMatrix;
	}
	/**
	*	エンティティの取り付け処理
	*/
	void Entity::AddChild(Entity* e){

		e->parent = this;
		children.push_back(e);
	//	node->UpdateTransform();
	}

	/**
	*	指定した子エンティティを取り除く
	*
	*	@param c 取り除くエンティティ
	*/
	void Entity::RemoveChild(Entity* c){

		//自身の子に取り除く対象cがあるかどうか調べる
		auto itr = std::find_if(children.begin(), children.end(), 
			[c](const Entity* e) { return e == c; });

		if (itr != children.end()) {
			(*itr)->parent = nullptr;	//子エンティティからの参照切り離し
			*itr = nullptr;				//自身の参照の切り離し
		}
	}



	/**
	*	エンティティを破棄する
	*
	*	この関数を呼び出した後は、エンティティを操作してはならない
	*/
	void Entity::Destroy() {

		if (pBuffer) {
			pBuffer->RemoveEntity(this);
		}
	}

	/**
	*	ユーザー定義用の更新処理
	*
	*	@param dt 経過時間
	*/
	void Entity::Update(float dt){

		//デフォルトでは何もしない
	}
	
	/*
	*	エンティティのシステム更新処理
	*/
	void Entity::UpdateRecursive(float dt){

		//std::cout << "0x" << std::hex <<  this  << std::endl;

		//ユーザー定義更新処理呼び出し
		Update(dt);

		//子の更新処理呼び出し
		for (auto e : children) {

			e->UpdateRecursive(dt);
		}
	}

	/**
	*	自分自身をリンクリストから切り離す
	*
	*	自分はどこにも接続されていない状態になる
	*/
	void Buffer::Link::Remove(){
		next->prev = prev;
		prev->next = next;
		prev = this;
		next = this;
	}

	/**
	*	リンクオブジェクトを自分の手前に追加する
	*
	*	@param p	追加するリンクオブジェクトへのポインタ
	*
	*	pを所属元のリンクリストから切り離し、自分の手前に追加する
	*/
	void Buffer::Link::Insert(Link* p)
	{
		p->Remove();
		p->prev = prev;
		p->next = this;
		prev->next = p;
		prev = p;
	}

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
	BufferPtr Buffer::Create(size_t maxEntityCount, GLsizeiptr ubSizePerEntity, int bindingPoint, const char* ubName) {


		struct Impl : Buffer { Impl() {} ~Impl() {} };
		BufferPtr p = std::make_shared<Impl>();
		if (!p) {
			std::cerr << "WARNING in Entity::Buffer::Create: バッファの作成に失敗." << std::endl;
			return {};
		}

		p->ubo = UniformBuffer::Create(maxEntityCount * ubSizePerEntity, bindingPoint, ubName);
		p->buffer.reset(new LinkEntity[maxEntityCount]);
		if (!p->ubo || !p->buffer) {
			std::cerr << "WARNING in Entity::Buffer::Create: バッファの作成に失敗." << std::endl;
			return {};
		}

		p->bufferSize = maxEntityCount;
		p->ubSizePerEntity = ubSizePerEntity;
		GLintptr offset = 0;
		const LinkEntity* const end = &p->buffer[maxEntityCount];
		for (LinkEntity* itr = &p->buffer[0]; itr != end; ++itr) {
			itr->uboOffset = offset;
			itr->pBuffer = p.get();
			p->freeList.Insert(itr);
			offset += ubSizePerEntity;
		}
		p->collisionHandlerList.reserve(maxGroupId);

		for (auto& e : p->visibilityFlags) {
			e = 1;	
		}

		return p;
	}

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
	Entity* Buffer::AddEntity(int groupId, const glm::vec3& position, const Mesh::MeshPtr& mesh, const TexturePtr texture[2], const Shader::ProgramPtr& program, Entity::UpdateFuncType func) {

		if (freeList.prev == freeList.next) {
			std::cerr << "WARNING in Entity::Buffer::AddEntity: "
				"空きエンティティがありません." << std::endl;
			return nullptr;
		}

		if (groupId <0 || groupId > maxGroupId) {

			std::cerr << "ERROR in Entity::Buffer::AddEntity: 範囲外のグループID(" << groupId << ")が渡されました。\nグループIDは0～" << maxGroupId << "でなければなりません。" << std::endl;
			return nullptr;
		}

		LinkEntity* entity = static_cast<LinkEntity*>(freeList.prev);
		activeList[groupId].Insert(entity);

		//エンティティデータの初期化処理
		entity->name = mesh->Name();
		entity->groupId = groupId;
		entity->transform = TransformData({ position,glm::vec3(1,1,1),glm::quat() });
		entity->localTransform = entity->transform;
		entity->velocity = glm::vec3();
		entity->color = glm::vec4(1);
		entity->mesh = mesh;
		entity->texture[0] = texture[0];
		entity->texture[1] = texture[1];
		entity->program = program;
		entity->updateFunc = func;
		entity->isActive = true;

		//entity->Init();

		return entity;
	}

	/**
	*	エンティティを削除する
	*
	*	@param 削除するエンティティのポインタ
	*/
	void Buffer::RemoveEntity(Entity* entity) {

		if (!entity || !entity->isActive) {
			std::cerr << "WARNING in Entity::Buffer::RemoveEntity: "
				"非アクティブなエンティティを削除しようとしました." << std::endl;
			return;
		}
		LinkEntity* p = static_cast<LinkEntity*>(entity);
		if (p < &buffer[0] || p >= &buffer[bufferSize]) {
			std::cerr << "WARNING in Entity::Buffer::RemoveEntity: "
				"異なるバッファから取得したエンティティを削除しようとしました." << std::endl;
			return;
		}
		if (p == itrUpdate) {
			itrUpdate = p->prev;
		}
		if (p == itrUpdateRhs) {
			itrUpdateRhs = p->prev;
		}

		freeList.Insert(p);
		p->mesh.reset();
		for (auto& e : p->texture) {
			e.reset();
		}
		p->program.reset();
		p->updateFunc = nullptr;
		p->isActive = false;
	}

	/**
	*	全てのエンティティを削除する
	*/
	void Buffer::RemoveAllEntity() {

		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
			while (activeList[groupId].next != &activeList[groupId]) {
				RemoveEntity(static_cast<LinkEntity*>(activeList[groupId].next));
			}
		}
	}

	/**
	*	矩形同士の衝突判定
	*/
	bool HasCollision(const CollisionData& lhs, const CollisionData& rhs) {
		if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x)return false;
		if (lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y)return false;
		if (lhs.max.z < rhs.min.z || lhs.min.z > rhs.max.z)return false;
		return true;
	}

	/**
	*	アクティブなエンティティの状態を更新する
	*
	*	@param delta	前回の更新からの経過時間
	*	@param matView	View行列
	*	@param matProj	Projection行列
	*	@param matDepthVP 影描画用行列
	*/
	void Buffer::Update(double delta, const glm::mat4* matView, const glm::mat4& matProj, const glm::mat4& matDepthVP) {

		//TODO: 
		//LinkEntity& e = *static_cast<LinkEntity*>(activeList[0].next);強引にルートノードとして引っ張っている
		//e.UpdateRecursive(delta);


		//エンティティの更新処理
		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
			for (itrUpdate = activeList[groupId].next; itrUpdate != &activeList[groupId]; itrUpdate = itrUpdate->next) {

				LinkEntity& e = *static_cast<LinkEntity*>(itrUpdate);
				e.transform.position += e.velocity * static_cast<float>(delta);

				//エンティティ内の更新処理
				if (e.updateFunc) {
					e.updateFunc(e, delta);
				}

				//ワールド空間の衝突判定用座標の更新処理
				e.colWorld.min = e.colLocal.min + e.transform.position;
				e.colWorld.max = e.colLocal.max + e.transform.position;
			}
		}

		//衝突判定処理
		for (const auto& e : collisionHandlerList) {
			if (!e.handler) {
				continue;
			}
			Link* listL = &activeList[e.groupId[0]];
			Link* listR = &activeList[e.groupId[1]];
			for (itrUpdate = listL->next; itrUpdate != listL; itrUpdate = itrUpdate->next) {
				LinkEntity* entityL = static_cast<LinkEntity*>(itrUpdate);
				for (itrUpdateRhs = listR->next; itrUpdateRhs != listR; itrUpdateRhs = itrUpdateRhs->next) {
					LinkEntity* entityR = static_cast<LinkEntity*>(itrUpdateRhs);
					if (!HasCollision(entityL->colWorld, entityR->colWorld)) {
						continue;
					}

					e.handler(*entityL, *entityR);
					if (entityL != itrUpdate) {
						break;
					}
				}
			}
		}
		itrUpdate = nullptr;
		itrUpdateRhs = nullptr;

		//カメラの変換行列(ビュー・射影)の計算処理
		uint8_t* p = static_cast<uint8_t*>(ubo->MapBuffer());
		glm::mat4 matVP[Uniform::maxViewCount];
		for (int i = 0; i < Uniform::maxViewCount; ++i) {
			//カメラごとの行列計算処理
			matVP[i] = matProj * matView[i];
		}

		/*	未実装
		//ワールド変換行列の計算処理
		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
			for (itrUpdate = activeList[groupId].next; itrUpdate != &activeList[groupId]; itrUpdate = itrUpdate->next) {
				LinkEntity& e = *static_cast<LinkEntity*>(itrUpdate);

			}
		}*/

		//groupIdごとにVertexData更新処理
		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
			for (itrUpdate = activeList[groupId].next; itrUpdate != &activeList[groupId]; itrUpdate = itrUpdate->next) {
				LinkEntity& e = *static_cast<LinkEntity*>(itrUpdate);
				UpdateUniformVertexData(e, p + e.uboOffset, matVP, matDepthVP, visibilityFlags[groupId]);
			}
		}
		ubo->UnmapBuffer();
	}

	/**
	*	アクティブなエンティティを描画する
	*
	*	@param meshBuffer 描画に使用するメッシュバッファへのポインタ
	*/
	void Buffer::Draw(const Mesh::BufferPtr& meshBuffer) const {

		meshBuffer->BindVAO();
		for (int viewIndex = 0; viewIndex < Uniform::maxViewCount; ++viewIndex) {
			for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
				//std::cout << "groupID: " << groupId << std::endl;

				if (!(visibilityFlags[groupId] & (1 << viewIndex))) {
					//見えないエンティティは描画しない
					continue;
				}

				for (const Link* itr = activeList[groupId].next; itr != &activeList[groupId]; itr = itr->next) {

					const LinkEntity& e = *static_cast<const LinkEntity*>(itr);
					if (e.mesh && e.texture && e.program) {

						//std::cout << "DrawEntity name: " << e.name << std::endl;

						e.program->UseProgram();
						//e.program->BindTexture(GL_TEXTURE0, GL_TEXTURE_2D, e.texture->Id());
						for (size_t i = 0; i < sizeof(e.texture) / sizeof(e.texture[0]); ++i) {

							e.program->BindTexture(GL_TEXTURE0 + i, GL_TEXTURE_2D, e.texture[i]->Id());
						}
						e.program->SetViewIndex(viewIndex);
						ubo->BindBufferRange(e.uboOffset, ubSizePerEntity);
						e.mesh->Draw(meshBuffer);
					}
				}
			}
		}
	}

	/**
	*	アクティブなエンティティを描画する
	*
	*	@param meshBuffer 描画に使用するメッシュバッファへのポインタ
	*	tips シェーダの設定は外部で行うこと
	*/
	void Buffer::DrawDepth(const Mesh::BufferPtr& meshBuffer) const {

		meshBuffer->BindVAO();
		for (int viewIndex = 0; viewIndex < Uniform::maxViewCount; ++viewIndex) {
			for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
				if (!(visibilityFlags[groupId] & (1 << viewIndex))) {
					continue;
				}
				for (const Link* itr = activeList[groupId].next; itr != &activeList[groupId];
					itr = itr->next) {
					const LinkEntity& e = *static_cast<const LinkEntity*>(itr);
					if (e.mesh && e.texture && e.program) {
						for (size_t i = 0; i < sizeof(e.texture) / sizeof(e.texture[0]); ++i) {
							e.program->BindTexture(GL_TEXTURE0 + i, GL_TEXTURE_2D,
								e.texture[i]->Id());
						}
						ubo->BindBufferRange(e.uboOffset, ubSizePerEntity);
						e.mesh->Draw(meshBuffer);
					}
				}
			}
		}
	}

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
	void Buffer::CollisionHandler(int gid0, int gid1, CollisionHandlerType handler) {

		if (gid0 > gid1) {
			std::swap(gid0, gid1);
		}
		auto itr = std::find_if(collisionHandlerList.begin(), collisionHandlerList.end(),
			[&](const CollisionHandlerInfo& e) {
			return e.groupId[0] == gid0 && e.groupId[1] == gid1; });
		if (itr == collisionHandlerList.end()) {
			collisionHandlerList.push_back({ { gid0,gid1 },handler });
		}
		else {
			itr->handler = handler;
		}
	}

	/**
	*	衝突解決ハンドラを取得する
	*
	*	@param gid0	衝突対象のグループID
	*	@param gid1 衝突対象のグループID
	*
	*	@return 衝突解決ハンドラ
	*/
	const CollisionHandlerType& Buffer::CollisionHandler(int gid0, int gid1) const {

		if (gid0 > gid1) {
			std::swap(gid0, gid1);
		}
		auto itr = std::find_if(collisionHandlerList.begin(), collisionHandlerList.end(),
			[&](const CollisionHandlerInfo& e) {
			return e.groupId[0] == gid0 && e.groupId[1] == gid1; });
		if (itr == collisionHandlerList.end()) {
			static const CollisionHandlerType dummy;
			return dummy;
		}
		return itr->handler;
	}

	/**
	*	全ての衝突解決ハンドラを削除する
	*/
	void Buffer::ClearCollisionHanderList() {
		collisionHandlerList.clear();
	}
}
