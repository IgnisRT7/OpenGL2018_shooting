/**
*	@file Mesh.h
*/
#pragma once

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

		const std::string& Name() const { return name; }
		void Draw(const BufferPtr& buffer) const;

	private:


		Mesh() = default;
		Mesh(const std::string& n, size_t begin, size_t end);
		Mesh(const Mesh&) = default;
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
		static BufferPtr Create(int vboSize, int iboSize);

		bool LoadMeshFromFile(const char* filename);
		const MeshPtr& GetMesh(const char* name) const;
		const Material& GetMaterial(size_t index) const;
		void BindVAO() const;

		void PushLevel();
		void PopLevel();
		void ClearLevel();
		int Size() { return levelStack.size(); }

	private:

		Buffer() = default;
		~Buffer();
		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

	private:

		GLuint vbo = 0;	///< ���f���̒��_�f�[�^���i�[����VBO
		GLuint ibo = 0;	///< ���f���̃C���f�b�N�X�f�[�^���i�[����IBO
		GLuint vao = 0; ///< ���f���pVAO
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
