/**
*	@file	Mesh.cpp
*/

#include "Mesh.h"
#include <fbxsdk.h>
#include <iostream>
#include <algorithm>
#include <fbxsdk/utils/fbxgeometryconverter.h>

/**
*	���f���f�[�^�Ǘ��̂��߂̖��O���
*/
namespace Mesh {

	/// ���_�f�[�^�^
	struct Vertex {

		glm::vec3 position = { 0,0,0 };	///< ���W
		glm::vec4 color = { 1,1,1,1 };	///< �F
		glm::vec2 texCoord = { 0,0 };	///< �e�N�X�`�����W
		glm::vec3 normal = { 1,0,0 };	///< �@��
		glm::vec4 tangent = { 1,0,0,0 };	///< �ڃx�N�g��
	};

	/**
	*	FBX�x�N�g����glm�x�N�g���ɕϊ�����
	*
	*	@param fbxVec FBX�x�N�g��
	*
	*	@return glm::�x�N�g��
	*/
	template<typename T>
	glm::vec4 ToVec4(const T& fbxVec) {
		return glm::vec4(
			static_cast<float>(fbxVec[0]),
			static_cast<float>(fbxVec[1]),
			static_cast<float>(fbxVec[2]),
			static_cast<float>(fbxVec[3]));
	}
	template<typename T>
	glm::vec3 ToVec3(const T& fbxVec) {
		return glm::vec3(
			static_cast<float>(fbxVec[0]),
			static_cast<float>(fbxVec[1]),
			static_cast<float>(fbxVec[2]));
	}
	template<typename T>
	glm::vec2 ToVec2(const T& fbxVec) {
		return glm::vec2(
			static_cast<float>(fbxVec[0]),
			static_cast<float>(fbxVec[1]));
	}

	/**
	*	���_�p�����[�^���擾����
	*
	*	@param mappingMode	�g�p����C���f�b�N�X��I������}�b�s���O���[�h
	*	@param isDirectRef	���ڎQ�ƃ��[�h�Ȃ�^�A�ԐڎQ�ƃ��[�h�Ȃ�U��n��
	*	@param pIndexList	�ԐڎQ�Ɨp�̃C���f�b�N�X�z��BisDirectRef���U�̏ꍇ�g����
	*	@param pList		�v�f�̔z��
	*	@param cpIndex		�}�b�s���O���[�h�����_�P�ʂ̏ꍇ�Ɏg�p����C���f�b�N�X
	*	@param polygonVertex �}�b�s���O���[�h���|���S���P�ʂ̏ꍇ�Ɏg�p����C���f�b�N�X
	*	@param defaultValue	���Ή��}�b�s���O���[�h�̏ꍇ�ɕԂ��l
	*	
	*	@return �}�b�s���O���[�h�ƎQ�ƃ��[�h�ɉ����ēK�؂Ȕz�񂩂璸�_�̃p�����[�^��Ԃ�
	*/
	template<typename T>
	T GetElement(
		FbxGeometryElement::EMappingMode mappingMode, bool isDirectRef, const FbxLayerElementArrayTemplate<int>* pIndexList,
		const FbxLayerElementArrayTemplate<T>* pList, int cpIndex, int polygonVertex, const T& defaultValue) {

		switch (mappingMode) {
		case FbxLayerElement::eByControlPoint:
			return (*pList)[isDirectRef ? cpIndex : (*pIndexList)[cpIndex] ];
		case FbxLayerElement::eByPolygonVertex:
			return (*pList)[isDirectRef ? polygonVertex : (*pIndexList)[polygonVertex] ];
		default:
			return defaultValue;
		}
	}

	/**
	*	FBX�I�u�W�F�N�g��j�����邽�߂̃w���p�[�\����
	*/
	template<typename T>
	struct Deleter {
		void operator()(T* p) { if (p) { p->Destroy(); } }
	};

	/**
	*	�}�e���A���̉��f�[�^
	*/
	struct TemporaryMaterial {
		glm::vec4 color = glm::vec4(1);
		std::vector<uint32_t> indexBuffer;
		std::vector<Vertex> vertexBuffer;
		std::vector<std::string> textureName;
	};

