#pragma once
#include <vector>
#include <utility>

#ifdef USE_DEBUGLOGGER

/*
*	デバッグ用文字出力クラス
*/
class OutputLogDebugger {
public:

	OutputLogDebugger& Instance();

	void AddTag(std::string tagName);

	void OutputLog(std::string str, uint32_t tag);

private:
	
	OutputLogDebugger();
	~OutputLogDebugger();
	OutputLogDebugger(const OutputLogDebugger&) = delete;
	OutputLogDebugger& operator=(const OutputLogDebugger&) = delete;

	std::vector<std::pair<std::string, uint32_t> > tagList;

};

#endif