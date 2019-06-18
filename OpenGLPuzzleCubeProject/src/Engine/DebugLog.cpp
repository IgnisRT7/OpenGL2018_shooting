#include "DebugLog.h"

#ifdef USE_DEBUG_LOGGER

/**
*	インスタンスの取得
*/
OutputLogDebugger& OutputLogDebugger::Instance() {

	static OutputLogDebugger instance;
	return instance;
}

/**
*	タグの追加
*
*	@param タグ名
*/
void OutputLogDebugger::AddTag(std::string tagName) {

	auto result = std::find(tagList.begin(), tagList.end(), tagName);
	if (result == tagList.end()) {
		//該当するタグが存在しないので新規追加

		//tagList.push_back(std::make_pair<std::string,int>(tagName,1 >> static_cast<int>(tagList.size())  ))
	}

}

#endif