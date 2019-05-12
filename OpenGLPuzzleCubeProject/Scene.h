/**
*	@Scene.h
*/

#pragma once
#include <memory>
#include <string>
#include <vector>

/**
*	�V�[���N���X
*/
class Scene {
public:

	Scene(const char* name);
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	virtual ~Scene() {}

	virtual bool Initialize() = 0 {};
	virtual void Update(float delta) = 0 {};
	virtual void Render() = 0 {};
	virtual void Finalize() = 0 {};

	virtual void Play();
	virtual void Stop();
	virtual void Hide();

	const std::string Name() const;

protected:

	std::string name;		///< �V�[���̖��O
	bool isActive = true;	///< �A�N�e�B�u��Ԃ��ǂ���
	bool isVisible = true;	///< �\�������ǂ���
};

using ScenePtr = std::shared_ptr<Scene>;

/**
*	�V�[���Ǘ��N���X
*/
class SceneStack {
public:

	static SceneStack& Instance();

	void PushScene(ScenePtr);
	void Pop();
	void Replace(ScenePtr);
	Scene& Current();
	const Scene& Current()const;
	size_t Size() const;
	bool Empty() const;

	void Update(float);
	void Render();

private:

	SceneStack();
	SceneStack(const SceneStack&) = delete;
	SceneStack& operator=(const SceneStack&) = delete;
	~SceneStack() = default;

	std::vector<ScenePtr> stack;

	std::vector<ScenePtr> buffer;

};
