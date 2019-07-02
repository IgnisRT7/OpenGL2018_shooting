/**
*	@file GLFWEW.h
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
		static Window& Instance();
		bool Init(int w, int h, const char* title);
		bool ShouldClose() const;
		void SwapBuffers() const;
		const GamePad& GetGamePad() const;
		void UpdateGamePad();
		void UpdateMouseWheel(float x, float y);
		void ClearWheel() { gamepad.mouseWheelY = 0; }
		void SetKeyMap(std::vector<stKeyMap>& k);

		GLFWwindow& GetWindow()const { return *window; }
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

		std::map<int, uint32_t> keyMap;


	};



}	// namespace GLFWEW




