#pragma once

#include <Windows.h>
#include <commctrl.h>

#include "XMLParser.h"

class ShowMenu
{
public:

	struct MainArgument {
		HINSTANCE hInstance;
		HINSTANCE hPrevInstance;
		PSTR lpCmdLine;
		int nCmdShow;
	};


	//�����o�֐�
	ShowMenu();
	~ShowMenu();

	//���C���֐��̈����̃R�s�[
	ShowMenu(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);

	//ShowMenu�̃��C���֐�
	BOOL MainShowMenu();

private:

	//��ʓo�^
	BOOL RegisterScreenInfo();

	//�c���[�R���g���[���쐬
	static HWND CreateTreeControl(HWND hWnd);

	//�c���[�R���g���[���C���[�W�o�^
	static HIMAGELIST RegisterTreeImages(HWND TreeHwnd);

	//�c���[�R���g���[�����̐ݒ�
	static TVINSERTSTRUCT SetTreeControlInfo(HWND TreeHwnd, std::vector<CComBSTR>* vecPass, std::vector<CComBSTR>* vecName);

	//�c���[���j���[�̑I�����ʂɉ���������
	static void DoEachModule(HWND TreeHwnd, std::map<CComBSTR, CComBSTR> XMLFileContents);

	//�E�B���h�E�̒��̃c���[���j���[�\���̈�ȊO�ŉE�N���b�N���ꂽ�ꍇ�̃��j���[�o�^
	static HMENU RegisetrRihgtClickMenu();


	//�E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



private:
	//�����o�ϐ�

	static MainArgument stSMArgument;

	HWND hWnd;

	//XMLParser�N���X�̃I�u�W�F�N�g�|�C���^
	static XMLParser* XmlObj;
};

