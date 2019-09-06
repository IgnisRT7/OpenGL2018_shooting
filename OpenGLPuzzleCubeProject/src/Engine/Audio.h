/**
*	@file	Audio.h
*	@brief	音声再生用
*	@author tn-mai(講義資料製作者)
*/

#pragma once

namespace Audio {

	static const size_t playerMax = 8;

	/**
	*	オーディオシステムを初期化する
	*
	*	@param acfPath		ACFファイルのパス
	*	@param acbPath		ACBファイルのパス
	*	@param awbPath		AWBファイルのパス
	*	@param dspBusName	D-Bus名	
	*
	*	@retval true	初期化成功
	*	@retval false	初期化失敗
	*/
	bool Initialize(const char* acfPath, const char* acbPath, const char* acwPath, const char* dspBusName);

	/**
	*	オーディオシステムの状態を更新する
	*/
	void Update();

	/**
	*	オーディオシステムを破棄する
	*/
	void Destroy();

	/**
	*	音声を再生する
	*
	*	@param playerId 再生に使用するプレイヤーのID
	*	@param cueId	再生するキューのID
	*/
	void Play(int playerId,int cueId);

	/**
	*	音声を停止する
	*
	*	@param playerId 再生を停止するプレイヤーのID
	*/
	void Stop(int playerId);

	/**
	*	音声を全停止する
	*/
	void StopAll();
}