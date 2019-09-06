/**
*	@file GLFWEW.cpp
*	@brief	�Q�[���O�V�X�e�������̐���p
*	@brief	Takuya Yokoyama , tn-mai
*/
#include "GLFWEW.h"
#include <iostream>

///GLFW��GLEW�����b�v���邽�߂̖��O���
namespace GLFWEW {

	/**
	*	GLFW����̃G���[�񍐂���������
	*
	*	@param error	�G���[�̔ԍ�
	*	@param desc		�G���[�̓��e
	*/
	void ErrorCallback(int error, const char* desc) {

		std::cerr << "EEROR : " << desc << std::endl;
	}

	/**
	*	�}�E�X�̃X�N���[�������擾����
	*
	*	@param window �E�C���h�E�n���h��
	*	@param xPos		X�����̃X�N���[�����
	*	@param yPos		Y�����̃X�N���[�����
	*/
	void GetScrollInfo(GLFWwindow* window, double xPos, double yPos) {
		
		Window::Instance().UpdateMouseWheel(static_cast<float>(xPos), static_cast<float>(yPos));
	}

	Window& Window::Instance() {
		static Window instance;
		return instance;
	}

	Window::Window() :
		isGLFWInitialized(false),
		isInitialized(false),
		window(nullptr) {

	}

	Window::~Window() {
		if (isGLFWInitialized) {
			glfwTerminate();
		}
	}

	bool Window::Init(int w, int h, const char* title) {

		if (isInitialized) {
			std::cerr << "ERROR: GLFWEW�͊��ɏ���������Ă��܂�" << std::endl;
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
			std::cerr << "ERROR: GLEW�̏������Ɏ��s���܂���" << std::endl;
			return false;
		}

		//�f�t�H���g�̃L�[�}�b�v�̐ݒ�R�[�h
		SetKeyMap(std::vector<stKeyMap>() = {
			{ GLFW_KEY_W, GamePad::DPAD_UP },
			{ GLFW_KEY_A, GamePad::DPAD_LEFT },
			{ GLFW_KEY_S, GamePad::DPAD_DOWN },
			{ GLFW_KEY_D, GamePad::DPAD_RIGHT },
			{ GLFW_KEY_ENTER, GamePad::START },
			{ GLFW_KEY_SPACE,GamePad::A },
			{ GLFW_KEY_R, GamePad::R },
			{ GLFW_KEY_ESCAPE,GamePad::ESCAPE },
			});

		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer: " << renderer << std::endl;
		const GLubyte* version = glGetString(GL_VERSION);
		std::cout << "Version: " << version << std::endl;

		windowSizeX = w;
		windowSizeY = h;

		return true;
	}

	bool Window::ShouldClose() const {
		return glfwWindowShouldClose(window) != 0;
	}

	void Window::SwapBuffers() const {
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	const GamePad& Window::GetGamePad() const {
		return gamepad;
	}

	void Window::UpdateDeltaTime(){

		//�o�ߎ��Ԃ̍X�V����
		nowTime = glfwGetTime();
		deltaTime = static_cast<float>(nowTime - prevTime);
		prevTime = nowTime;
		deltaTime = glm::min(0.25f, deltaTime);
	}

	/**
	*	�W���C�X�e�B�b�N�̃A�i���O���͑��uID
	*
	*	@note XBOX360�R���g���[���[�
	*/
	enum GLFWAXESID {
		GLFWAXESID_LeftX,	///< ���X�e�B�b�N��X��
		GLFWAXESID_LeftY,	///< ���X�e�B�b�N��Y��
		GLFWAXESID_BackX,	///< �A�i���O�g���K�[
		GLFWAXESID_RightX,	///< �E�X�e�B�b�N��X��
		GLFWAXESID_RightY,	///< �E�X�e�B�b�N��Y��
	};

	/**
	*	�W���C�X�e�B�b�N�̃f�W�^�����͑��u
	*
	*	@note XBOX360�R���g���[���[�
	*/
	enum {
		GLFWBUTTONID_A,		///< A�{�^��
		GLFWBUTTONID_B,		///< B�{�^��
		GLFWBUTTONID_X,		///< X�{�^��
		GLFWBUTTONID_Y,		///< Y�{�^��
		GLFWBUTTONID_L,		///< L�{�^��
		GLFWBUTTONID_R,		///< R�{�^��
		GLFWBUTTONID_Back,	///< Back�{�^��
		GLFWBUTTONID_Start,	///< Start�{�^��
		GLFWBUTTONID_LThumb,///< ���X�e�B�b�N�{�^��
		GLFWBUTTONID_RThumb,///< �E�X�e�B�b�N�{�^��
		GLFWBUTTONID_Up,	///< ��L�[
		GLFWBUTTONID_Right,	///< �E�L�[
		GLFWBUTTONID_Down,	///< ���L�[
		GLFWBUTTONID_Left,	///< ���L�[
	};

	void Window::UpdateGamePad() {

		const uint32_t prevButtons = gamepad.buttons;
		const uint32_t prevMouseButtons = gamepad.mouseButtons;
		int axesCount, buttonCount;

		//�����͂ƃ{�^�����͂̎擾
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
		const uint8_t* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

		if (axes && buttons && axesCount >= 2 && buttonCount >= 8) {
			///�����͂̏���

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
			///�L�[���͂̍X�V����
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
				
				//�}�b�v���̃L�[(glfw�̃L�[�R�[�h)�̓��͏�Ԃ��擾
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

		///�}�E�X�̃{�^�����͏���
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

		///�}�E�X�̍��W�X�V����
		double mousex, mousey;
		glfwGetCursorPos(window, &mousex, &mousey);
		glm::vec2 newMousePos = glm::vec2(static_cast<float>(mousex), static_cast<float>(mousey));

		gamepad.mouseVelocity = newMousePos - gamepad.mousePosition;
		gamepad.mousePosition = newMousePos;

	}

	void Window::UpdateMouseWheel(float x, float y){

		static glm::vec2 prev = glm::vec2(0);

		gamepad.mouseWheelY = static_cast<int>(y - prev.y);

		prev = glm::vec2(x, y);

	}

	void Window::SetKeyMap(std::vector<stKeyMap>& k){

		keyMap.clear();

		for (stKeyMap& keyValue : k) {
			
			keyMap[keyValue.glfwCode] = keyValue.keyCode;
		}	
	}

	bool Window::GetWindowSize(int* w, int* h){

		glfwGetWindowSize(window,w, h);

		if (*w != windowSizeX || *h != windowSizeY) {
			windowSizeX = *w;
			windowSizeY = *h;
			return true;
		}

		return false;
	}

} //namespace GLFWEW
