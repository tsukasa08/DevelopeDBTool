#pragma once

#include <Windows.h>
#include <string>
#include <vector>

class HelpWindow
{
public:

	//WinMain�֐��̈����̍\����
	struct MainArgument {
		HINSTANCE hInstance;
		HINSTANCE hPrevInstance;
		PSTR lpCmdLine;
		int nCmdShow;
	};


	HelpWindow();
	~HelpWindow();

	//����������̃R���X�g���N�^
	HelpWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);

	//�w���v�E�B���h�E�̃��C���֐�
	BOOL HelpWindowMain();

private:

	//��ʓo�^
	BOOL RegisterHelpWindowInfo();

	//�e�L�X�g�`��֐�
	static BOOL Paint(HWND hWnd, std::vector<std::string> vecHelpContents);

	//�w���v�E�B���h�E�ɕ`�悷����e���L�q���ꂽ�e�L�X�g�t�@�C���ǂݎ��֐�
	static BOOL ReatHelpContentsFile(std::string HelpTxtFilePass, std::vector<std::string>* vecHelpContents);

	//�E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	//�����o�ϐ�

	static MainArgument stSMArgument;

	HWND HelpWindowhWnd;
};

