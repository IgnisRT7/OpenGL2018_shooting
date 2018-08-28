/**
*	@file GamePad.h
*/
#pragma once
#include <stdint.h>

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
	};
	uint32_t buttons;	///< 押されている間フラグが立つ
	uint32_t buttonDown;///< 押された瞬間だけフラグが立つ
};
