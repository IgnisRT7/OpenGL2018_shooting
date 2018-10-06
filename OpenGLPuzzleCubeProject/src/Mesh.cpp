/**
*	@file Mesh.cpp
*/
#include "Mesh.h"
#include <fbxsdk.h>
#include <iostream>

/**
*	モデルデータ管理のための名前空間
*/
namespace Mesh {

	/// 頂点データ型
	struct Vertex {

		glm::vec3 position;	///< 座標
		glm::vec4 color;	///< 色
		glm::vec2 texCoord;	///< テクスチャ座標
		glm::vec3 normal;	///< 法線
		glm::vec4 tangent;	///< 接ベクトル

	};

	/**
	*	Vertex Buffer Object	を作成する
	*
	*	@param size 頂点データのサイズ
	*	@param data 頂点データへのポインタ
	*
	*	@return 作成したVBO
	*/
	GLuint CreateVBO(GLsizeiptr size, const GLvoid* data) {

		GLuint vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return vbo;
	}

	/**
	*	Index Buffer Objectを作成する
	*
	*	@param size インデックスデータのサイズ
	*	@param data インデックスデータへのポインタ
	*
	*	@return 作成したIBO
	*/
	GLuint CreateIBO(GLsizeiptr size, const GLvoid* data) {

		GLuint ibo = 0;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return ibo;
	}

