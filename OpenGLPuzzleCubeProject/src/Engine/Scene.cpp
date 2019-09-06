/**
*	@file Scene.cpp
*/
#include "Scene.h"
#include <iostream>
#include "GameEngine.h"

Scene::Scene(const char* name) : name(name) {

	std::cout << "[Info]: Scene '" << name << "' のコンストラクタ" << std::endl;
}

Scene::~Scene() {

	Finalize();
	std::cout << "[Info]: Scene '" << name << "' のデストラクタ" << std::endl;
}

void Scene::Play() {

	isActive = true;
	std::cout << "[ Scene::Play ]" << name << " がプレイ可能になりました" << std::endl;
}

void Scene::Stop() {

	isActive = false;
	std::cout << "[ Scene::Stop ]" << name << " が停止しました " << std::endl;
}

SceneStack& SceneStack::Instance() {

	static SceneStack instance;
	return instance;
}

SceneStack::SceneStack() {
	stack.reserve(16);
}

void SceneStack::Push(ScenePtr scene) {

	GameEngine& game = GameEngine::Instance();

	if (!stack.empty()) {
		Current().Stop();
	}

	//プッシュするだけなのでリソースは残しておく
	game.StopAllAudio();
	game.RemoveAllEntity();
	game.PushLevel();

	stack.push_back(scene);
	std::cout << "[ SceneStack::Push ] " << scene->Name() << "(シーン)が プッシュされました" << std::endl;
	Current().Initialize();
	Current().Play();
	std::cout << "[ SceneStack::Push ] " << scene->Name() << "(シーン)の初期化が完了しました" << std::endl;
}

void SceneStack::Pop() {

	GameEngine& game = GameEngine::Instance();
	
	if (stack.empty()) {
		std::cout << "[ SceneStack::Pop] [警告] シーンスタックが空です" << std::endl;
		return;
	}

	//起動中のシーンの停止と終了処理
	Current().Stop();
	Current().Finalize();
	
	game.StopAllAudio();
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
		game.StopAllAudio();
		game.RemoveAllEntity();
		game.ClearLevel();
		
	}

	//シーンの追加と初期化と実行処理
	stack.push_back(scene);
	std::cout << "[ SceneStack::Replace ]" << sceneName << "->" << scene->Name() << std::endl;
	Current().Initialize();
	Current().Play();
}

Scene& SceneStack::Current() {
	return *stack.back();
}

const Scene& SceneStack::Current() const {
	return *stack.back();
}

size_t SceneStack::Size() const {

	return stack.size();
}

bool SceneStack::Empty() const {
	return stack.empty();
}

void SceneStack::Update(float delta) {

	/*for (ScenePtr& e : stack) {
		
		e->Update(deltaTime);
	}*/
	Current().Update(delta);
}





