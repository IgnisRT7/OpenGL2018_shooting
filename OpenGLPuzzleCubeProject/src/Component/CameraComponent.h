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
*	TODO: ココに書くべき処理ではないため後に移行予定
*/
class SceneComponent {
public:

	SceneComponent() = default;
	SceneComponent(const SceneComponent&) = delete;
	virtual ~SceneComponent() = default;
	const SceneComponent& operator=(const SceneComponent&) = delete;

private:
};

/**
*	カメラのコンポーネントクラス
*/
class CameraComponent :public SceneComponent{
public:

	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = delete;
	virtual ~CameraComponent() = default;
	const CameraComponent& operator=(const CameraComponent&) = delete;
	
	/**
	*	ビュー変換行列の取得
	*
	*	@return ビュー変換行列
	*/
	virtual const glm::mat4& ViewMatrix() const;

	/**
	*	プロジェクション変換行列の取得
	*
	*	@return プロジェクション変換行列
	*/
	virtual const glm::mat4& ProjctionMatrix() const;

	/**
	*	初期化処理
	*/
	virtual void Initialize() {};

	/**
	*	更新処理
	*
	*	@param deltaTime	経過時間
	*/
	virtual void Update(float delta);

	/**
	*	カメラの位置情報の設定
	*
	*	@return p	設定するカメラの座標
	*/
	void Position(glm::vec3& p) { position = p; }		

	/**
	*	カメラの見ている方向の設定
	*
	*	@param d	カメラの方向ベクトル
	*/
	void Direction(glm::vec3& d) { direction = d; }

	/**
	*	カメラの上方向ベクトルの設定
	*
	*	@param u	カメラの上方向ベクトル
	*/
	void UpVector(glm::vec3& u) { up = u; }

	/**
	*	カメラの画角の設定
	*
	*	@param f	画角
	*/
	void Fov(float f) { fov = f; }

	/**
	*	アスペクト比の設定
	*
	*	@param a	アスペクト比
	*/
	void Aspect(float a) { aspect = a; }

	/**
	*	カメラのNear距離を設定する
	*
	*	@param n	Near値
	*/
	void Near(float n) { near = n; }

	/**
	*	カメラのFar距離を設定する
	*
	*	@param f	Far値
	*/
	void Far(float f) { far = f; }

	/**
	*	カメラのビューパラメータの一括設定
	*
	*	@param p	座標
	*	@param d	方向ベクトル
	*	@param u	上方向ベクトル
	*/
	void ViewMatrixParam(glm::vec3& p, glm::vec3& d, glm::vec3 u) { position = p; direction = d; up = u; }

	/**
	*	カメラのプロジェクションパラメータの一括設定
	*
	*	@param fo	画角
	*	@param a	アスペクト比
	*	@param n	Near値
	*	@param fa	Far値
	*/
	void ProjMatrixParam(float fo, float a, float n, float fa) { fov = fo; aspect = a; near = n; far = fa; }

	/**
	*	カメラの位置とみている方向から方向ベクトルを設定
	*	
	*	@param pos	カメラがある座標
	*	@param at	カメラの注視点
	*/
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
	~CameraDebugComponent() = default;
	const CameraDebugComponent& operator=(const CameraDebugComponent&) = delete;

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

private:

	glm::vec3 forward;	/// 前方向ベクトル
	glm::vec3 right;	/// 右方向ベクトル
	glm::vec3 upLocal;	/// 上方向ベクトル(ローカル)

};