	/**
	*	���b�V���̉��f�[�^
	*/
	struct TemporaryMesh {
		std::string name;
		std::vector<TemporaryMaterial> materialList;
	};

	/**
	*	FBX�f�[�^�𒆊ԃf�[�^�ɕϊ�����N���X
	*/
	struct FbxLoader {
//		bool GetMotionList(void);

		/**
		*	FBX�t�@�C����ǂݍ���
		*
		*	@param filename FBX�t�@�C����
		*
		*	@retval true	�ǂݍ��ݐ���
		*	@retval false	�ǂݍ��ݎ��s
		*/
		bool Load(const char* filename);

		/**
		*	FBX�f�[�^�����f�[�^�ɕϊ�����
		*
		*	@param fbxNode �ϊ��Ώۂ�FBX�m�[�h�ւ̃|�C���^
		*
		*	@retval true	�ϊ�����
		*	@retval false	�ϊ����s
		*/
		bool Convert(FbxNode* node);

		/**
		*	FBX���b�V�������f�[�^�ɕϊ�����
		*
		*	@param fbxNode �ϊ��Ώۂ�FBX�m�[�h�ւ̃|�C���^
		*
		*	@retval true	�ϊ�����
		*	@retval false	�ϊ����s
		*/
		bool LoadMesh(FbxNode* node);
		std::vector<TemporaryMesh> meshList;
		std::unique_ptr<FbxManager, Deleter<FbxManager> > fbxManager;
		FbxScene* fbxScene;
	};

	bool FbxLoader::Load(const char* filename) {
		
		//FBXManager�쐬
		 fbxManager = std::unique_ptr<FbxManager, Deleter<FbxManager> >(FbxManager::Create());
		if (!fbxManager) {
			std::cerr << "[Error]: FbxLoader::Load "<<
				filename << "�̓ǂݍ��݂Ɏ��s(FbxManagerFaild)" << std::endl;
			return false;
		}

		//FbxScene�쐬
		fbxScene = FbxScene::Create(fbxManager.get(), "");
		if (!fbxScene) {
			std::cerr << "[Error]: FbxLoader::Load "<<
				filename << "�̓ǂݍ��݂Ɏ��s(FbxSceneFaild)" << std::endl;
			return false;
		}
		else {

			//FBxImporter�쐬
			std::unique_ptr<FbxImporter, Deleter<FbxImporter> > fbxImporter(FbxImporter::Create(fbxManager.get(), ""));
			const bool importStatus = fbxImporter->Initialize(filename);
			if (!importStatus || !fbxImporter->Import(fbxScene)) {
				std::cerr << "[Error]: FbxLoader::Load " <<
					filename << "�̓ǂݍ��݂Ɏ��s(FbxImporterFaild)\n" << fbxImporter->GetStatus().GetErrorString() << std::endl;
				return false;
			}
		}

		if (!Convert(fbxScene->GetRootNode())) {
			std::cerr << "[Error]: FbxLoader::Load " <<
				filename << "�̕ϊ��Ɏ��s(FbxConverterFaild)" << std::endl;
			return false;
		}

		return true;
	}

	bool FbxLoader::Convert(FbxNode* fbxNode) {

		if (!fbxNode) {
			return true;
		}

		if (!LoadMesh(fbxNode)) {
			return false;
		}

		const int childCount = fbxNode->GetChildCount();
		for (int i = 0; i < childCount; ++i) {

			if (!Convert(fbxNode->GetChild(i))) {
				return false;
			}
		}
		return true;
	}

