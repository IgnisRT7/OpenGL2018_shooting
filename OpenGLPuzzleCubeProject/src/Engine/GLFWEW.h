/**
*	@file GLFWEW.h
*	@brief	ゲーム外システム部分の制御用
*	@brief	Takuya Yokoyama , tn-mai
*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GamePad.h"
#include <map>
#include <vector>

#define GL_DEBUG_STRING(str) std::cerr << str;

namespace GLFWEW {

	struct stKeyMap {
		int glfwCode;		/// GLEW上でのコード定義
		uint32_t keyCode;	/// 実際に使用されるビット値
	};

	/**
	*	GLFWとGLEWのラッパークラス
	*/
	class Window {
	public:

		/**
		*	シングルトンインスタンスを取得する
		*
		*	@return Windowのシングルトンインスタンス
		*/
		static Window& Instance();

		/**
		*	GLFW/GLEWの初期化
		*
		*	@param w ウインドウの描画範囲の幅(ピクセル)
		*	@param h ウインドウの描画範囲の高さ(ピクセル)
		*	@param title ウインドウタイトル(UTF-8の0終端文字列)
		*
		*	@retval true 初期化成功
		*	@retval false 初期化失敗
		*/
		bool Init(int w, int h, const char* title);

		/**
		*	ウインドウを閉じるべきか調べる
		*
		*	retval trueu 閉じる
		*	retval false 閉じない
		*/
		bool ShouldClose() const;

		/**
		*	フロントバッファとバックバッファを切り替える
		*/
		void SwapBuffers() const;

		/**
		*	ゲームパッドの状態を取得する
		*
		*	@return ゲームパッドの状態
		*/
		const GamePad& GetGamePad() const;

		/**
		*	ウインドウインスタンスの取得
		*
		*	@return ウインドウインスタンス
		*/
		GLFWwindow& GetWindow()const { return *window; }

		/**
		*	経過時間の取得
		*
		*	@return 経過時間
		*/
		const float DeltaTime() const { return deltaTime; }

		/**
		*	前回からの経過時間の更新処理
		*/
		void UpdateDeltaTime();

		/**
		*	ゲームパッドの状態を更新する
		*/
		void UpdateGamePad();

		/**
		*	マウスホイールの値の更新処理
		*
		*	@param x	ホイールXの回転量
		*	@param y	ホイールYの回転量
		*/
		void UpdateMouseWheel(float x, float y);

		/**
		*	ホイール情報のクリア処理
		*/
		void ClearWheel() { gamepad.mouseWheelY = 0; }

		/**
		*	キーのマッピングの設定
		*
		*	@param k キーマッピングのデータ
		*/
		void SetKeyMap(std::vector<stKeyMap>& k);

		/**
		*	ウインドウサイズを取得します
		*
		*	@param w	ウインドウの横サイズの格納場所
		*	@param h	ウインドウの縦サイズの格納場所
		*
		*	return true	ウインドウサイズが変更された
		*	retrun false変更されていない
		*/
		bool GetWindowSize(int* w, int* h);

	private:

		Window();
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool isGLFWInitialized;
		bool isInitialized;
		GLFWwindow* window;
		GamePad gamepad;

		int windowSizeX;
		int windowSizeY;

		double prevTime;
		double nowTime;
		float deltaTime=0;

		std::map<int, uint32_t> keyMap;


	};



}	// namespace GLFWEW




