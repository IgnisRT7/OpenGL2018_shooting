/**
*	@file Uniform.h
*/
#pragma once
#include <glm/glm.hpp>

namespace Uniform {

	/// 頂点データのパラメータ型	
	struct VertexData {
		glm::mat4 matMVP;
		glm::mat4 matModel;
		glm::mat4 matNormal;
		glm::vec4 color;
		glm::mat4x3 _dummy;
	};

	/**
	*	ライトデータ(点光源)
	*/
	struct PointLight {
		glm::vec4 position;	///< 座標(ワールド座標系)
		glm::vec4 color;	///< 明るさ
	};

	const int maxLightCount = 4;

	/**
	*	ライティングパラメータ
	*/
	struct LightData {
		glm::vec4 ambientColor;	///< 環境光
		PointLight light[maxLightCount];	///< ライトのリスト
	};

	/**
	*	ポストエフェクトデータ
	*/
	struct PostEffectData {

		glm::mat4x4 matColor;	///< 色変換行列
		float luminanceScale;	///< 輝度増減係数
		float bloomThreshold;	///< ブルームを発生させる閾値

	};
}