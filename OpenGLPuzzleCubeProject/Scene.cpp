/**
*	@file Scene.cpp
*/
#include "Scene.h"
#include <iostream>
#include "src/GameEngine.h"

/**
*	�R���X�g���N�^
*
*	@param name �V�[����
*/
Scene::Scene(const char* name) : name(name) {

	std::cout << "[ Scene::() ] " << name << std::endl;
}

/**
*	�f�X�g���N�^
*/
Scene::~Scene() {

	Finalize();
	std::cout << "[ Scene::~() ] " << name << std::endl;
}

/**
*	�V�[����������Ԃɂ���
*/
void Scene::Play() {

	isActive = true;
	std::cout << "[ Scene::Play ]" << name << " ���v���C�\�ɂȂ�܂���" << std::endl;
}

/**
*	�V�[�����~��Ԃɂ���
*/
void Scene::Stop() {

	isActive = false;
	std::cout << "[ Scene::Stop ]" << name << " ����~���܂��� " << std::endl;
}

/**
*	�V�[�������擾����
*
*	@return �V�[����
*/
const std::string& Scene::Name() const {

	return name;
}

/**
*	�V�[���X�^�b�N���擾����
*
*	@return �V�[���X�^�b�N
*/
SceneStack& SceneStack::Instance() {

	static SceneStack instance;
	return instance;
}

/**
*	�R���X�g���N�^
*/
SceneStack::SceneStack() {
	stack.reserve(16);
}

/**
*	�V�[�����v�b�V������
*
*	@param scene	�V�����V�[��
*/
void SceneStack::Push(ScenePtr scene) {

	GameEngine& game = GameEngine::Instance();

	if (!stack.empty()) {
		Current().Stop();
	}

	//�v�b�V�����邾���Ȃ̂Ń��\�[�X�͎c���Ă���
	game.RemoveAllEntity();
	game.PushLevel();

	stack.push_back(scene);
	std::cout << "[ SceneStack::Push ] " << scene->Name() << "(�V�[��)�� �v�b�V������܂���" << std::endl;
	Current().Initialize();
	Current().Play();
	std::cout << "[ SceneStack::Push ] " << scene->Name() << "(�V�[��)�̏��������������܂���" << std::endl;
}

/**
*	�V�[�����|�b�v����
*/
void SceneStack::Pop() {

	GameEngine& game = GameEngine::Instance();
	
	if (stack.empty()) {
		std::cout << "[ SceneStack::Pop] [�x��] �V�[���X�^�b�N����ł�" << std::endl;
		return;
	}

	//�N�����̃V�[���̒�~�ƏI������
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
*	�V�[����u��������
*
*	@param scene	�V�����V�[��
*/
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
		game.RemoveAllEntity();
		game.ClearLevel();
		
	}

	//�V�[���̒ǉ��Ə������Ǝ��s����
	stack.push_back(scene);
	std::cout << "[ SceneStack::Replace ]" << sceneName << "->" << scene->Name() << std::endl;
	Current().Initialize();
	Current().Play();
}

/**
*	���݂̃V�[�����擾����
*
*	@return	���݂̃V�[��
*/
Scene& SceneStack::Current() {
	return *stack.back();
}

/**
*	���݂̃V�[�����擾����
*
*	@return	���݂̃V�[��
*/
const Scene& SceneStack::Current() const {
	return *stack.back();
}

/**
*	�V�[���̐����擾����
*
*	@return	�X�^�b�N�ɐς܂�Ă���V�[���̐�
*/
size_t SceneStack::Size() const {

	return stack.size();
}

/**
*	�X�^�b�N���󂩂ǂ����𒲂ׂ�
*
*	@retval true	�X�^�b�N�͋�
*	@retval false	�X�^�b�N��1�ȏ�̃V�[�����ς܂�Ă���
*/
bool SceneStack::Empty() const {
	return stack.empty();
}

/**
*	�V�[�����X�V����
*
*	@param delta	�O�񂩂�̌o�ߎ���
*/
void SceneStack::Update(float delta) {

	/*for (ScenePtr& e : stack) {
		
		e->Update(deltaTime);
	}*/
	Current().Update(delta);
}





