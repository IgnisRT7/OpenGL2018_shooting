/**
*	@file GLFWEW.h
*	@brief	�Q�[���O�V�X�e�������̐���p
*	@brief	Takuya Yokoyama , tn-mai
*/

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GamePad.h"
#include <map>
#include <vector>

#define GL_DEBUG_STRING(str) std::cerr << str;

namespace GLFWEW {

	struct stKeyMap {
		int glfwCode;		/// GLEW��ł̃R�[�h��`
		uint32_t keyCode;	/// ���ۂɎg�p�����r�b�g�l
	};

	/**
	*	GLFW��GLEW�̃��b�p�[�N���X
	*/
	class Window {
	public:

		/**
		*	�V���O���g���C���X�^���X���擾����
		*
		*	@return Window�̃V���O���g���C���X�^���X
		*/
		static Window& Instance();

		/**
		*	GLFW/GLEW�̏�����
		*
		*	@param w �E�C���h�E�̕`��͈͂̕�(�s�N�Z��)
		*	@param h �E�C���h�E�̕`��͈͂̍���(�s�N�Z��)
		*	@param title �E�C���h�E�^�C�g��(UTF-8��0�I�[������)
		*
		*	@retval true ����������
		*	@retval false ���������s
		*/
		bool Init(int w, int h, const char* title);

		/**
		*	�E�C���h�E�����ׂ������ׂ�
		*
		*	retval trueu ����
		*	retval false ���Ȃ�
		*/
		bool ShouldClose() const;

		/**
		*	�t�����g�o�b�t�@�ƃo�b�N�o�b�t�@��؂�ւ���
		*/
		void SwapBuffers() const;

		/**
		*	�Q�[���p�b�h�̏�Ԃ��擾����
		*
		*	@return �Q�[���p�b�h�̏��
		*/
		const GamePad& GetGamePad() const;

		/**
		*	�E�C���h�E�C���X�^���X�̎擾
		*
		*	@return �E�C���h�E�C���X�^���X
		*/
		GLFWwindow& GetWindow()const { return *window; }

		/**
		*	�o�ߎ��Ԃ̎擾
		*
		*	@return �o�ߎ���
		*/
		const float DeltaTime() const { return deltaTime; }

		/**
		*	�O�񂩂�̌o�ߎ��Ԃ̍X�V����
		*/
		void UpdateDeltaTime();

		/**
		*	�Q�[���p�b�h�̏�Ԃ��X�V����
		*/
		void UpdateGamePad();

		/**
		*	�}�E�X�z�C�[���̒l�̍X�V����
		*
		*	@param x	�z�C�[��X�̉�]��
		*	@param y	�z�C�[��Y�̉�]��
		*/
		void UpdateMouseWheel(float x, float y);

		/**
		*	�z�C�[�����̃N���A����
		*/
		void ClearWheel() { gamepad.mouseWheelY = 0; }

		/**
		*	�L�[�̃}�b�s���O�̐ݒ�
		*
		*	@param k �L�[�}�b�s���O�̃f�[�^
		*/
		void SetKeyMap(std::vector<stKeyMap>& k);

		/**
		*	�E�C���h�E�T�C�Y���擾���܂�
		*
		*	@param w	�E�C���h�E�̉��T�C�Y�̊i�[�ꏊ
		*	@param h	�E�C���h�E�̏c�T�C�Y�̊i�[�ꏊ
		*
		*	return true	�E�C���h�E�T�C�Y���ύX���ꂽ
		*	retrun false�ύX����Ă��Ȃ�
		*/
		bool GetWindowSize(int* w, int* h);

	private:

		Window();
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		bool isGLFWInitialized;
		bool isInitialized;
		GLFWwindow* window;
		GamePad gamepad;

		int windowSizeX;
		int windowSizeY;

		double prevTime;
		double nowTime;
		float deltaTime=0;

		std::map<int, uint32_t> keyMap;


	};



}	// namespace GLFWEW




