/**
*	@file	Effect.cpp
*	@brief	3Dモデルを使用した疑似エフェクト
*	@author	TakuyaYokoyama
*/
#include "Effect.h"

namespace GameState {

	/**
	*	初期化処理
	*/
	void Blast::Initialize() {

		entity->CastShadow(false);
	}

	/**
	*	更新処理
	*/
	void Blast::Update(float delta) {

		timer += delta;

		if (timer >= 0.3) {
			entity->Destroy();
			return;
		}
		const float variation = timer * 4.0f;	//変化量
		entity->Scale(glm::vec3(1.0f + variation));	//徐々に拡大していく

																	//時間経過で色と透明度を変化させる
		static const glm::vec4 color[] = {
			glm::vec4(1.0f,1.0f,0.75f,1),
			glm::vec4(1.0f,0.5f,0.1f,1),
			glm::vec4(0.25f,0.1f,0.1f,0),
		};
		const glm::vec4 col0 = color[static_cast<int>(variation)];
		const glm::vec4 col1 = color[static_cast<int>(variation) + 1];
		const glm::vec4 newColor = glm::mix(col0, col1, std::fmod(variation, 1));
		entity->Color(newColor);

		//Y軸回転させる
		glm::vec3 euler = glm::eulerAngles(entity->Rotation());
		euler.y += glm::radians(60.0f) + delta;
		entity->Rotation(glm::quat(euler));
	}

}