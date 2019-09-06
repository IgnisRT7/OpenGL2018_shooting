/**
*	@Scene.h
*	@brief	ゲーム内のシーンの管理用
*	@author	tn-mai(講義資料製作者)
*/

#pragma once
#include <memory>
#include <string>
#include <vector>

/**
*	シーンクラス
*/
class Scene {
public:

	/**
	*	コンストラクタ
	*
	*	@param name シーン名
	*/
	Scene(const char* name = "NoName");
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	virtual ~Scene();

	/**
	*	初期化処理
	*
	*	@retval true	初期化成功
	*	@retval false	初期化失敗
	*/
	virtual bool Initialize() = 0 {};

	/**
	*	更新処理
	*
	*	@param deltaTime	経過時間
	*/
	virtual void Update(float deltaTime) = 0 {};

	//	virtual void Render() = 0 {};

	/**
	*	終了処理
	*/
	virtual void Finalize() = 0 {};

	/**
	*	シーンを活動状態にする
	*/
	virtual void Play() = 0;

	/**
	*	シーンを停止状態にする
	*/
	virtual void Stop() = 0;

	/**
	*	シーンを非表示処理にする
	*/
	virtual void Hide() = 0;

	/**
	*	シーン名を取得する
	*
	*	@return シーン名
	*/
	const std::string& Name() const { return name; }

	/**
	*	シーンがアクティブかどうかを調べる
	*
	*	@retval true	アクティブ
	*	@retval false	非アクティブ
	*/
	bool IsActive() const { return isActive; }

protected:

	std::string name;		///< シーンの名前
	bool isActive = true;	///< アクティブ状態かどうか
	bool isVisible = true;	///< 表示中かどうか
};

using ScenePtr = std::shared_ptr<Scene>;

/**
*	シーン管理クラス
*/
class SceneStack {
public:

	/**
	*	シーンスタックを取得する
	*
	*	@return シーンスタックインスタンス
	*/
	static SceneStack& Instance();

	/**
	*	シーンをプッシュする
	*
	*	@param scene	新しいシーン
	*/
	void Push(ScenePtr s);

	/**
	*	シーンをポップする
	*/
	void Pop();

	/**
	*	シーンを置き換える
	*
	*	@param scene	新しいシーン
	*/
	void Replace(ScenePtr s);

	/**
	*	現在のシーンを取得する
	*
	*	@return	現在のシーン
	*/
	Scene& Current();

	/**
	*	現在のシーンを取得する
	*
	*	@return	現在のシーン
	*/
	const Scene& Current()const;

	/**
	*	シーンの数を取得する
	*
	*	@return	スタックに積まれているシーンの数
	*/
	size_t Size() const;

	/**
	*	スタックが空かどうかを調べる
	*
	*	@retval true	スタックは空
	*	@retval false	スタックに1つ以上のシーンが積まれている
	*/
	bool Empty() const;

	/**
	*	シーンを更新する
	*
	*	@param delta	前回からの経過時間
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
