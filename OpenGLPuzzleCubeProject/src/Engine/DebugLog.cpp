#include "DebugLog.h"

#ifdef USE_DEBUG_LOGGER

/**
*	�C���X�^���X�̎擾
*/
OutputLogDebugger& OutputLogDebugger::Instance() {

	static OutputLogDebugger instance;
	return instance;
}

/**
*	�^�O�̒ǉ�
*
*	@param �^�O��
*/
void OutputLogDebugger::AddTag(std::string tagName) {

	auto result = std::find(tagList.begin(), tagList.end(), tagName);
	if (result == tagList.end()) {
		//�Y������^�O�����݂��Ȃ��̂ŐV�K�ǉ�

		//tagList.push_back(std::make_pair<std::string,int>(tagName,1 >> static_cast<int>(tagList.size())  ))
	}

}

#endif