	/**
	*	頂点アトリビュートを設定する
	*
	*	@param index 頂点アトリビュートのインデックス
	*	@param cls	頂点データ型名
	*	@param mbr	頂点アトリビュートに設定するclsのメンバ変数名
	*/
#define SetVertexAttribPointer(index,cls,mbr) SetVertexAttribPointerI(\
	index,	\
sizeof(cls::mbr) / sizeof(float),	\
sizeof(cls),	\
reinterpret_cast<GLvoid*>(offsetof(cls,mbr)))

	void SetVertexAttribPointerI(GLuint index, GLint size, GLsizei stride, const GLvoid* pointer) {

		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, pointer);
	}

	/**
	*	Vertex Array Object を作成する
	*
	*	@param vbo VAOに関連付けられるVBO
	*	@param ibo VAOに関連付けられるIBO
	*
	*	@return 作成したVAO
	*/
	GLuint CreateVAO(GLuint vbo, GLuint ibo) {

		GLuint vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		SetVertexAttribPointer(0, Vertex, position);
		SetVertexAttribPointer(1, Vertex, color);
		SetVertexAttribPointer(2, Vertex, texCoord);
		SetVertexAttribPointer(3, Vertex, normal);
		SetVertexAttribPointer(4, Vertex, tangent); 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBindVertexArray(0);
		//glDeleteBuffers(1, &vbo);
		//glDeleteBuffers(1, &ibo);

		return vao;
	}

	/**
	*	FBXベクトルをglmベクトルに変換する
	*
	*	@param fbxVec FBXベクトル
	*
	*	@return glm::ベクトル
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
	*	頂点パラメータを取得する
	*
	*	@param mappingMode	使用するインデックスを選択するマッピングモード
	*	@param isDirectRef	直接参照モードなら真、間接参照モードなら偽を渡す
	*	@param pIndexList	間接参照用のインデックス配列。isDirectRefが偽の場合使われる
	*	@param pList		要素の配列
	*	@param cpIndex		マッピングモードが頂点単位の場合に使用するインデックス
	*	@param polygonVertex マッピングモードがポリゴン単位の場合に使用するインデックス
	*	@param defaultValue	未対応マッピングモードの場合に返す値
	*	
	*	@return マッピングモードと参照モードに応じて適切な配列から頂点のパラメータを返す
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
	*	FBXオブジェクトを破棄するためのヘルパー構造体
	*/
	template<typename T>
	struct Deleter {
		void operator()(T* p) { if (p) { p->Destroy(); } }
	};

	/**
	*	マテリアルの仮データ
	*/
	struct TemporaryMaterial {
		glm::vec4 color = glm::vec4(1);
		std::vector<uint32_t> indexBuffer;
		std::vector<Vertex> vertexBuffer;
		std::vector<std::string> textureName;
	};

	/**
	*	メッシュの仮データ
	*/
	struct TemporaryMesh {
		std::string name;
		std::vector<TemporaryMaterial> materialList;
	};

	/**
	*	FBXデータを中間データに変換するクラス
	*/
	struct FbxLoader {
		bool Load(const char* filename);
		bool Convert(FbxNode* node);
		bool LoadMesh(FbxNode* node);
		std::vector<TemporaryMesh> meshList;
	};

	/**
	*	FBXファイルを読み込む
	*
	*	@param filename FBXファイル名
	*
	*	@retval true	読み込み成功
	*	@retval false	読み込み失敗
	*/
	bool FbxLoader::Load(const char* filename) {
		
		std::unique_ptr<FbxManager, Deleter<FbxManager> > fbxManager(FbxManager::Create());
		if (!fbxManager) {
			std::cerr << "ERROR: " << filename << "の読み込みに失敗(FbxManager)の作成に失敗" << std::endl;
			return false;
		}

		FbxScene* fbxScene = FbxScene::Create(fbxManager.get(), "");
		if (!fbxScene) {
			std::cerr << "ERROR:" << filename << "の読み込みに失敗(FbxSceneの作成に失敗)" << std::endl;
			return false;
		}
		else {
			std::unique_ptr<FbxImporter, Deleter<FbxImporter> > fbxImporter(FbxImporter::Create(fbxManager.get(), ""));
			const bool importStatus = fbxImporter->Initialize(filename);
			if (!importStatus || !fbxImporter->Import(fbxScene)) {
				std::cerr << "ERROR: " << filename << "の読み込みに失敗\n" << fbxImporter->GetStatus().GetErrorString() << std::endl;
				return false;
			}
		}

		if (!Convert(fbxScene->GetRootNode())) {
			std::cerr << "ERROR: " << filename << "の変換に失敗" << std::endl;
			return false;
		}

		return true;
	}

	/**
	*	FBXデータを仮データに変換する
	*
	*	@param fbxNode 変換対象のFBXノードへのポインタ
	*
	*	@retval true	変換成功
	*	@retval false	変換失敗
	*/
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

	/**
	*	FBXメッシュを仮データに変換する
	*
	*	@param fbxNode 変換対象のFBXノードへのポインタ
	*
	*	@retval true	変換成功
	*	@retval false	変換失敗
	*/
	bool FbxLoader::LoadMesh(FbxNode* fbxNode) {

		FbxMesh* fbxMesh = fbxNode->GetMesh();
		if (!fbxMesh) {
			return true;
		}

		TemporaryMesh mesh;
		mesh.name = fbxNode->GetName();
		if (!fbxMesh->IsTriangleMesh()) {
			std::cerr << "Warning! :" << mesh.name << "には三角形以外のメンが含まれています" << std::endl;
		}

		//マテリアル情報を読み取る
		const int materialCount = fbxNode->GetMaterialCount();
		mesh.materialList.reserve(materialCount);
		for (int i = 0; i < materialCount; ++i) {

			TemporaryMaterial material;
			if (FbxSurfaceMaterial* fbxMaterial = fbxNode->GetMaterial(i)) {
				//マテリアルの色情報を読み取る
				const FbxClassId classId = fbxMaterial->GetClassId();
				if (classId == FbxSurfaceLambert::ClassId || classId == FbxSurfacePhong::ClassId) {
					const FbxSurfaceLambert* pLambert = static_cast<const FbxSurfaceLambert*>(fbxMaterial);
					material.color = glm::vec4(ToVec3(pLambert->Diffuse.Get()), static_cast<float>(1.0f - pLambert->TransparencyFactor));
				}
			}
			mesh.materialList.push_back(material);
		}

		if (mesh.materialList.empty()) {
			mesh.materialList.push_back(TemporaryMaterial());
		}

		//頂点要素の有無を調べる
		const bool hasColor = fbxMesh->GetElementVertexColorCount() > 0;
		const bool hasTexcoord = fbxMesh->GetElementUVCount() > 0;
		const bool hasNormal = fbxMesh->GetElementNormalCount() > 0;
		const bool hasTangent = fbxMesh->GetElementTangentCount() > 0;

		//UVセット名のリストを取得する
		FbxStringList uvSetNameList;
		fbxMesh->GetUVSetNames(uvSetNameList);

		//色情報を読み取る準備
		//@note 座標/UV/法線以外のパラメータには直接読み取る関数が提供されていないため、
		//		「FbxGeometryElement???」クラスから読み取る必要がある
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

		//接ベクトルを読み取る準備
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
			const FbxGeometryElementBinormal* fbxBinormalList = fbxMesh->GetElementBinormal();
			binormalMappingMode = fbxBinormalList->GetMappingMode();
			isBinormalDirectRef = fbxBinormalList->GetReferenceMode() == FbxLayerElement::eDirect;
			binormalIndexList = &fbxBinormalList->GetIndexArray();
			binormalList = &fbxBinormalList->GetDirectArray();
		}



		//頂点がどのマテリアルに属するかを示すマテリアルインデックスリストを取得する
		const FbxLayerElementArrayTemplate<int>* materialIndexList = nullptr;
		if (FbxGeometryElementMaterial* fbxMaterialLayer = fbxMesh->GetElementMaterial()) {
			materialIndexList = &fbxMaterialLayer->GetIndexArray();
		}

		//ポリゴン数に基づいて、仮データバッファの容量を予約する
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

				//頂点カラー
				v.color = glm::vec4(1);
				if (hasColor) {
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

				//UV座標
				v.texCoord = glm::vec2(0);
				if (hasTexcoord) {
					FbxVector2 uv;
					bool unmapped;
					fbxMesh->GetPolygonVertexUV(polygonIndex, pos, uvSetNameList[0], uv, unmapped);
					v.texCoord = ToVec2(uv);
				}

				//法線
				v.normal = glm::vec3(0, 0, 1);
				if (hasNormal) {
					FbxVector4 normal;
					fbxMesh->GetPolygonVertexNormal(polygonIndex, pos, normal);
					v.normal = glm::normalize(ToVec3(matR.MultT(normal)));
				}
				
				v.tangent = glm::vec4(1, 0, 0, 1);
				if (hasTangent) {

					//従法線ベクトル
					const glm::vec3 binormal = ToVec3(matR.MultT(GetElement(
						binormalMappingMode, isBinormalDirectRef, binormalIndexList,
						binormalList, cpIndex, polygonVertex, FbxVector4(0, 1, 0, 1))));

					//接ベクトル
					const glm::vec3 tangent = ToVec3(matR.MultT(GetElement(
						tangentMappingMode, isTangentDirectRef, tangentIndexList,
						tangentList, cpIndex, polygonVertex, FbxVector4(1, 0, 0, 1))));

					//仮の従法線ベクトル計算
					const glm::vec3 binormalTmp = glm::normalize(glm::cross(v.normal, tangent));
					
					if (glm::dot(binormal, binormalTmp) < 0) {
						v.tangent.w = -1;
					}
				}

				//頂点に対応する仮マテリアルに、頂点データとインデックスデータを追加する
				TemporaryMaterial& materialData = mesh.materialList[materialIndexList ? (*materialIndexList)[polygonIndex] : 0];
				materialData.indexBuffer.push_back(static_cast<uint32_t>(materialData.vertexBuffer.size()));
				materialData.vertexBuffer.push_back(v);

				++polygonVertex;
			}
		}

		meshList.push_back(std::move(mesh));
		return true;
	}

	/**
	*	コンストラクタ
	*
	*	@param meshName	メッシュデータ名
	*	@param begin	描画するマテリアルの先頭インデックス
	*	@param end		描画するマテリアルの終端インデックス
	*/
	Mesh::Mesh(const std::string& meshName, size_t begin, size_t end) :
		name(meshName), beginMaterial(begin), endMaterial(end) {

	}

	/**
	*	メッシュを描画する
	*
	*	@param buffer	描画に使用するメッシュバッファへのポインタ
	*/
	void Mesh::Draw(const BufferPtr& buffer) const {

		if (!buffer) {
			return;
		}

		if (buffer->GetMesh(name.c_str()).get() != this) {
			std::cerr << "WARNING: バッファに存在しないメッシュ'" << name << "'を描画しようとしました" << std::endl;
			return;
		}

		for (size_t i = beginMaterial; i < endMaterial; ++i) {
			const Material& m = buffer->GetMaterial(i);
			glDrawElementsBaseVertex(GL_TRIANGLES, m.size, m.type, m.offsest, m.baseVertex);
		}
	}

	/**
	*	メッシュバッファを作成する
	*
	*	@param vboSize	バッファに格納可能な総頂点数
	*	@param iboSize	バッファに格納可能な総インデックス数
	*/
	BufferPtr Buffer::Create(int vboSize, int iboSize) {

		struct Impl :Buffer { Impl() {} ~Impl() {} };
		BufferPtr p = std::make_shared<Impl>();
		p->vbo = CreateVBO(vboSize * sizeof(Vertex), nullptr);
		if (!p->vbo) {
			return {};
		}

		p->ibo = CreateIBO(iboSize * sizeof(uint32_t), nullptr);
		if (!p->ibo) {
			return {};
		}

		p->vao = CreateVAO(p->vbo, p->ibo);
		if (!p->vao) {
			return {};
		}

		p->PushLevel();

		return p;
	}

	/**
	*	デストラクタ
	*/
	Buffer::~Buffer() {

		if (vao) {
			glDeleteVertexArrays(1, &vao);
		}
		if (ibo) {
			glDeleteBuffers(1, &ibo);
		}
		if (vbo) {
			glDeleteBuffers(1, &vbo);
		}
	}

	/**
	*	メッシュをファイルから読み込む
	*
	*	@param filename メッシュファイル名
	*
	*	@retval true	読み込み成功
	*	@retval false	読み込み失敗
	*/
	bool Buffer::LoadMeshFromFile(const char* filename) {

		FbxLoader loader;
		if (!loader.Load(filename)) {
			return false;
		}

		Level& level = levelStack.back();
		GLint64 vboSize = 0;
		GLint64 iboSize = 0;
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glGetBufferParameteri64v(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &vboSize);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glGetBufferParameteri64v(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &iboSize);
		for (TemporaryMesh& e : loader.meshList) {
			for (TemporaryMaterial& material : e.materialList) {

				const GLsizeiptr verticesBytes = material.vertexBuffer.size() * sizeof(Vertex);
				if (level.vboEnd + verticesBytes >= vboSize) {
					std::cerr << "WARNING: vboサイズが不足しています(" << level.vboEnd << '/' << vboSize << ')' << std::endl;
					continue;
				}

				const GLsizei indexSize = static_cast<GLsizei>(material.indexBuffer.size());
				const GLsizeiptr indicesBytes = indexSize * sizeof(uint32_t);
				if (level.iboEnd + indicesBytes >= iboSize) {
					std::cerr << "WARNING: iboサイズが不足しています(" << level.iboEnd << '/' << iboSize << ')' << std::endl;
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

	/**
	*	メッシュを取得する
	*
	*	@param name メッシュ名
	*
	*	@return nameに対応するメッシュへのポインタ
	*/
	const MeshPtr& Buffer::GetMesh(const char* name) const {

		for (const auto& e : levelStack) {
			auto itr = e.meshList.find(name);
			if (itr != e.meshList.end()) {
				return itr->second;
			}
		}

		//データが見つからなかった
		static const MeshPtr dummy;
		return dummy;
	}

	/**
	*	マテリアルを取得する
	*
	*	@param index マテリアルインデックス
	*
	*	@return indexに対応するマテリアル
	*/
	const Material& Buffer::GetMaterial(size_t index) const {

		if (index >= materialList.size()) {
			//インデックスに対応するマテリアルが無いのでダミーを代用
			static const Material dummy{ GL_UNSIGNED_BYTE,0,0,0,glm::vec4(1) };
			return dummy;
		}
		return materialList[index];
	}

	/**
	*	バッファが保持するVAOをOpenGLの処理対象に設定する
	*/
	void Buffer::BindVAO() const {
		glBindVertexArray(vao);
	}

	/**
	*	スタックに新しいリソースレベルを作成する
	*/
	void Buffer::PushLevel() {

		levelStack.push_back(Level());
		ClearLevel();
	}

	/**
	*	スタックの末尾にあるリソースレベルを消去する
	*/
	void Buffer::PopLevel() {

		if (levelStack.size() > minimalStackSize) {
			levelStack.pop_back();
		}
	}

	/**
	*	末尾のリソースレベルを空の状態にする
	*/
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

		currentLevel.meshList.clear();
	}

}

