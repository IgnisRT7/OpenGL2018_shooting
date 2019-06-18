/**
*	@file GLFWEW.cpp
*/
#include "GLFWEW.h"
#include <iostream>

///GLFWとGLEWをラップするための名前空間
namespace GLFWEW {

	/**
	*	GLFWからのエラー報告を処理する
	*
	*	@param error	エラーの番号
	*	@param desc		エラーの内容
	*/
	void ErrorCallback(int error, const char* desc) {

		std::cerr << "EEROR : " << desc << std::endl;
	}

	/**
	*	マウスのスクロール情報を取得する
	*
	*	@param window ウインドウハンドル
	*	@param xPos		X方向のスクロール情報
	*	@param yPos		Y方向のスクロール情報
	*/
	void GetScrollInfo(GLFWwindow* window, double xPos, double yPos) {
		
		Window::Instance().UpdateMouseWheel(static_cast<float>(xPos), static_cast<float>(yPos));
	}

	/**
	*	シングルトンインスタンスを取得する
	*
	*	@return Windowのシングルトンインスタンス
	*/
	Window& Window::Instance() {
		static Window instance;
		return instance;
	}

	/**
	*	コンストラクタ
	*/
	Window::Window() :
		isGLFWInitialized(false),
		isInitialized(false),
		window(nullptr) {

	}

	/**
	*	デストラクタ
	*/
	Window::~Window() {
		if (isGLFWInitialized) {
			glfwTerminate();
		}
	}

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
	bool Window::Init(int w, int h, const char* title) {

		if (isInitialized) {
			std::cerr << "ERROR: GLFWEWは既に初期化されています" << std::endl;
			return false;
		}

		if (!isGLFWInitialized) {
			glfwSetErrorCallback(ErrorCallback);

			if (glfwInit() != GL_TRUE) {
				return false;
			}
			isGLFWInitialized = true;
		}

		if (!window) {
			window = glfwCreateWindow(w, h, title, nullptr, nullptr);
			if (!window) {
				return false;
			}
			glfwMakeContextCurrent(window);
			glfwSetScrollCallback(window, GetScrollInfo);
		}

		if (glewInit() != GLEW_OK) {
			std::cerr << "ERROR: GLEWの初期化に失敗しました" << std::endl;
			return false;
		}

		//デフォルトのキーマップの設定コード
		SetKeyMap(std::vector<stKeyMap>() = {
			{ GLFW_KEY_W, GamePad::DPAD_UP },
			{ GLFW_KEY_A, GamePad::DPAD_LEFT },
			{ GLFW_KEY_S, GamePad::DPAD_DOWN },
			{ GLFW_KEY_D, GamePad::DPAD_RIGHT },
			{ GLFW_KEY_ENTER,GamePad::START},
			{ GLFW_KEY_SPACE,GamePad::A},
			{GLFW_KEY_ESCAPE,GamePad::ESCAPE},
			});

		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer: " << renderer << std::endl;
		const GLubyte* version = glGetString(GL_VERSION);
		std::cout << "Version: " << version << std::endl;

		windowSizeX = w;
		windowSizeY = h;

		return true;
	}

	/**
	*	ウインドウを閉じるべきか調べる
	*
	*	retval trueu 閉じる
	*	retval false 閉じない
	*/
	bool Window::ShouldClose() const {
		return glfwWindowShouldClose(window) != 0;
	}