	bool FbxLoader::LoadMesh(FbxNode* fbxNode) {

		FbxMesh* fbxMesh = fbxNode->GetMesh();
		if (!fbxMesh) {
			return true;
		}

		TemporaryMesh mesh;
		mesh.name = fbxNode->GetName();

		std::cout << "[Info]: FbxLoader::LoadMesh " << mesh.name << " �ǂݍ��ݒ�" << std::endl;;

		if (!fbxMesh->IsTriangleMesh()) {
			std::cout << "[Warning]: FbxLoader::LoadMesh " << mesh.name << "�ɂ͎O�p�`�ȊO�̃������܂܂�Ă��܂�" << std::endl;
			std::cout << "�O�p���������s���܂�" << std::endl;

			//�S���_�|���S���̎O�p��
			FbxGeometryConverter fbxGeometoryConverter(fbxManager.get());
			fbxGeometoryConverter.Triangulate(fbxScene, true);
		}

		//�}�e���A������ǂݎ��
		const int materialCount = fbxNode->GetMaterialCount();
		mesh.materialList.reserve(materialCount);

		for (int i = 0; i < materialCount; ++i) {

			TemporaryMaterial material;
			if (FbxSurfaceMaterial* fbxMaterial = fbxNode->GetMaterial(i)) {
				//�}�e���A���̐F����ǂݎ��
				const FbxClassId classId = fbxMaterial->GetClassId();
				if (classId == FbxSurfaceLambert::ClassId || classId == FbxSurfacePhong::ClassId) {
					const FbxSurfaceLambert* pLambert = static_cast<const FbxSurfaceLambert*>(fbxMaterial);
					material.color = glm::vec4(ToVec3(pLambert->Diffuse.Get()), static_cast<float>(1.0f - pLambert->TransparencyFactor));
				}

				mesh.materialList.push_back(material);
			}
		}

		if (mesh.materialList.empty()) {
			mesh.materialList.push_back(TemporaryMaterial());
		}

		//���_�v�f�̗L���𒲂ׂ�
		const bool hasColor = fbxMesh->GetElementVertexColorCount() > 0;
		const bool hasTexcoord = fbxMesh->GetElementUVCount() > 0;
		const bool hasNormal = fbxMesh->GetElementNormalCount() > 0;
		const bool hasTangent = fbxMesh->GetElementTangentCount() > 0;

		//UV�Z�b�g���̃��X�g���擾����
		FbxStringList uvSetNameList;
		fbxMesh->GetUVSetNames(uvSetNameList);

		//�F����ǂݎ�鏀��
		//@note ���W/UV/�@���ȊO�̃p�����[�^�ɂ͒��ړǂݎ��֐����񋟂���Ă��Ȃ����߁A
		//		�uFbxGeometryElement???�v�N���X����ǂݎ��K�v������
		FbxGeometryElement::EMappingMode colorMappingMode = FbxLayerElement::eNone;
		bool isColorDirectRef = true;
		const FbxLayerElementArrayTemplate<int>* colorIndexList = nullptr;
		const FbxLayerElementArrayTemplate<FbxColor>* colorList = nullptr;
		if (hasColor) {

			const FbxGeometryElementVertexColor* fbxColorList = fbxMesh->GetElementVertexColor();
			colorMappingMode = fbxColorList->GetMappingMode();
			isColorDirectRef = fbxColorList->GetReferenceMode() == FbxLayerElement::eDirect;
			colorIndexList = &fbxColorList->GetIndexArray();
			colorList = &fbxColorList->GetDirectArray();
		}

		//�ڃx�N�g����ǂݎ�鏀��
		FbxGeometryElement::EMappingMode tangentMappingMode = FbxLayerElement::eNone;
		bool isTangentDirectRef = true;
		const FbxLayerElementArrayTemplate<int>* tangentIndexList = nullptr;
		const FbxLayerElementArrayTemplate<FbxVector4>* tangentList = nullptr;
		FbxGeometryElement::EMappingMode binormalMappingMode = FbxLayerElement::eNone;
		bool isBinormalDirectRef = true;
		const FbxLayerElementArrayTemplate<int>* binormalIndexList = nullptr;
		const FbxLayerElementArrayTemplate<FbxVector4>* binormalList = nullptr;

		if (hasTangent) {

			const FbxGeometryElementTangent* fbxTangentList = fbxMesh->GetElementTangent();

			tangentMappingMode = fbxTangentList->GetMappingMode();
			isTangentDirectRef = fbxTangentList->GetReferenceMode() == FbxLayerElement::eDirect;
			tangentIndexList = &fbxTangentList->GetIndexArray();
			tangentList = &fbxTangentList->GetDirectArray();

			const FbxGeometryElementBinormal* fbxBinormalList = fbxMesh->GetElementBinormal();

			binormalMappingMode = fbxBinormalList->GetMappingMode();
			isBinormalDirectRef = fbxBinormalList->GetReferenceMode() == FbxLayerElement::eDirect;
			binormalIndexList = &fbxBinormalList->GetIndexArray();
			binormalList = &fbxBinormalList->GetDirectArray();

		}

		//���_���ǂ̃}�e���A���ɑ����邩�������}�e���A���C���f�b�N�X���X�g���擾����
		const FbxLayerElementArrayTemplate<int>* materialIndexList = nullptr;
		if (FbxGeometryElementMaterial* fbxMaterialLayer = fbxMesh->GetElementMaterial()) {
			materialIndexList = &fbxMaterialLayer->GetIndexArray();
		}

		//�|���S�����Ɋ�Â��āA���f�[�^�o�b�t�@�̗e�ʂ�\�񂷂�
		const int polygonCount = fbxMesh->GetPolygonCount() * 3;
		for (auto& e : mesh.materialList) {
			const size_t avarageCapacity = polygonCount / mesh.materialList.size();
			e.indexBuffer.reserve(avarageCapacity);
			e.vertexBuffer.reserve(avarageCapacity);
		}

		const FbxAMatrix matTRS(fbxNode->EvaluateGlobalTransform());
		
		const FbxAMatrix matR(FbxVector4(0, 0, 0), matTRS.GetR(), FbxVector4(1, 1, 1));
		const FbxVector4* const fbxControlPoints = fbxMesh->GetControlPoints();
		int polygonVertex = 0;
		for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex) {
			for (int pos = 0; pos < 3; ++pos) {

				Vertex v;
				const int cpIndex = fbxMesh->GetPolygonVertex(polygonIndex, pos);

				v.position = ToVec3(matTRS.MultT(fbxControlPoints[cpIndex]));

				//���_�J���[
				v.color = glm::vec4(1);
				if (hasColor || !colorList) {
					switch (colorMappingMode) {
					case FbxLayerElement::eByControlPoint:
						v.color = ToVec4((*colorList)[isColorDirectRef ? cpIndex : (*colorIndexList)[polygonVertex]]);
						break;

					case FbxLayerElement::eByPolygonVertex:
						v.color = ToVec4((*colorList)[
							isColorDirectRef ? polygonVertex : (*colorIndexList)[polygonVertex]]);
						break;

					default:
						break;
					}
				}

				//UV���W
				v.texCoord = glm::vec2(0);
				if (hasTexcoord) {
					FbxVector2 uv;
					bool unmapped;
					fbxMesh->GetPolygonVertexUV(polygonIndex, pos, uvSetNameList[0], uv, unmapped);
					v.texCoord = ToVec2(uv);
				}

				//�@��
				v.normal = glm::vec3(0, 0, 1);
				if (hasNormal) {
					FbxVector4 normal;
					fbxMesh->GetPolygonVertexNormal(polygonIndex, pos, normal);
					v.normal = glm::normalize(ToVec3(matR.MultT(normal)));
				}
				
				v.tangent = glm::vec4(1, 0, 0, 1);
				if (hasTangent) {

					//�]�@���x�N�g��
					const glm::vec3 binormal = ToVec3(matR.MultT(GetElement(
						binormalMappingMode, isBinormalDirectRef, binormalIndexList,
						binormalList, cpIndex, polygonVertex, FbxVector4(0, 1, 0, 1))));

					//�ڃx�N�g��
					const glm::vec3 tangent = ToVec3(matR.MultT(GetElement(
						tangentMappingMode, isTangentDirectRef, tangentIndexList,
						tangentList, cpIndex, polygonVertex, FbxVector4(1, 0, 0, 1))));
					v.tangent = glm::vec4(glm::normalize(tangent), 1);

					//���̏]�@���x�N�g���v�Z
					const glm::vec3 binormalTmp = glm::normalize(glm::cross(v.normal, tangent));
					
					if (glm::dot(binormal, binormalTmp) < 0) {
						v.tangent.w = -1;
					}
				}

				//���_�ɑΉ����鉼�}�e���A���ɁA���_�f�[�^�ƃC���f�b�N�X�f�[�^��ǉ�����
				TemporaryMaterial& materialData = mesh.materialList[materialIndexList ? (*materialIndexList)[polygonIndex] : 0];
				materialData.indexBuffer.push_back(static_cast<uint32_t>(materialData.vertexBuffer.size()));
				materialData.vertexBuffer.push_back(v);

				++polygonVertex;
			}
		}

