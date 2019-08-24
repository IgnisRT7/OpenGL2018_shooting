/**
*	@file Entity.cpp
*/
#include "Entity.h"
#include "Uniform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>

/**
*	�G���e�B�e�B�Ɋւ���R�[�h���i�[���閼�O���
*/
namespace Entity {

	/**
	*	VertexData��UBO�ɓ]������
	*
	*	@param entity
	*	@param ubo
	*	@param matViewProjection
	*	@param viewFlags
	*/
	void UpdateUniformVertexData(Entity& entity, void*ubo, const glm::mat4* matVP, const glm::mat4& matDepthVP, glm::u32 viewFlags) {

		Uniform::VertexData data;
		data.matModel = entity.CalcModelMatrix();
		data.matNormal = glm::mat4_cast(entity.Rotation());

		//�r���[ x �ˉe �s��̌v�Z
		for (int i = 0; i < Uniform::maxViewCount; ++i) {
			//�`��Ώۂł͂Ȃ����͍̂X�V���Ȃ�
			if (viewFlags & (1U << i)) {
				data.matMVP[i] = matVP[i] * data.matModel;
			}
		}

		data.matDepthMVP = matDepthVP * data.matModel;
		data.color = entity.Color();
		memcpy(ubo, &data, sizeof(data));
	}

	/**
	*	�ړ��E��]�E�g�k�s����擾����
	*
	*	@return TRS�s��
	*/
	glm::mat4 Entity::CalcModelMatrix() const
	{
		const glm::mat4 t = glm::translate(glm::mat4(), transform.position);
		const glm::mat4 r = glm::mat4_cast(transform.rotation);
		const glm::mat4 s = glm::scale(glm::mat4(), transform.scale);
		return t * r * s;
	}

	/**
	*	�G���e�B�e�B��j������
	*
	*	���̊֐����Ăяo������́A�G���e�B�e�B�𑀍삵�Ă͂Ȃ�Ȃ�
	*/
	void Entity::Destroy() {

		if (pBuffer) {
			pBuffer->RemoveEntity(this);
		}
	}

	/**
	*	�������g�������N���X�g����؂藣��
	*
	*	�����͂ǂ��ɂ��ڑ�����Ă��Ȃ���ԂɂȂ�
	*/
	void Buffer::Link::Remove() {
		next->prev = prev;
		prev->next = next;
		prev = this;
		next = this;
	}

	/**
	*	�����N�I�u�W�F�N�g�������̎�O�ɒǉ�����
	*
	*	@param p	�ǉ����郊���N�I�u�W�F�N�g�ւ̃|�C���^
	*
	*	p���������̃����N���X�g����؂藣���A�����̎�O�ɒǉ�����
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
	*	�G���e�B�e�B�o�b�t�@���쐬����
	*
	*	@param maxEntityCount	������G���e�B�e�B�̍ő吔
	*	@param ubSizePerEntity	�G���e�B�e�B���Ƃ�Uniform Buffer�̃o�C�g��
	*	@param bindingPoint		�G���e�B�e�B�pUBO�̃o�C���f�B���O�|�C���g
	*	@param ubName			�G���e�B�e�B�pUniform Buffer�̖��O
	*
	*	@return �쐬�����G���e�B�e�B�o�b�t�@�ւ̃|�C���^
	*/
	BufferPtr Buffer::Create(size_t maxEntityCount, GLsizeiptr ubSizePerEntity, int bindingPoint, const char* ubName) {


		struct Impl : Buffer { Impl() {} ~Impl() {} };
		BufferPtr p = std::make_shared<Impl>();
		if (!p) {
			std::cerr << "[Error] Entity::Buffer::Create: �o�b�t�@�̍쐬�Ɏ��s." << std::endl;
			return {};
		}

		//�f�[�^�̃T�C�Y���h���C�o���v������I�t�Z�b�g�E�A���C�����g�ɍ��킹��
		GLint offsetAlignment = 0;
		glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offsetAlignment);
		ubSizePerEntity = ((ubSizePerEntity + offsetAlignment - 1) / offsetAlignment)*offsetAlignment;

