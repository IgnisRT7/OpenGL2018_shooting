/**
*	@file GamePad.h
*	@brief	ゲームパッド用パラメータ
*	@brief	Takuya Yokoyama , tn-mai
*/
#pragma once
#include <stdint.h>
#include <map>
#include <glm/glm.hpp>

/**
*	ゲームパッド情報
*/
struct GamePad {
	
	enum {
		DPAD_UP = 0x0001,
		DPAD_DOWN = 0x0002,
		DPAD_LEFT = 0x0004,
		DPAD_RIGHT = 0x0008,
		START = 0x0010,
		A = 0x0020,
		B = 0x0040,
		X = 0x0080,
		Y = 0x0100,
		L = 0x0200,
		R = 0x0400,
		ESCAPE = 0x1000,

		MOUSE_LEFT_BUTTON = 0x01,
		MOUSE_RIGHT_BUTTON = 0x02,
		MOUSE_MIDDLE_BUTTON = 0x04,
	};

	uint32_t buttons;	///< 押されている間フラグが立つ
	uint32_t buttonDown;///< 押された瞬間だけフラグが立つ

	uint32_t mouseButtons;
	uint32_t mouseButtonDown;

	glm::vec2 mousePosition;
	glm::vec2 mouseVelocity;
	int mouseWheelY;

};

class InputManager {
public:
	

	void Initialize();

	void Update();
	
	std::map<int, uint32_t> keyMap;

private:

	InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
};

