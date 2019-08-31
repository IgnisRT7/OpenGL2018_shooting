/**
*	@file CameraComponent.h
*	@brief	シーンで使用されるカメラコンポーネント
*	@author	takuya Yokoyama
*/
#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class SceneComponent;
using SceneComponentPtr = std::shared_ptr<SceneComponent>;

/**
*	シーンコンポーネント
*	TODO: ココに書くべき処理ではないため後に移行するように
*/
class SceneComponent {
public:

private:
};

/**
*	カメラのコンポーネントクラス
*/
class CameraComponent :public SceneComponent{
public:

	virtual const glm::mat4& ViewMatrix() const;
	virtual const glm::mat4& ProjctionMatrix() const;

	virtual void Initialize() {};

	/**
	*	更新処理
	*
	*	@param deltaTime	経過時間
	*/
	virtual void Update(float delta);

	void Position(glm::vec3& p) { position = p; }		
	void Direction(glm::vec3& d) { direction = d; }
	void UpVector(glm::vec3& u) { up = u; }
	void Fov(float f) { fov = f; }
	void Aspect(float a) { aspect = a; }
	void Near(float n) { near = n; }
	void Far(float f) { far = f; }

	void ViewMatrixParam(glm::vec3& p, glm::vec3& d, glm::vec3 u) { position = p; direction = d; up = u; }
	void ProjMatrixParam(float f, float a, float n, float fa) { far = f; aspect = a; near = n; far = fa; }
	void LookAt(glm::vec3 pos, glm::vec3 at) { position = pos; direction = glm::normalize(at - pos); }


protected:

	glm::vec3 position = glm::vec3(0, 0, 0);		/// カメラ座標
	glm::vec3 direction = glm::vec3(0, 0, 1);		/// カメラ向き
	glm::vec3 up = glm::vec3(0, 1, 0);			/// カメラ姿勢

	float fov = 45.0f;		/// 画角
	float aspect = 1.0f;	/// アスペクト比
	float near = 0.1f;		/// 最近点
	float far = 1000.0f;	/// 最遠点

	glm::mat4 matView;
	glm::mat4 matProj;
};

/**
*	デバッグ用　カメラ操作クラス
*/
class CameraDebugComponent : public CameraComponent {
public:

	CameraDebugComponent() = default;
	CameraDebugComponent(const CameraDebugComponent&) = delete;
	void operator=(const CameraDebugComponent&) = delete;

private:

	/**
	*	初期化処理
	*/
	void Initialize() override;

	/**
	*	更新処理
	*
	*	@param deltaTime	経過時間
	*/
	void Update(float delta) override;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 upLocal;

};




