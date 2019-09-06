/**
*	@file Scene.cpp
*/
#include "Scene.h"
#include <iostream>
#include "GameEngine.h"

Scene::Scene(const char* name) : name(name) {

	std::cout << "[Info]: Scene '" << name << "' �̃R���X�g���N�^" << std::endl;
}

Scene::~Scene() {

	Finalize();
	std::cout << "[Info]: Scene '" << name << "' �̃f�X�g���N�^" << std::endl;
}

void Scene::Play() {

	isActive = true;
	std::cout << "[ Scene::Play ]" << name << " ���v���C�\�ɂȂ�܂���" << std::endl;
}

void Scene::Stop() {

	isActive = false;
	std::cout << "[ Scene::Stop ]" << name << " ����~���܂��� " << std::endl;
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

	//�v�b�V�����邾���Ȃ̂Ń��\�[�X�͎c���Ă���
	game.StopAllAudio();
	game.RemoveAllEntity();
	game.PushLevel();

	stack.push_back(scene);
	std::cout << "[ SceneStack::Push ] " << scene->Name() << "(�V�[��)�� �v�b�V������܂���" << std::endl;
	Current().Initialize();
	Current().Play();
	std::cout << "[ SceneStack::Push ] " << scene->Name() << "(�V�[��)�̏��������������܂���" << std::endl;
}

void SceneStack::Pop() {

	GameEngine& game = GameEngine::Instance();
	
	if (stack.empty()) {
		std::cout << "[ SceneStack::Pop] [�x��] �V�[���X�^�b�N����ł�" << std::endl;
		return;
	}

	//�N�����̃V�[���̒�~�ƏI������
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
		std::cout << "[ SceneStack::Replace ] [�x��]�V�[���X�^�b�N����ł�" << std::endl;
	}
	else {

		sceneName = Current().Name();

		//�N�����̃V�[���̒�~�ƏI������
		Current().Stop();
		Current().Finalize();
		stack.pop_back();

		
		GameEngine& game = GameEngine::Instance();
		game.StopAllAudio();
		game.RemoveAllEntity();
		game.ClearLevel();
		
	}

	//�V�[���̒ǉ��Ə������Ǝ��s����
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





