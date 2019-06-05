#pragma once
/**
*	@file StageLoader.h
*/


class StageLoader {
public:



	bool Init(const char* filePass);
	

private:

	StageLoader() = default;
	~StageLoader();
	StageLoader(const StageLoader&) = delete;
	void operator=(const StageLoader&) = delete;

private:


};