		meshList.push_back(std::move(mesh));
		return true;
	}

	Mesh::Mesh(const std::string& meshName, size_t begin, size_t end) :
		name(meshName), beginMaterial(begin), endMaterial(end) {

	}

	void Mesh::Draw(const BufferPtr& buffer) const {

		if (!buffer) {
			return;
		}

		if (buffer->GetMesh(name.c_str()).get() != this) {
			std::cerr << "[Warnig]: Mesh::Draw �o�b�t�@�ɑ��݂��Ȃ����b�V��'" << name << "'��`�悵�悤�Ƃ��܂���" << std::endl;
			return;
		}

		for (size_t i = beginMaterial; i < endMaterial; ++i) {
			const Material& m = buffer->GetMaterial(i);
			glDrawElementsBaseVertex(GL_TRIANGLES, m.size, m.type, m.offsest, m.baseVertex);
		}
	}

	BufferPtr Buffer::Create(int vboSize, int iboSize) {

		std::cout << "[Info]: MeshBuffer::Create" << std::endl;

		struct Impl :Buffer { Impl() {} ~Impl() {} };
		BufferPtr p = std::make_shared<Impl>();

		p->vbo.Init(GL_ARRAY_BUFFER,vboSize*sizeof(Vertex));
		p->ibo.Init(GL_ELEMENT_ARRAY_BUFFER, iboSize * sizeof(uint32_t));
		p->vao.Init(p->vbo.Id(), p->ibo.Id());

		p->vao.Bind();
		p->vao.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
		p->vao.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color));
		p->vao.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));
		p->vao.VertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
		p->vao.VertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, tangent));
		p->vao.UnBind();

		if (!p->vbo.Id() || !p->ibo.Id() || !p->vao.Id()){

			std::cout << "�o�b�t�@�̍쐬�Ɏ��s " << std::endl;
		}

		p->PushLevel();

		return p;
	}

	Buffer::~Buffer() {

		if (vao.Id()) {
			vao.Destroy();
		}
		if (ibo.Id()) {
			ibo.Destroy();
		}
		if (vbo.Id()) {
			vbo.Destroy();
		}
	}

	bool Buffer::LoadMeshFromFile(const char* filename) {

		FbxLoader loader;
		if (!loader.Load(filename)) {
			return false;
		}

		Level& level = levelStack.back();
		GLint64 vboSize = 0;
		GLint64 iboSize = 0;
		glBindBuffer(GL_ARRAY_BUFFER, vbo.Id());
		glGetBufferParameteri64v(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vboSize);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.Id());
		glGetBufferParameteri64v(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &iboSize);
		for (TemporaryMesh& e : loader.meshList) {
			for (TemporaryMaterial& material : e.materialList) {

				const GLsizeiptr verticesBytes = material.vertexBuffer.size() * sizeof(Vertex);
				if (level.vboEnd + verticesBytes >= vboSize) {
					std::cerr << "[Warning]: vbo�T�C�Y���s�����Ă��܂�(" << level.vboEnd << '/' << vboSize << ')' << std::endl;
					continue;
				}

				const GLsizei indexSize = static_cast<GLsizei>(material.indexBuffer.size());
				const GLsizeiptr indicesBytes = indexSize * sizeof(uint32_t);
				if (level.iboEnd + indicesBytes >= iboSize) {
					std::cerr << "[Warning]: ibo�T�C�Y���s�����Ă��܂�(" << level.iboEnd << '/' << iboSize << ')' << std::endl;
					continue;
				}

				glBufferSubData(GL_ARRAY_BUFFER, level.vboEnd, verticesBytes, material.vertexBuffer.data());
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, level.iboEnd, indicesBytes, material.indexBuffer.data());
				const GLint baseVertex = static_cast<uint32_t>(level.vboEnd / sizeof(Vertex));
				materialList.push_back({ GL_UNSIGNED_INT,indexSize,reinterpret_cast<GLvoid*>(level.iboEnd),baseVertex,material.color });
				level.vboEnd += verticesBytes;
				level.iboEnd += indicesBytes;
			}

			struct Impl : public Mesh {
				Impl(const std::string& n, size_t b, size_t e) :Mesh(n, b, e) {}
				~Impl() {}
			};
			const size_t endMaterial = materialList.size();
			const size_t beginMaterial = endMaterial - e.materialList.size();
			level.meshList.insert(std::make_pair(e.name, std::make_shared<Impl>(e.name, beginMaterial, endMaterial)));
		}
		return true;
	}

	const MeshPtr& Buffer::GetMesh(const char* name) const {

		static const MeshPtr dummy;
		if(!name){
			return dummy;
		}
		
		for (const auto& e : levelStack) {
			auto itr = e.meshList.find(name);
			if (itr != e.meshList.end()) {
				return itr->second;
			}
		}

		//�f�[�^��������Ȃ�����
		return dummy;
	}

	const Material& Buffer::GetMaterial(size_t index) const {

		if (index >= materialList.size()) {
			//�C���f�b�N�X�ɑΉ�����}�e���A���������̂Ń_�~�[���p
			static const Material dummy{ GL_UNSIGNED_BYTE,0,0,0,glm::vec4(1) };
			std::cout << "[Error]: MeshBuffer::GetMaterial() material index is out of range!" << std::endl;
			return dummy;
		}
		return materialList[index];
	}

	void Buffer::BindVAO() const {
		vao.Bind();
	}

	void Buffer::UnBindVAO() const{
		vao.UnBind();
	}

	void Buffer::PushLevel() {

		levelStack.push_back(Level());
		ClearLevel();
	}

	void Buffer::PopLevel() {

		Buffer::Level& currentLevel = levelStack.back();

		if (levelStack.size() > minimalStackSize) {
			levelStack.pop_back();
		}

		//���݂̃��x�����Ǘ����Ă���}�e���A�����X�g�͈͓̔����폜����
		if (!currentLevel.meshList.empty()) {
			std::vector<int> removeList;
			for (auto itr = currentLevel.meshList.begin(); itr != currentLevel.meshList.end(); itr++) {
				for (size_t i = itr->second->beginMaterial; i < itr->second->endMaterial; i++) {
					removeList.push_back(i);
				}
			}
			std::sort(removeList.begin(), removeList.end(), [&](int a, int b) {return a > b; });

			for (size_t i = 0; i < removeList.size(); i++) {

				materialList.erase(materialList.begin() + removeList[i]);

			}
		}
	}

	void Buffer::ClearLevel() {

		Level& currentLevel = levelStack.back();
		if (levelStack.size() <= minimalStackSize) {
			currentLevel.vboEnd = 0;
			currentLevel.iboEnd = 0;
		}
		else {
			const Level& prevLevel = levelStack[levelStack.size() - (minimalStackSize + 1)];
			currentLevel.vboEnd = prevLevel.vboEnd;
			currentLevel.iboEnd = prevLevel.iboEnd;
		}

		//���݂̃��x�����Ǘ����Ă���}�e���A�����X�g�͈͓̔����폜����
		std::vector<int> removeList;
		for (auto itr = currentLevel.meshList.begin(); itr != currentLevel.meshList.end(); itr++) {
			for (size_t i = itr->second->beginMaterial; i < itr->second->endMaterial;i++) {
				removeList.push_back(i);
			}
		}
		std::sort(removeList.begin(), removeList.end(), [&](int a, int b) {return a > b; });

		for (size_t i = 0; i < removeList.size(); i++) {

			materialList.erase(materialList.begin() + removeList[i]);
			
		}

		currentLevel.meshList.clear();
	}

}
