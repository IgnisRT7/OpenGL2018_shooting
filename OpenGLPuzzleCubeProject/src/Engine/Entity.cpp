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
	void UpdateUniformVertexData(Entity& entity, void* ubo, const glm::mat4* matVP, const glm::mat4& matDepthVP, glm::u32 viewFlags) {

		Uniform::VertexData data;
		data.matModel = entity.CalcModelMatrix();
		data.matNormal = glm::mat4_cast(entity.Rotation());

		//ビュー x 射影 行列の計算
		for (int i = 0; i < Uniform::maxViewCount; ++i) {
			//描画対象ではないものは更新しない
			if (viewFlags & (1U << i)) {
				data.matMVP[i] = matVP[i] * data.matModel;
			}
		}

		data.matDepthMVP = matDepthVP * data.matModel;
		data.color = entity.Color();
		memcpy(ubo, &data, sizeof(data));
	}

	glm::mat4 Entity::CalcModelMatrix() const
	{
		const glm::mat4 t = glm::translate(glm::mat4(), transform.position);
		const glm::mat4 r = glm::mat4_cast(transform.rotation);
		const glm::mat4 s = glm::scale(glm::mat4(), transform.scale);
		return t * r * s;
	}

	void Entity::Destroy() {

		if (pBuffer) {
			pBuffer->RemoveEntity(this);
		}
	}

	void Buffer::Link::Remove() {
		next->prev = prev;
		prev->next = next;
		prev = this;
		next = this;
	}

	void Buffer::Link::Insert(Link* p)
	{
		p->Remove();
		p->prev = prev;
		p->next = this;
		prev->next = p;
		prev = p;
	}

	BufferPtr Buffer::Create(size_t maxEntityCount, GLsizeiptr ubSizePerEntity, int bindingPoint, const char* ubName) {

		struct Impl : Buffer { Impl() {} ~Impl() {} };
		BufferPtr p = std::make_shared<Impl>();
		if (!p) {
			std::cerr << "[Error] Entity::Buffer::Create: バッファの作成に失敗." << std::endl;
			return {};
		}

		//データのサイズをドライバが要求するオフセット・アライメントに合わせる
		GLint offsetAlignment = 0;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offsetAlignment);
		ubSizePerEntity = ((ubSizePerEntity + offsetAlignment - 1) / offsetAlignment) * offsetAlignment;

		p->ubo = UniformBuffer::Create(maxEntityCount * ubSizePerEntity, bindingPoint, ubName);
		p->buffer.reset(new LinkEntity[maxEntityCount]);
		if (!p->ubo || !p->buffer) {
			std::cerr << "[Error]: Entity::Buffer::Create: バッファの作成に失敗." << std::endl;
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
			e = 1;	//1カメ可視可能
		}

		return p;
	}

	Entity* Buffer::AddEntity(int groupId, const glm::vec3& position, const Mesh::MeshPtr& mesh, const TexturePtr texture[2], const Shader::ProgramPtr& program, EntityDataBasePtr eData) {

		if (freeList.prev == freeList.next) {
			std::cerr << "[Warning]: Entity::Buffer::AddEntity "
				"空きエンティティがありません." << std::endl;
			return nullptr;
		}

		if (groupId <0 || groupId > maxGroupId) {

			std::cerr << "[Error]: Entity::Buffer::AddEntity"
				"範囲外のグループID(" << groupId << ")が渡されました。\nグループIDは0～" << maxGroupId << "でなければなりません。" << std::endl;
			return nullptr;
		}

		LinkEntity* entity = static_cast<LinkEntity*>(freeList.prev);
		activeList[groupId].Insert(entity);

		entity->name = mesh ? mesh->Name() : "none";
		entity->groupId = groupId;
		entity->transform = TransformData({ position,glm::vec3(1,1,1),glm::quat() });
		entity->velocity = glm::vec3();
		entity->color = glm::vec4(1);
		entity->mesh = mesh;
		entity->texture[0] = texture[0];
		entity->texture[1] = texture[1];
		entity->program = program;
		entity->entityData = eData;
		if (eData) {
			eData->SetEntity(*dynamic_cast<Entity*>(entity));
			eData->Initialize();
		}
		entity->isActive = true;
		return entity;
	}

	void Buffer::RemoveEntity(Entity* entity) {

		if (!entity || !entity->isActive) {
			std::cerr << "[Warning]: Entity::Buffer::RemoveEntity: "
				"非アクティブなエンティティを削除しようとしました." << std::endl;
			return;
		}
		LinkEntity* p = static_cast<LinkEntity*>(entity);
		if (p < &buffer[0] || p >= &buffer[bufferSize]) {
			std::cerr << "[Warning]: Entity::Buffer::RemoveEntity: "
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
		p->isActive = false;
	}

	void Buffer::RemoveAllEntity() {

		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
			while (activeList[groupId].next != &activeList[groupId]) {
				RemoveEntity(static_cast<LinkEntity*>(activeList[groupId].next));
			}
		}

	}

	/**
	*	矩形同士の衝突判定
	*
	*	@param lhs	コリジョンデータA
	*	@param rhs	コリジョンデータB
	*
	*	@retval true	衝突した
	*	@retval false	衝突しなかった
	*/
	bool HasCollision(const CollisionData& lhs, const CollisionData& rhs) {
		if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x)return false;
		if (lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y)return false;
		if (lhs.max.z < rhs.min.z || lhs.min.z > rhs.max.z)return false;
		return true;
	}

	void Buffer::Update(float delta, const glm::mat4* matView, const glm::mat4& matProj, const glm::mat4& matDepthVP) {

		//エンティティの更新処理
		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
			for (itrUpdate = activeList[groupId].next; itrUpdate != &activeList[groupId]; itrUpdate = itrUpdate->next) {

				LinkEntity& e = *static_cast<LinkEntity*>(itrUpdate);

				e.transform.position += e.velocity * delta;
				if (e.entityData) {
					e.entityData->Update(delta);
				}

				//コリジョンデータをワールド空間上に展開
				e.colWorld.min = e.colLocal.min + e.transform.position;
				e.colWorld.max = e.colLocal.max + e.transform.position;
			}
		}

		//衝突判定処理
		for (const auto& e : collisionHandlerList) {

			Link* listL = &activeList[e.groupId[0]];
			Link* listR = &activeList[e.groupId[1]];
			for (itrUpdate = listL->next; itrUpdate != listL; itrUpdate = itrUpdate->next) {
				LinkEntity* entityL = static_cast<LinkEntity*>(itrUpdate);

				for (itrUpdateRhs = listR->next; itrUpdateRhs != listR; itrUpdateRhs = itrUpdateRhs->next) {
					LinkEntity* entityR = static_cast<LinkEntity*>(itrUpdateRhs);

					if (!HasCollision(entityL->colWorld, entityR->colWorld)) {
						continue;
					}

					if (entityL->entityData) {
						entityL->entityData->CollisionEnter(*entityR);
					}
					if (entityR->entityData) {
						entityR->entityData->CollisionEnter(*entityL);
					}

					if (entityL != itrUpdate) {
						break;
					}
				}
			}
		}
		itrUpdate = nullptr;
		itrUpdateRhs = nullptr;

		//UBOの更新処理
		uint8_t* p = static_cast<uint8_t*>(ubo->MapBuffer());
		glm::mat4 matVP[Uniform::maxViewCount];
		for (int i = 0; i < Uniform::maxViewCount; ++i) {
			//カメラごとの行列計算処理
			matVP[i] = matProj * matView[i];
		}
		//groupIdごとにVertexData更新処理
		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
			for (itrUpdate = activeList[groupId].next; itrUpdate != &activeList[groupId]; itrUpdate = itrUpdate->next) {
				LinkEntity& e = *static_cast<LinkEntity*>(itrUpdate);
				UpdateUniformVertexData(e, p + e.uboOffset, matVP, matDepthVP, visibilityFlags[groupId]);
			}
		}
		ubo->UnmapBuffer();
	}

	void Buffer::Draw(const Mesh::BufferPtr& meshBuffer) const {

		meshBuffer->BindVAO();
		for (int viewIndex = 0; viewIndex < Uniform::maxViewCount; ++viewIndex) {
			for (int groupId = 0; groupId <= maxGroupId; ++groupId) {

				if (!(visibilityFlags[groupId] & (1 << viewIndex))) {
					//見えないエンティティは描画しない
					continue;
				}

				for (const Link* itr = activeList[groupId].next; itr != &activeList[groupId]; itr = itr->next) {

					const LinkEntity& e = *static_cast<const LinkEntity*>(itr);

					if (e.mesh && e.texture && e.program) {

						e.program->UseProgram();
						for (size_t i = 0; i < sizeof(e.texture) / sizeof(e.texture[0]); ++i) {

							if (e.texture[i]) {
								e.program->BindTexture(GL_TEXTURE0 + i, e.texture[i]->Id());
							}
						}
						ubo->BindBufferRange(e.uboOffset, ubSizePerEntity);
						e.mesh->Draw(meshBuffer);
					}
				}
			}
		}
		meshBuffer->UnBindVAO();
	}

	void Buffer::DrawDepth(const Mesh::BufferPtr& meshBuffer) const {

		meshBuffer->BindVAO();
		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {

			//カメラから見えない設定の場合は表示させない
			if (!visibilityFlags[groupId]) {
				continue;
			}

			for (const Link* itr = activeList[groupId].next; itr != &activeList[groupId];
				itr = itr->next) {
				const LinkEntity& e = *static_cast<const LinkEntity*>(itr);

				//データがあるとき かつ castShadowフラグが有効の時に実行
				if (e.mesh && e.program && e.castShadow) {
					for (size_t i = 0; i < sizeof(e.texture) / sizeof(e.texture[0]); ++i) {
						e.program->BindTexture(GL_TEXTURE0 + i,e.texture[i]->Id());
					}
					ubo->BindBufferRange(e.uboOffset, ubSizePerEntity);
					e.mesh->Draw(meshBuffer);
				}
			}
		}
		meshBuffer->UnBindVAO();
	}

	void Buffer::DrawStencil(const Mesh::BufferPtr& meshBuffer, const Shader::ProgramPtr& program)const {

		meshBuffer->BindVAO();

		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {

			//カメラから見えない設定の場合は表示させない
			if (!visibilityFlags[groupId]) {
				continue;
			}

			for (const Link* itr = activeList[groupId].next; itr != &activeList[groupId];
				itr = itr->next) {
				const LinkEntity& e = *static_cast<const LinkEntity*>(itr);

				//データがあるとき かつ castStencilフラグが有効の時に実行
				if (e.mesh && e.texture && e.program && e.castStencil) {

					ubo->BindBufferRange(e.uboOffset, ubSizePerEntity);
					program->SetVectorParameter(e.StencilColor(), "stencilColor");
					e.mesh->Draw(meshBuffer);
				}
			}
		}
		meshBuffer->UnBindVAO();
	}

	void Buffer::GroupVisibility(int groupId, int cameraIndex, bool isVisible) {

		if (isVisible) {
			visibilityFlags[groupId] |= (1U << cameraIndex);
		}
		else {
			visibilityFlags[groupId] &= ~(1U << cameraIndex);
		}
	}

	void Buffer::CollisionHandler(int gid0, int gid1) {

		if (gid0 > gid1) {
			std::swap(gid0, gid1);
		}
		auto itr = std::find_if(collisionHandlerList.begin(), collisionHandlerList.end(),
			[&](const CollisionHandlerInfo& e) {
				return e.groupId[0] == gid0 && e.groupId[1] == gid1; });
		if (itr == collisionHandlerList.end()) {
			collisionHandlerList.push_back({ gid0,gid1 });
		}
	}

	void Buffer::ClearCollisionHanderList() {
		collisionHandlerList.clear();
	}

	Entity* Buffer::FindEntity(FindEntityFunc searchFunc) {

		if (searchFunc) return nullptr;

		for (int viewIndex = 0; viewIndex < Uniform::maxViewCount; ++viewIndex) {
			for (int groupId = 0; groupId <= maxGroupId; ++groupId) {

				for (Link* itr = activeList[groupId].next; itr != &activeList[groupId]; itr = itr->next) {

					LinkEntity* e = static_cast<LinkEntity*>(itr);


					if (searchFunc(dynamic_cast<Entity*>(e))) {
						return dynamic_cast<Entity*>(e);
					}

				}
			}
		}

		return nullptr;
	}
}
