/**
*	@file	GameEngine.cpp
*/

#pragma once

#include "GameEngine.h"
#include "GLFWEW.h"
#include "Audio.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "../../Res/Resource.h"
//#include "DebugLog.h"

///	最終出力用の頂点データ型
struct Vertex {
	glm::vec3 position;	///< 座標
	glm::vec4 color;	///< 色
	glm::vec2 texCoord;	///< テクスチャ座標
};

/// 最終出力用の頂点データ
const Vertex vertices[] = {
	{ { -0.5f, -0.3f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
{ { 0.3f, -0.3f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
{ { 0.3f,  0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
{ { -0.5f,  0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },

{ { -0.3f,  0.3f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },
{ { -0.3f, -0.5f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
{ { 0.5f, -0.5f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
{ { 0.5f, -0.5f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
{ { 0.5f,  0.3f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
{ { -0.3f,  0.3f, 0.1f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },

{ { -1.0f, -1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 0.0f } },
{ { 1.0f, -1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 0.0f } },
{ { 1.0f,  1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } },
{ { -1.0f,  1.0f, 0.5f },{ 1.0f, 1.0f, 1.0f, 1.0f },{ 0.0f, 1.0f } },
};

///最終出力用のインデックスデータ
const GLuint indices[] = {
	0, 1, 2, 2, 3, 0,
	4, 5, 6, 7, 8, 9,
	10, 11, 12, 12, 13, 10,
};

/**
*	部分描画データ
*/
struct RenderingPart {
	GLsizei size;	///< 描画するインデックス数
	GLvoid* offset;	///< 描画開始インデックスのバイトオフセット
};

/**
*	RenderingPartを作成する
*
*	@param size 描画するインデックス数
*	@param offset 描画開始インデックスのオフセット(インデックス単位)
*
*	@return 作成した部分描画オブジェクト
*/
constexpr RenderingPart MakeRenderingPart(GLsizei size, GLsizei offset) {
	return { size,reinterpret_cast<GLvoid*>(offset * sizeof(GLuint)) };
}

/**
*	部分描画データリスト
*/
static const RenderingPart renderingParts[] = {
	MakeRenderingPart(12,0),
	MakeRenderingPart(6,12),
};

GameEngine& GameEngine::Instance() {
	static GameEngine instance;
	return instance;
}

bool GameEngine::Init(int w, int h, const char* title) {

	if (isInitalized) {
		return true;
	}

	windowSize = glm::vec2(w, h);
	windowAspect = w / h;
	viewportRect[0] = glm::vec2(0, 0);
	viewportRect[1] = windowSize;

	if (!GLFWEW::Window::Instance().Init(w, h, title)) {
		return false;
	}

	//オフスクリーンバッファの頂点データ初期化
	vbo.Init(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	ibo.Init(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	vao.Init(vbo.Id(), ibo.Id());

	vao.Bind();
	vao.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
	vao.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color));
	vao.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));
	vao.UnBind();

	if (!ibo.Id() || !vbo.Id() || !vao.Id() ) {

		std::cerr << "[Error]: GameEngine::Init 初期化に失敗(MainBufferFaild)" << std::endl;
		return false;
	}

	///描画用バッファ群作成

	offscreen = OffscreenBuffer::Create(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), GL_RGBA16F);
	offDepth = OffscreenBuffer::Create(4096, 4096, GL_DEPTH_COMPONENT32);
	offStencil = OffscreenBuffer::Create(1024, 1024, GL_RGBA16F);
	
	bool offBloomFaild = false;
	for (int i = 0, scale = 4; i < bloomBufferCount; ++i, scale *= 4) {
		const int w = static_cast<int>(windowSize.x) / scale;
		const int h = static_cast<int>(windowSize.y) / scale;
		offBloom[i] = OffscreenBuffer::Create(w, h, GL_RGBA16F);
		if (!offBloom[i]) {
			offBloomFaild = true;
			break;
		}
	}

	//テクスチャサイズ取得
	int offWidth, offHeight;
	glBindTexture(GL_TEXTURE_2D, offBloom[bloomBufferCount - 1]->GetTexture());
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &offWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &offHeight);
	glBindTexture(GL_TEXTURE_2D, 0);

	//PBOを作成
	const int pboByteSize = offWidth * offHeight * sizeof(GLfloat) * 4;
	for (auto& e : pbo) {
		e.Init(GL_PIXEL_PACK_BUFFER, pboByteSize, nullptr, GL_DYNAMIC_READ);
	}

	if (!offscreen || !offDepth || !offStencil || offBloomFaild || !pbo->Id() || !pbo->Id()) {
		std::cerr << "[Error]: GameEngine::Init 初期化に失敗(OffscreenBuffersFaild)" << std::endl;
		return false;
	}

	//シェーダのコンパイル
	shaderMap.reserve(sizeof(Resource::shaderNameList) / sizeof(Resource::shaderNameList[0]));
	for (auto& e : Resource::shaderNameList) {

		std::string vPass = (std::string(Resource::shaderFolderPass) + e[1]);
		std::string fPass = (std::string(Resource::shaderFolderPass) + e[2]);

		Shader::ProgramPtr program = Shader::Program::Create(vPass.c_str(), fPass.c_str());
		if (!program) {
			return false;
		}
		shaderMap.insert(std::make_pair(std::string(e[0]), program));
	}

	//Uniformブロックのバインド
	shaderMap["Tutorial"]->UniformBlockBinding("VertexData", 0);
	shaderMap["Tutorial"]->UniformBlockBinding("LightData", 1);
	shaderMap["ColorFilter"]->UniformBlockBinding("PostEffectData", 2);
	shaderMap["HiLumExtract"]->UniformBlockBinding("PostEffectData", 2);

	uboLight = UniformBuffer::Create(sizeof(Uniform::LightData), 1, "LightData");
	uboPostEffect = UniformBuffer::Create(sizeof(Uniform::PostEffectData), 2, "PostEffectData");

	//meshBuffer作成
	meshBuffer = Mesh::Buffer::Create(1000 * 1024, 1000 * 1024);
	if (!meshBuffer) {
		std::cerr << "[Error]: GameEngine::Init 初期化に失敗" << std::endl;
	}

	textureStack.push_back(TextureLevel());

	//エンティティバッファの作成
	entityBuffer = Entity::Buffer::Create(1024, sizeof(Uniform::VertexData), 0, "VertexData");
	if (!entityBuffer) {
		std::cerr << "[Error]: GameEngineの初期化に失敗" << std::endl;
		return false;
	}

	//スプライトレンダラのバッファ作成
	spriteRenderer.Init(1000, shaderMap["Sprite"]);

	//TODO : 試作用カメラコンポーネント作成
	mainCamera = std::make_shared<CameraComponent>();
	mainCamera->Aspect(windowSize.x / windowSize.y);

	rand.seed(std::random_device()());

	fontRenderer.Init(1024, windowSize);

	isInitalized = true;
	return true;
}

void GameEngine::Run() {
	
	GLFWEW::Window& window = GLFWEW::Window::Instance();

	//メインループ
	while (!window.ShouldClose()) {

		int w, h;
		if (window.GetWindowSize(&w, &h)) {
			//ウインドウサイズが変更された

			CalculateViewPortByAspect(w, h, (800.0f / 600.0f));
		}

		//ウインドウシステム側の更新
		window.UpdateDeltaTime();
		deltaTime = window.DeltaTime();
		UpdateFps();
		window.UpdateGamePad();

		//アプリケーション側の更新処理
		spriteRenderer.BeginUpdate();

		if (!Update(window.DeltaTime())) {
			break;
		}

		spriteRenderer.EndUpdate();

		//描画処理
		Render();
		window.SwapBuffers();

		if (pboIndexForWriting < 0) {
			pboIndexForWriting = 0;
		}
		else {
			pboIndexForWriting ^= 1;
		}
	}
}

bool GameEngine::LoadTextureFromFile(const char* filename) {

	if (GetTexture(filename)) {
		return true;
	}

	TexturePtr texture = Texture::LoadFromFile(filename);
	if (!texture) {
		return false;
	}

	textureStack.back().insert(std::make_pair(std::string(filename), texture));
	return true;
}

bool GameEngine::LoadMeshFromFile(const char* filename) {
	return meshBuffer->LoadMeshFromFile(filename);
}

bool GameEngine::LoadFontFromFile(const char* filename){

	return fontRenderer.LoadFromFile(filename); 
}

Entity::Entity* GameEngine::AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, Entity::EntityDataBasePtr eData, const char* shader) {

	return AddEntity(groupId, pos, meshName, texName, nullptr, eData, shader);
}

Entity::Entity* GameEngine::AddEntity(int groupId, const glm::vec3& pos, const char* meshName, const char* texName, const char* normalName, Entity::EntityDataBasePtr eData, const char* shader) {
	
	decltype(shaderMap)::const_iterator itr = shaderMap.end();
	if (shader) {
		itr = shaderMap.find(shader);
	}
	if (itr == shaderMap.end()) {
		itr = shaderMap.find("Tutorial");
		if (itr == shaderMap.end()) {
			return nullptr;
		}
	}

	const Mesh::MeshPtr& mesh = meshBuffer->GetMesh(meshName);
	TexturePtr tex[2];
	if (meshName) {
		tex[0] = GetTexture(texName);
		if (normalName) {
			tex[1] = GetTexture(normalName);
		}
		else {
			tex[1] = GetTexture(Resource::tex_defaultNormal);
		}
	}
	return entityBuffer->AddEntity(groupId, pos, mesh, tex, itr->second, eData);
}

void GameEngine::AddSprite(Sprite &s){

	spriteRenderer.AddVertices(s);
}

void GameEngine::RemoveEntity(Entity::Entity* e) {

	entityBuffer->RemoveEntity(e);
}

void GameEngine::RemoveAllEntity() {

	entityBuffer->RemoveAllEntity();
}

void GameEngine::Light(int index, const Uniform::PointLight& light) {

	if (index<0 || index>Uniform::maxLightCount) {
		std::cerr << "[Warning]: GameEngine::Light '" << index << "'は不正なライトインデックスです" << std::endl;
		return;
	}
	lightData.light[index] = light;
}

const Uniform::PointLight& GameEngine::Light(int index)const {

	if (index<0 || index>Uniform::maxLightCount) {
		std::cerr << "[Warning]: '" << index << "'は不正なライトインデックスです" << std::endl;
		static const Uniform::PointLight dummy;
		return dummy;
	}

	return lightData.light[index];
}

void GameEngine::AmbientLight(const glm::vec4& color) {
	lightData.ambientColor = color;
}

const glm::vec4& GameEngine::AmbientLight() const {
	return lightData.ambientColor;
}

std::mt19937& GameEngine::Rand() {
	return rand;
}

const GamePad& GameEngine::GetGamePad() const {
	return GLFWEW::Window::Instance().GetGamePad();
}

void GameEngine::CollisionHandler(int gid0, int gid1) {
	entityBuffer->CollisionHandler(gid0, gid1);
}

void GameEngine::ClearCollisionHandlerList() {
	entityBuffer->ClearCollisionHanderList();
}

const TexturePtr& GameEngine::GetTexture(const char* filename) const {

	for (const auto& e : textureStack) {
		const auto itr = e.find(filename);
		if (itr != e.end()) {
			return itr->second;
		}
	}

	static const TexturePtr dummy;
	return dummy;
}

void GameEngine::CalculateViewPortByAspect(int width, int height,float aspect){

	windowAspect = aspect;
	float theoreticalWidth = height * windowAspect;	/// ウインドウサイズの高さを1としたときの横幅のサイズ(理論値)

	viewportRect[1].y = (theoreticalWidth > width) ? width * (1 / aspect) : height;
	viewportRect[1].x = viewportRect[1].y * aspect;

	viewportRect[0].x = (width - viewportRect[1].x) * 0.5f;
	viewportRect[0].y = (height - viewportRect[1].y) * 0.5f;
}

bool GameEngine::InitAudio(const char* acfPath, const char* acbPath, const char* awbPath, const char* dspBufName) {

	return Audio::Initialize(acfPath, acbPath, awbPath, dspBufName);
}

void GameEngine::PlayAudio(int playerId, int cueId) {

	Audio::Play(playerId, cueId);
}

void GameEngine::StopAudio(int playerId) {

	Audio::Stop(playerId);
}

void GameEngine::StopAllAudio() {

	Audio::StopAll();
}

void GameEngine::PushLevel() {

	meshBuffer->PushLevel();
	textureStack.push_back(TextureLevel());
}

void GameEngine::PopLevel() {

	meshBuffer->PopLevel();
	if (textureStack.size() > minimalStackSize) {
		textureStack.pop_back();
	}
}

void GameEngine::ClearLevel() {

	textureStack.back().clear();
	meshBuffer->ClearLevel();
}

void GameEngine::PushScene(ScenePtr s){
	
	SceneStack::Instance().Push(s);
}

void GameEngine::PopScene(){

	SceneStack::Instance().Pop();
}

void GameEngine::ReplaceScene(ScenePtr s){
	
	SceneStack::Instance().Replace(s);
}

GameEngine::~GameEngine() {

	Audio::Destroy();

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

bool GameEngine::Update(float delta) {

	if (GetGamePad().buttonDown & GamePad::ESCAPE) {
		return false;
	}

	float ratedDelta = delta * timeScale;

	SceneStack& sceneStack = SceneStack::Instance();

	//シーンのフェード演出処理
	if (isSceneFadeStart){
		if ((sceneFadeTimer -= ratedDelta) < 0) {

			isSceneFadeStart = false;
			sceneFadeTimer = 3.0f;
		}
	}

	fontRenderer.MapBuffer();

	sceneStack.Update(ratedDelta);

	glm::mat4 matProj;
	glm::mat4 matView[4];

	if (mainCamera) {
		//メインカメラからビュー・射影変換行列の情報を取得する

		mainCamera->Update(ratedDelta);

		matProj = mainCamera->ProjctionMatrix();
		matView[0] = mainCamera->ViewMatrix();
	}

	//シャドウの設定
	const glm::vec2 range = shadowParameter.range * 0.5f;
	const glm::mat4 matDepthProj = glm::ortho<float>(
		-range.x, range.x, -range.y, range.y, shadowParameter.near, shadowParameter.far);

	const glm::mat4 matDepthView = glm::lookAt(
		shadowParameter.lightPos, shadowParameter.lightPos + shadowParameter.lightDir, shadowParameter.lightUp);

	//エンティティバッファの更新
	entityBuffer->Update(ratedDelta, matView, matProj, matDepthProj * matDepthView);

	fontRenderer.UnmapBuffer();

	return true;
}

void GameEngine::UpdateFps(){

	// fps の更新処理
	fpsBuffer[++bufferCount % 60] = deltaTime;

	fps = 0;
	for (int i = 0; i < 60; i++) {
		fps += fpsBuffer[i];
	}

	fps /= 60.0f;
	fps = 1 / fps;
}

void GameEngine::RenderShadow() const {

	glBindFramebuffer(GL_FRAMEBUFFER, offDepth->GetFramebuffer());
	glClearDepth(1);
	glClear(GL_DEPTH_BUFFER_BIT);

	if (!isEnableShadow) {
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glDisable(GL_BLEND);
	glViewport(0, 0, offDepth->Width(), offDepth->Height());
	glScissor(0, 0, offDepth->Width(), offDepth->Height());

	const Shader::ProgramPtr& progDepth = shaderMap.find("RenderDepth")->second;
	progDepth->UseProgram();
	entityBuffer->DrawDepth(meshBuffer);

	glCullFace(GL_BACK);
}

void GameEngine::RenderStencil() const {

	glBindFramebuffer(GL_FRAMEBUFFER, offStencil->GetFramebuffer());
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!isDrawOutline) {
		//ステンシルバッファは現状アウトラインのみなので描画しない場合は切っておく
		return;
	}

	glViewport(0, 0, offStencil->Width(), offStencil->Height());
	glScissor(0, 0, offStencil->Width(), offStencil->Height());

	const Shader::ProgramPtr& progStencil = shaderMap.find("RenderStencil")->second;

	progStencil->UseProgram();
	progStencil->SetVectorParameter(glm::vec3(1, 0, 0), "stencilColor");
	entityBuffer->DrawStencil(meshBuffer,progStencil);

}

void GameEngine::RenderEntity() const {

	glBindFramebuffer(GL_FRAMEBUFFER, offscreen->GetFramebuffer());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, offscreen->Width(), offscreen->Height());
	glScissor(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));

	glClearColor(0.1f, 0.3f, 0.5f, 1.0f);
	glClearDepth(1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderMap.find("Tutorial")->second->BindShadowTexture(GL_TEXTURE_2D, offDepth->GetTexture());

	uboLight->BufferSubData(&lightData);

	//エンティティごとに描画に使用するシェーダが異なるためここでは設定しない
	entityBuffer->Draw(meshBuffer);
}

void GameEngine::RenderBloomEffect() const {

	///明るい部分の抽出処理

	glBindFramebuffer(GL_FRAMEBUFFER, offBloom[0]->GetFramebuffer());

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, offBloom[0]->Width(), offBloom[0]->Height());

	vao.Bind();

	const Shader::ProgramPtr& progHiLumExtract = shaderMap.find("HiLumExtract")->second;
	progHiLumExtract->UseProgram();

	progHiLumExtract->BindTexture(GL_TEXTURE0, offscreen->GetTexture());
	glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);

	///抽出したデータをぼかす

	const Shader::ProgramPtr& progShrink = shaderMap.find("Shrink")->second;
	progShrink->UseProgram();

	for (int i = 1; i < bloomBufferCount; ++i) {

		glBindFramebuffer(GL_FRAMEBUFFER, offBloom[i]->GetFramebuffer());
		glViewport(0, 0, offBloom[i]->Width(), offBloom[i]->Height());
		progShrink->BindTexture(GL_TEXTURE0, offBloom[i - 1]->GetTexture());
		glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);

	}

	///フレームバッファに対してぼかし加工を行います

	const Shader::ProgramPtr& progBlur3x3 = shaderMap.find("Blur3x3")->second;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	progBlur3x3->UseProgram();

	for (int i = bloomBufferCount - 1; i > 0; --i) {

		glBindFramebuffer(GL_FRAMEBUFFER, offBloom[i - 1]->GetFramebuffer());
		glViewport(0, 0, offBloom[i - 1]->Width(), offBloom[i - 1]->Height());
		progBlur3x3->BindTexture(GL_TEXTURE0, offBloom[i]->GetTexture());
		glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);
	}

	vao.UnBind();
}

void GameEngine::RenderFrameBuffer() const{

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDisable(GL_BLEND);
	glViewport(static_cast<int>(viewportRect[0].x), static_cast<int>(viewportRect[0].y),
		static_cast<int>(viewportRect[1].x), static_cast<int>(viewportRect[1].y));

	vao.Bind();

	const Shader::ProgramPtr& progColorFilter = shaderMap.find("ColorFilter")->second;
	progColorFilter->UseProgram();

	progColorFilter->SetFloatParameter(isSceneFadeStart ? (sceneFadeTimer / 3) : 1, "scenFadeTimerRate");
	progColorFilter->SetBoolParameter(isDrawOutline, "isDrawOutline");

	Uniform::PostEffectData postEffect;
	postEffect.luminanceScale = luminanceScale;
	postEffect.bloomThreshold = 1.0f / luminanceScale;
	uboPostEffect->BufferSubData(&postEffect);
	progColorFilter->BindTexture(GL_TEXTURE0, offscreen->GetTexture());
	progColorFilter->BindTexture(GL_TEXTURE1, offBloom[0]->GetTexture());
	progColorFilter->BindTexture(GL_TEXTURE2, offDepth->GetTexture());

	glDrawElements(GL_TRIANGLES, renderingParts[1].size, GL_UNSIGNED_INT, renderingParts[1].offset);
	vao.UnBind();
}

void GameEngine::Render() {

	RenderShadow(); 

	RenderStencil();

	RenderEntity();

	RenderBloomEffect();
	
	RenderFrameBuffer();

	spriteRenderer.Draw(this->windowSize);

	fontRenderer.Draw();

	{
		// オフスクリーンバッファの大きさを取得.
		int width, height;
		glBindTexture(GL_TEXTURE_2D, offBloom[bloomBufferCount - 1]->GetTexture());
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
		glBindTexture(GL_TEXTURE_2D, 0);

		// 初回(pboIndexForWriting<0の場合)はまだデータがないため、
		// 変換コピーするだけで輝度計算はしない.
		if (pboIndexForWriting < 0) {

			// オフスクリーンバッファの内容をPBOに変換コピー
			const GLuint pboWrite = pbo[1].Id();
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pboWrite);
			glBindFramebuffer(GL_FRAMEBUFFER, offBloom[bloomBufferCount - 1]->GetFramebuffer());
			glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, 0);

		}
		else {

			// オフスクリーンバッファの内容をPBOに変換コピー.
			const GLuint pboWrite = pbo[pboIndexForWriting].Id();
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pboWrite);
			glBindFramebuffer(GL_FRAMEBUFFER, offBloom[bloomBufferCount - 1]->GetFramebuffer());
			glReadPixels(0, 0, width, height, GL_RGBA, GL_FLOAT, 0);

			// PBOの内容を読み取って輝度スケールを計算.
			const GLuint pboRead = pbo[pboIndexForWriting ^ 1].Id();
			glBindBuffer(GL_PIXEL_PACK_BUFFER, pboRead);
			const GLfloat* p = static_cast<GLfloat*>(glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
			float totalLum = 0;
			for (int i = 0; i < width * height; ++i) {
				totalLum += p[i * 4 + 3];
			}
			luminanceScale = 0;
			luminanceScale = keyValue / std::exp(totalLum / static_cast<float>(width * height));
			glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	}
}

