/**
*	@Scene.h
*	@brief	�Q�[�����̃V�[���̊Ǘ��p
*	@author	tn-mai(�u�`���������)
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

	/**
	*	�R���X�g���N�^
	*
	*	@param name �V�[����
	*/
	Scene(const char* name = "NoName");
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	virtual ~Scene();

	/**
	*	����������
	*
	*	@retval true	����������
	*	@retval false	���������s
	*/
	virtual bool Initialize() = 0 {};

	/**
	*	�X�V����
	*
	*	@param deltaTime	�o�ߎ���
	*/
	virtual void Update(float deltaTime) = 0 {};

	//	virtual void Render() = 0 {};

	/**
	*	�I������
	*/
	virtual void Finalize() = 0 {};

	/**
	*	�V�[����������Ԃɂ���
	*/
	virtual void Play() = 0;

	/**
	*	�V�[�����~��Ԃɂ���
	*/
	virtual void Stop() = 0;

	/**
	*	�V�[�����\�������ɂ���
	*/
	virtual void Hide() = 0;

	/**
	*	�V�[�������擾����
	*
	*	@return �V�[����
	*/
	const std::string& Name() const { return name; }

	/**
	*	�V�[�����A�N�e�B�u���ǂ����𒲂ׂ�
	*
	*	@retval true	�A�N�e�B�u
	*	@retval false	��A�N�e�B�u
	*/
	bool IsActive() const { return isActive; }

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

	/**
	*	�V�[���X�^�b�N���擾����
	*
	*	@return �V�[���X�^�b�N�C���X�^���X
	*/
	static SceneStack& Instance();

	/**
	*	�V�[�����v�b�V������
	*
	*	@param scene	�V�����V�[��
	*/
	void Push(ScenePtr s);

	/**
	*	�V�[�����|�b�v����
	*/
	void Pop();

	/**
	*	�V�[����u��������
	*
	*	@param scene	�V�����V�[��
	*/
	void Replace(ScenePtr s);

	/**
	*	���݂̃V�[�����擾����
	*
	*	@return	���݂̃V�[��
	*/
	Scene& Current();

	/**
	*	���݂̃V�[�����擾����
	*
	*	@return	���݂̃V�[��
	*/
	const Scene& Current()const;

	/**
	*	�V�[���̐����擾����
	*
	*	@return	�X�^�b�N�ɐς܂�Ă���V�[���̐�
	*/
	size_t Size() const;

	/**
	*	�X�^�b�N���󂩂ǂ����𒲂ׂ�
	*
	*	@retval true	�X�^�b�N�͋�
	*	@retval false	�X�^�b�N��1�ȏ�̃V�[�����ς܂�Ă���
	*/
	bool Empty() const;

	/**
	*	�V�[�����X�V����
	*
	*	@param delta	�O�񂩂�̌o�ߎ���
	*/
	void Update(float);

private:

	SceneStack();
	SceneStack(const SceneStack&) = delete;
	SceneStack& operator=(const SceneStack&) = delete;
	~SceneStack() = default;

	std::vector<ScenePtr> stack;
	ScenePtr nextScne;
};
