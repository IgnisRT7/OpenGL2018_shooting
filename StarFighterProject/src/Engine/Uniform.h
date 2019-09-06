/**
*	@file	Uniform.h
*	@brief	シェーダに使用するユニファームバッファブロックの管理
*	@author	tn-mai(講義資料製作者)
*/

#pragma once
#include <glm/glm.hpp>

namespace Uniform {

	static const int maxViewCount = 4;

	/// 頂点データのパラメータ型	
	struct alignas(256) VertexData {
		glm::mat4 matMVP[maxViewCount];
		glm::mat4 matDepthMVP;
		glm::mat4 matModel;
		glm::mat4 matNormal;
		glm::vec4 color;
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
	struct alignas(256) LightData {
		glm::vec4 eyePos[maxViewCount];
		glm::vec4 ambientColor;	///< 環境光
		PointLight light[maxLightCount];	///< ライトのリスト
	};

	/**
	*	ポストエフェクトデータ
	*/
	struct alignas(256) PostEffectData {

		glm::mat4x4 matColor;	///< 色変換行列
		float luminanceScale;	///< 輝度増減係数
		float bloomThreshold;	///< ブルームを発生させる閾値

	};
}