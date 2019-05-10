#include "DebugLog.h"

OutputLogDebugger& OutputLogDebugger::Instance() {

	static OutputLogDebugger instance;
	return instance;
}

void OutputLogDebugger::AddTag(std::string tagName) {

	auto result = std::find(tagList.begin(), tagList.end(), tagName);
	if (result == tagList.end()) {
		//ŠY“–‚·‚éƒ^ƒO‚ª‘¶İ‚µ‚È‚¢‚Ì‚ÅV‹K’Ç‰Á

		//tagList.push_back(std::make_pair<std::string,int>(tagName,1 >> static_cast<int>(tagList.size())  ))
	}

}