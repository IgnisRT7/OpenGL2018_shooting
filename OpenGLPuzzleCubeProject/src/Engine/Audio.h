/**
*	@file	Audio.h
*	@brief	�����Đ��p
*	@author tn-mai(�u�`���������)
*/

#pragma once

namespace Audio {

	static const size_t playerMax = 8;

	/**
	*	�I�[�f�B�I�V�X�e��������������
	*
	*	@param acfPath		ACF�t�@�C���̃p�X
	*	@param acbPath		ACB�t�@�C���̃p�X
	*	@param awbPath		AWB�t�@�C���̃p�X
	*	@param dspBusName	D-Bus��	
	*
	*	@retval true	����������
	*	@retval false	���������s
	*/
	bool Initialize(const char* acfPath, const char* acbPath, const char* acwPath, const char* dspBusName);

	/**
	*	�I�[�f�B�I�V�X�e���̏�Ԃ��X�V����
	*/
	void Update();

	/**
	*	�I�[�f�B�I�V�X�e����j������
	*/
	void Destroy();

	/**
	*	�������Đ�����
	*
	*	@param playerId �Đ��Ɏg�p����v���C���[��ID
	*	@param cueId	�Đ�����L���[��ID
	*/
	void Play(int playerId,int cueId);

	/**
	*	�������~����
	*
	*	@param playerId �Đ����~����v���C���[��ID
	*/
	void Stop(int playerId);

	/**
	*	������S��~����
	*/
	void StopAll();
}