		p->ubo = UniformBuffer::Create(maxEntityCount * ubSizePerEntity, bindingPoint, ubName);
		p->buffer.reset(new LinkEntity[maxEntityCount]);
		if (!p->ubo || !p->buffer) {
			std::cerr << "[Error]: Entity::Buffer::Create: �o�b�t�@�̍쐬�Ɏ��s." << std::endl;
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
			e = 1;	//1�J�����\
		}

		return p;
	}

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
	Entity* Buffer::AddEntity(int groupId, const glm::vec3& position, const Mesh::MeshPtr& mesh, const TexturePtr texture[2], const Shader::ProgramPtr& program, EntityDataBasePtr eData) {

		if (freeList.prev == freeList.next) {
			std::cerr << "[Warning]: Entity::Buffer::AddEntity "
				"�󂫃G���e�B�e�B������܂���." << std::endl;
			return nullptr;
		}

		if (groupId <0 || groupId > maxGroupId) {

			std::cerr << "[Error]: Entity::Buffer::AddEntity"
				"�͈͊O�̃O���[�vID(" << groupId << ")���n����܂����B\n�O���[�vID��0�`" << maxGroupId << "�łȂ���΂Ȃ�܂���B" << std::endl;
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

	/**
	*	�G���e�B�e�B���폜����
	*
	*	@param �폜����G���e�B�e�B�̃|�C���^
	*/
	void Buffer::RemoveEntity(Entity* entity) {

		if (!entity || !entity->isActive) {
			std::cerr << "[Warning]: Entity::Buffer::RemoveEntity: "
				"��A�N�e�B�u�ȃG���e�B�e�B���폜���悤�Ƃ��܂���." << std::endl;
			return;
		}
		LinkEntity* p = static_cast<LinkEntity*>(entity);
		if (p < &buffer[0] || p >= &buffer[bufferSize]) {
			std::cerr << "[Warning]: Entity::Buffer::RemoveEntity: "
				"�قȂ�o�b�t�@����擾�����G���e�B�e�B���폜���悤�Ƃ��܂���." << std::endl;
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

	/**
	*	�S�ẴG���e�B�e�B���폜����
	*/
	void Buffer::RemoveAllEntity() {

		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
			while (activeList[groupId].next != &activeList[groupId]) {
				RemoveEntity(static_cast<LinkEntity*>(activeList[groupId].next));
			}
		}

	}

	/**
	*	��`���m�̏Փ˔���
	*/
	bool HasCollision(const CollisionData& lhs, const CollisionData& rhs) {
		if (lhs.max.x < rhs.min.x || lhs.min.x > rhs.max.x)return false;
		if (lhs.max.y < rhs.min.y || lhs.min.y > rhs.max.y)return false;
		if (lhs.max.z < rhs.min.z || lhs.min.z > rhs.max.z)return false;
		return true;
	}

	/**
	*	�A�N�e�B�u�ȃG���e�B�e�B�̏�Ԃ��X�V����
	*
	*	@param delta	�O��̍X�V����̌o�ߎ���
	*	@param matView	View�s��
	*	@param matProj	Projection�s��
	*/
	void Buffer::Update(float delta, const glm::mat4* matView, const glm::mat4& matProj, const glm::mat4& matDepthVP) {

		//�G���e�B�e�B�̍X�V����
		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
			for (itrUpdate = activeList[groupId].next; itrUpdate != &activeList[groupId]; itrUpdate = itrUpdate->next) {

				LinkEntity& e = *static_cast<LinkEntity*>(itrUpdate);

				e.transform.position += e.velocity * delta;
				if (e.entityData) {
					e.entityData->Update(delta);

				}

				//�R���W�����f�[�^�����[���h��ԏ�ɓW�J
				e.colWorld.min = e.colLocal.min + e.transform.position;
				e.colWorld.max = e.colLocal.max + e.transform.position;
			}
		}

		//�Փ˔��菈��
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

					//e.handler(*entityL, *entityR);

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

		//UBO�̍X�V����
		uint8_t* p = static_cast<uint8_t*>(ubo->MapBuffer());
		glm::mat4 matVP[Uniform::maxViewCount];
		for (int i = 0; i < Uniform::maxViewCount; ++i) {
			//�J�������Ƃ̍s��v�Z����
			matVP[i] = matProj * matView[i];
		}
		//groupId���Ƃ�VertexData�X�V����
		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {
			for (itrUpdate = activeList[groupId].next; itrUpdate != &activeList[groupId]; itrUpdate = itrUpdate->next) {
				LinkEntity& e = *static_cast<LinkEntity*>(itrUpdate);
				UpdateUniformVertexData(e, p + e.uboOffset, matVP, matDepthVP, visibilityFlags[groupId]);
			}
		}
		ubo->UnmapBuffer();
	}

	/**
	*	�A�N�e�B�u�ȃG���e�B�e�B��`�悷��
	*
	*	@param meshBuffer �`��Ɏg�p���郁�b�V���o�b�t�@�ւ̃|�C���^
	*/
	void Buffer::Draw(const Mesh::BufferPtr& meshBuffer) const {

		meshBuffer->BindVAO();
		for (int viewIndex = 0; viewIndex < Uniform::maxViewCount; ++viewIndex) {
			for (int groupId = 0; groupId <= maxGroupId; ++groupId) {


				if (!(visibilityFlags[groupId] & (1 << viewIndex))) {
					//�����Ȃ��G���e�B�e�B�͕`�悵�Ȃ�
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
						//e.program->SetViewIndex(viewIndex);
						ubo->BindBufferRange(e.uboOffset, ubSizePerEntity);
						e.mesh->Draw(meshBuffer);
					}
				}
			}
		}
		meshBuffer->UnBindVAO();
	}

	/**
	*	�A�N�e�B�u�ȃG���e�B�e�B��[�x����`�悷��
	*
	*	@param meshBuffer �`��Ɏg�p���郁�b�V���o�b�t�@�ւ̃|�C���^
	*	tips �V�F�[�_�̐ݒ�͊O���ōs������
	*/
	void Buffer::DrawDepth(const Mesh::BufferPtr& meshBuffer) const {

		meshBuffer->BindVAO();
		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {

			//�J�������猩���Ȃ��ݒ�̏ꍇ�͕\�������Ȃ�
			if (!visibilityFlags[groupId]) {
				continue;
			}

			for (const Link* itr = activeList[groupId].next; itr != &activeList[groupId];
				itr = itr->next) {
				const LinkEntity& e = *static_cast<const LinkEntity*>(itr);

				//�f�[�^������Ƃ� ���� castShadow�t���O���L���̎��Ɏ��s
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

	/**
	*	�X�e���V���o�b�t�@�̕`��
	*
	*	@param meshBuffer	���b�V���o�b�t�@
	*	@param prograrm		�g�p����v���O�����I�u�W�F�N�g
	*/
	void Buffer::DrawStencil(const Mesh::BufferPtr& meshBuffer, const Shader::ProgramPtr& program)const {

		meshBuffer->BindVAO();

		for (int groupId = 0; groupId <= maxGroupId; ++groupId) {

			//�J�������猩���Ȃ��ݒ�̏ꍇ�͕\�������Ȃ�
			if (!visibilityFlags[groupId]) {
				continue;
			}

			for (const Link* itr = activeList[groupId].next; itr != &activeList[groupId];
				itr = itr->next) {
				const LinkEntity& e = *static_cast<const LinkEntity*>(itr);

				//�f�[�^������Ƃ� ���� castStencil�t���O���L���̎��Ɏ��s
				if (e.mesh && e.texture && e.program && e.castStencil) {

					ubo->BindBufferRange(e.uboOffset, ubSizePerEntity);
					program->SetVectorParameter(e.StencilColor(), "stencilColor");
					e.mesh->Draw(meshBuffer);
				}
			}

		}
		meshBuffer->UnBindVAO();
	}

	/**
	*	�\������GroupID�̐ݒ�
	*
	*	@param groupId		�K�p����GroupID
	*	@param cameraIndex	�K�p����J�����̃C���f�b�N�X�ԍ�
	*	@param isVisible	�\���t���O
	*/
	void Buffer::GroupVisibility(int groupId, int cameraIndex, bool isVisible) {

		if (isVisible) {
			visibilityFlags[groupId] |= (1U << cameraIndex);
		}
		else {
			visibilityFlags[groupId] &= ~(1U << cameraIndex);
		}
	}

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

	/**
	*	�S�Ă̏Փˉ����n���h�����폜����
	*/
	void Buffer::ClearCollisionHanderList() {
		collisionHandlerList.clear();
	}

	/**
	*	�G���e�B�e�B�̌������s��
	*
	*	@param name	�G���e�B�e�B��
	*/
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
