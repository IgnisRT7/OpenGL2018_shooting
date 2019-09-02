/**
*	@file Mesh.h
*	@brief	���b�V���̊Ǘ��p
*	@author	tn-mai(�u�`���������)
*/
#pragma once

#include "BufferObject.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

namespace Mesh {

	class Mesh;
	class Buffer;

	using MeshPtr = std::shared_ptr<Mesh>;	///< ���b�V���f�[�^�|�C���^
	using BufferPtr = std::shared_ptr<Buffer>;	///< ���b�V���o�b�t�@�|�C���^

	/**
	*	�}�e���A���\����
	*/
	struct Material {

		GLenum type;	///< �C���f�b�N�X�f�[�^�^
		GLsizei size;	///< �`�悷��C���f�b�N�X��
		GLvoid* offsest;///< �`��J�n�C���f�b�N�X�̃o�C�g�I�t�Z�b�g
		GLint baseVertex;///< �C���f�b�N�X0�Ƃ݂Ȃ���钸�_�z����̈ʒu
		glm::vec4 color; ///< �}�e���A���̐F
	};

	/**
	*	���b�V��
	*	���b�V���f�[�^��Buffer�N���X����쐬�ł���悤�ɂȂ��Ă���
	*/
	class Mesh {
		friend class Buffer;
	public:

		/**
		*	���b�V�����̎擾
		*
		*	@return ���b�V����
		*/
		const std::string& Name() const { return name; }

		/**
		*	���b�V����`�悷��
		*
		*	@param buffer	�`��Ɏg�p���郁�b�V���o�b�t�@�ւ̃|�C���^
		*/
		void Draw(const BufferPtr& buffer) const;

	private:


		Mesh() = default;
		Mesh(const Mesh&) = default;

		/**
		*	�R���X�g���N�^
		*
		*	@param meshName	���b�V���f�[�^��
		*	@param begin	�`�悷��}�e���A���̐擪�C���f�b�N�X
		*	@param end		�`�悷��}�e���A���̏I�[�C���f�b�N�X
		*/
		Mesh(const std::string& n, size_t begin, size_t end);

		~Mesh() = default;
		Mesh& operator=(const Mesh&) = default;

	private:

		std::string name;	///< ���b�V���f�[�^��
		std::vector<std::string> textureList;	///�e�N�X�`�����̃��X�g
		size_t beginMaterial = 0;	///< �`�悷��}�e���A���̐擪�C���f�b�N�X
		size_t endMaterial = 0;		///< �`�悷��}�e���A���̏I�[�C���f�b�N�X
	};

	/**
	*	���b�V���o�b�t�@
	*/
	class Buffer {
	public:

		/**
		*	���b�V���o�b�t�@���쐬����
		*
		*	@param vboSize	�i�[�\�ȑ����_��
		*	@param iboSize	�o�b�t�@�Ɋi�[�\�ȑ��C���f�b�N�X��
		*/
		static BufferPtr Create(int vboSize, int iboSize);

		/**
		*	���b�V�����t�@�C������ǂݍ���
		*
		*	@param filename ���b�V���t�@�C����
		*
		*	@retval true	�ǂݍ��ݐ���
		*	@retval false	�ǂݍ��ݎ��s
		*/
		bool LoadMeshFromFile(const char* filename);

		/**
		*	���b�V�����擾����
		*
		*	@param name ���b�V����
		*
		*	@return name�ɑΉ����郁�b�V���ւ̃|�C���^
		*/
		const MeshPtr& GetMesh(const char* name) const;

		/**
		*	�}�e���A�����擾����
		*
		*	@param index �}�e���A���C���f�b�N�X
		*
		*	@return index�ɑΉ�����}�e���A��
		*/
		const Material& GetMaterial(size_t index) const;

		/**
		*	�o�b�t�@���ێ�����VAO��OpenGL�̏����Ώۂɐݒ肷��
		*/
		void BindVAO() const;

		/**
		*	�o�b�t�@�̕R�Â��̉���
		*/
		void UnBindVAO() const;

		/**
		*	�X�^�b�N�ɐV�������\�[�X���x�����쐬����
		*/
		void PushLevel();

		/**
		*	�X�^�b�N�̖����ɂ��郊�\�[�X���x������������
		*/
		void PopLevel();

		/**
		*	�����̃��\�[�X���x������̏�Ԃɂ���
		*/
		void ClearLevel();
		int Size() { return levelStack.size(); }

	private:

		Buffer() = default;
		~Buffer();
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

	private:

		BufferObject ibo;					///< ���_�f�[�^
		BufferObject vbo;
		VertexArrayObject vao;				///< �C���f�b�N�X�f�[�^

		std::vector<Material> materialList;	///< �}�e���A�����X�g

		//tips :�R�R���牺�͏�Lvbo,ibo,materialList�̂ǂ��Ɍ��ݓǂݍ��ނׂ�
		//		�f�[�^�����邩�̏����i�[����f�[�^�Q�ł�

		struct Level {
			GLintptr vboEnd = 0;			///< �ǂݍ��ݍςݒ��_�f�[�^�̏I�[
			GLintptr iboEnd = 0;			///< �ǂݍ��ݍς݃C���f�b�N�X�f�[�^�I�[
			size_t materialBaseOffset = 0;	///< �}�e���A���̊e�J�n�ʒu
			std::unordered_map<std::string, MeshPtr> meshList;	///< ���b�V�����X�g
		};
		std::vector <Level>	levelStack;				///< ���\�[�X�X�^�b�N
		static const size_t minimalStackSize = 1;	///< �X�^�b�N�T�C�Y�͂����菬�����Ȃ�Ȃ�
	};



}
