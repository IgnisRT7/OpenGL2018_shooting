/**
*	@file Scene.cpp
*/
#include "Scene.h"
#include <iostream>
#include "src/GameEngine.h"

/**
*	コンストラクタ
*
*	@param name シーン名
*/
Scene::Scene(const char* name) : name(name) {

	std::cout << "[ Scene::() ] " << name << std::endl;
}

/**
*	デストラクタ
*/
Scene::~Scene() {

	Finalize();
	std::cout << "[ Scene::~() ] " << name << std::endl;
}

/**
*	シーンを活動状態にする
*/
void Scene::Play() {

	isActive = true;
	std::cout << "[ Scene::Play ]" << name << " がプレイ可能になりました" << std::endl;
}

/**
*	シーンを停止状態にする
*/
void Scene::Stop() {

	isActive = false;
	std::cout << "[ Scene::Stop ]" << name << " が停止しました " << std::endl;
}

/**
*	シーン名を取得する
*
*	@return シーン名
*/
const std::string& Scene::Name() const {

	return name;
}

/**
*	シーンスタックを取得する
*
*	@return シーンスタック
*/
SceneStack& SceneStack::Instance() {

	static SceneStack instance;
	return instance;
}

/**
*	コンストラクタ
*/
SceneStack::SceneStack() {
	stack.reserve(16);
}

/**
*	シーンをプッシュする
*
*	@param scene	新しいシーン
*/
void SceneStack::Push(ScenePtr scene) {

	GameEngine& game = GameEngine::Instance();

	if (!stack.empty()) {
		Current().Stop();
	}

	//プッシュするだけなのでリソースは残しておく
	game.RemoveAllEntity();
	game.PushLevel();

	stack.push_back(scene);
	std::cout << "[ SceneStack::Push ] " << scene->Name() << "(シーン)が プッシュされました" << std::endl;
	Current().Initialize();
	Current().Play();
	std::cout << "[ SceneStack::Push ] " << scene->Name() << "(シーン)の初期化が完了しました" << std::endl;
}

/**
*	シーンをポップする
*/
void SceneStack::Pop() {

	GameEngine& game = GameEngine::Instance();
	
	if (stack.empty()) {
		std::cout << "[ SceneStack::Pop] [警告] シーンスタックが空です" << std::endl;
		return;
	}

	//起動中のシーンの停止と終了処理
	Current().Stop();
	Current().Finalize();

	game.RemoveAllEntity();
	game.ClearCollisionHandlerList();
	game.PopLevel();

	const std::string sceneName = Current().Name();
	stack.pop_back();
	std::cout << "[SceneStack::Pop ] "<< sceneName << std::endl;

	if (!stack.empty()) {
		Current().Play();
	}
}

/**
*	シーンを置き換える
*
*	@param scene	新しいシーン
*/
void SceneStack::Replace(ScenePtr scene) {

	std::string sceneName = "(empty)";
	if (stack.empty()) {
		std::cout << "[ SceneStack::Replace ] [警告]シーンスタックが空です" << std::endl;
	}
	else {

		sceneName = Current().Name();

		//起動中のシーンの停止と終了処理
		Current().Stop();
		Current().Finalize();
		stack.pop_back();

		
		GameEngine& game = GameEngine::Instance();
		game.RemoveAllEntity();
		game.ClearLevel();
		
	}

	//シーンの追加と初期化と実行処理
	stack.push_back(scene);
	std::cout << "[ SceneStack::Replace ]" << sceneName << "->" << scene->Name() << std::endl;
	Current().Initialize();
	Current().Play();
}

/**
*	現在のシーンを取得する
*
*	@return	現在のシーン
*/
Scene& SceneStack::Current() {
	return *stack.back();
}

/**
*	現在のシーンを取得する
*
*	@return	現在のシーン
*/
const Scene& SceneStack::Current() const {
	return *stack.back();
}

/**
*	シーンの数を取得する
*
*	@return	スタックに積まれているシーンの数
*/
size_t SceneStack::Size() const {

	return stack.size();
}

/**
*	スタックが空かどうかを調べる
*
*	@retval true	スタックは空
*	@retval false	スタックに1つ以上のシーンが積まれている
*/
bool SceneStack::Empty() const {
	return stack.empty();
}

/**
*	シーンを更新する
*
*	@param delta	前回からの経過時間
*/
void SceneStack::Update(float delta) {

	/*for (ScenePtr& e : stack) {
		
		e->Update(deltaTime);
	}*/
	Current().Update(delta);
}





