/**
*	@file EnemyLaunchController.h
*/

#include <vector>

struct EnemyLaunchType {

	int launchCount;	///< �o����
	float launchInterval;	///< �o���Ԋu
	int enemyType;		///< �G�̎��
	float launchStartTimer;	///< �X�e�[�W�J�n��ɏo�����鎞��
};

/**
*	1�X�e�[�W���̓G�̏o���Ǘ��N���X
*/
class EnemyLaunchController {
public:

	EnemyLaunchController();
	~EnemyLaunchController();
	EnemyLaunchController(const EnemyLaunchController&) = delete;
	void operator=(const EnemyLaunchController&) = delete;

	void Init(int stageNum);

	void Update(float delta);

private:

	std::vector<EnemyLaunchType> launchList;



};