	/**
	*	フロントバッファとバックバッファを切り替える
	*/
	void Window::SwapBuffers() const {
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	/**
	*	ゲームパッドの状態を取得する
	*
	*	@return ゲームパッドの状態
	*/
	const GamePad& Window::GetGamePad() const {
		return gamepad;
	}

	/**
	*	ジョイスティックのアナログ入力装置ID
	*
	*	@note XBOX360コントローラー基準
	*/
	enum GLFWAXESID {
		GLFWAXESID_LeftX,	///< 左スティックのX軸
		GLFWAXESID_LeftY,	///< 左スティックのY軸
		GLFWAXESID_BackX,	///< アナログトリガー
		GLFWAXESID_RightX,	///< 右スティックのX軸
		GLFWAXESID_RightY,	///< 右スティックのY軸
	};

	/**
	*	ジョイスティックのデジタル入力装置
	*
	*	@note XBOX360コントローラー基準
	*/
	enum {
		GLFWBUTTONID_A,		///< Aボタン
		GLFWBUTTONID_B,		///< Bボタン
		GLFWBUTTONID_X,		///< Xボタン
		GLFWBUTTONID_Y,		///< Yボタン
		GLFWBUTTONID_L,		///< Lボタン
		GLFWBUTTONID_R,		///< Rボタン
		GLFWBUTTONID_Back,	///< Backボタン
		GLFWBUTTONID_Start,	///< Startボタン
		GLFWBUTTONID_LThumb,///< 左スティックボタン
		GLFWBUTTONID_RThumb,///< 右スティックボタン
		GLFWBUTTONID_Up,	///< 上キー
		GLFWBUTTONID_Right,	///< 右キー
		GLFWBUTTONID_Down,	///< 下キー
		GLFWBUTTONID_Left,	///< 左キー
	};

	/**
	*	ゲームパッドの状態を更新する
	*/
	void Window::UpdateGamePad() {

		const uint32_t prevButtons = gamepad.buttons;
		const uint32_t prevMouseButtons = gamepad.mouseButtons;
		int axesCount, buttonCount;

		//軸入力とボタン入力の取得
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		const uint8_t* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

		if (axes && buttons && axesCount >= 2 && buttonCount >= 8) {
			///軸入力の処理

			gamepad.buttons &= (GamePad::DPAD_UP | GamePad::DPAD_DOWN | GamePad::DPAD_LEFT | GamePad::DPAD_RIGHT);

			static const float threshould = 0.3f;
			if (axes[GLFWAXESID_LeftY] >= threshould) {
				gamepad.buttons |= GamePad::DPAD_UP;
			}
			else if (axes[GLFWAXESID_LeftY] <= -threshould) {
				gamepad.buttons |= GamePad::DPAD_DOWN;
			}

			if (axes[GLFWAXESID_LeftX] >= threshould) {
				gamepad.buttons |= GamePad::DPAD_LEFT;
			}
			else if (axes[GLFWAXESID_LeftX] <= -threshould) {
				gamepad.buttons |= GamePad::DPAD_RIGHT;
			}

			static const struct {
				int glfwCode;
				uint32_t gamepadCode;
			} keyMap[] = {
				{ GLFWBUTTONID_A, GamePad::A },
				{ GLFWBUTTONID_B, GamePad::B },
				{ GLFWBUTTONID_X, GamePad::X },
				{ GLFWBUTTONID_Y, GamePad::Y },
				{ GLFWBUTTONID_Start, GamePad::START },
			};
			for (const auto& e : keyMap) {

				if (buttons[e.glfwCode] == GLFW_PRESS) {
					gamepad.buttons |= e.gamepadCode;
				}
				else if (buttons[e.glfwCode] == GLFW_RELEASE) {
					gamepad.buttons &= e.gamepadCode;
				}
			}
		}
		else {
			///キー入力の更新処理
			/*
			static const struct {
				int glfwCode;
				uint32_t gamepadCode;
			} keyMap[] = {
				{ GLFW_KEY_UP,GamePad::DPAD_UP },
				{ GLFW_KEY_DOWN,GamePad::DPAD_DOWN },
				{ GLFW_KEY_LEFT,GamePad::DPAD_LEFT },
				{ GLFW_KEY_RIGHT,GamePad::DPAD_RIGHT },
				{ GLFW_KEY_ENTER,GamePad::START },
				{ GLFW_KEY_A,GamePad::A | GamePad::DPAD_LEFT},
				{ GLFW_KEY_S,GamePad::B | GamePad::DPAD_DOWN},
				{ GLFW_KEY_W,GamePad::DPAD_UP},
				{ GLFW_KEY_D,GamePad::DPAD_RIGHT},
				{ GLFW_KEY_Z,GamePad::X },
				{ GLFW_KEY_X,GamePad::Y },
			};
			for (const auto& e : keyMap) {
				const int key = glfwGetKey(window, e.glfwCode);
				if (key == GLFW_PRESS) {
					gamepad.buttons |= e.gamepadCode;
				}
				else if (key == GLFW_RELEASE) {
					gamepad.buttons &= ~e.gamepadCode;
				}
			}*/

			for (auto itr = this->keyMap.begin(); itr != this->keyMap.end(); itr++) {
				
				//マップ内のキー(glfwのキーコード)の入力状態を取得
				const int key = glfwGetKey(window, itr->first);
				if (key == GLFW_PRESS) {
					gamepad.buttons |= itr->second;
				}
				else if (key == GLFW_RELEASE) {
					gamepad.buttons &= ~itr->second;
				}
			}
		}
		gamepad.buttonDown = gamepad.buttons & ~prevButtons;

		///マウスのボタン入力処理
		static const struct {
			int glfwMouseCode;
			uint32_t mouseCode;
		} mouseMap[] = {
			{ GLFW_MOUSE_BUTTON_LEFT,GamePad::MOUSE_LEFT_BUTTON},
			{ GLFW_MOUSE_BUTTON_RIGHT,GamePad::MOUSE_RIGHT_BUTTON},
		};
		for (auto& e : mouseMap){
			int key = glfwGetMouseButton(window,e.glfwMouseCode);
			if (key == GLFW_PRESS) {
				gamepad.mouseButtons |= e.mouseCode;
			}
			else if(key == GLFW_RELEASE){
				gamepad.mouseButtons &= e.mouseCode;
			}
		}
		gamepad.mouseButtonDown = gamepad.mouseButtons & ~prevMouseButtons;

		///マウスの座標更新処理
		double mousex, mousey;
		glfwGetCursorPos(window, &mousex, &mousey);
		glm::vec2 newMousePos = glm::vec2(static_cast<float>(mousex), static_cast<float>(mousey));

		gamepad.mouseVelocity = newMousePos - gamepad.mousePosition;
		gamepad.mousePosition = newMousePos;

	}

	/**
	*	マウスホイールの値の更新処理
	*
	*	@param x	ホイールXの回転量
	*	@param y	ホイールYの回転量
	*/
	void Window::UpdateMouseWheel(float x, float y){

		static glm::vec2 prev = glm::vec2(0);

		gamepad.mouseWheelY = y - prev.y;

		prev = glm::vec2(x, y);

	}

	/**
	*	キーのマッピングの設定
	*
	*	@param k キーマッピングのデータ
	*/
	void Window::SetKeyMap(std::vector<stKeyMap>& k){

		keyMap.clear();

		for (stKeyMap& keyValue : k) {
			
			keyMap[keyValue.glfwCode] = keyValue.keyCode;
		}
		
	}

	/**
	*	ウインドウサイズを取得します
	*
	*	@param w	ウインドウの横サイズの格納場所
	*	@param h	ウインドウの縦サイズの格納場所
	*
	*	return true	ウインドウサイズが変更された
	*	retrun false変更されていない
	*/
	bool Window::GetWindowSize(int& w, int& h){

		glfwGetWindowSize(window,&w, &h);

		if (w != windowSizeX || h != windowSizeY) {
			windowSizeX = w;
			windowSizeY = h;
			return true;
		}

		return false;
	}

} //namespace GLFWEW
