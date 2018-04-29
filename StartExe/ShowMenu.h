#pragma once

#include <Windows.h>
#include <commctrl.h>

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
	static TVINSERTSTRUCT SetTreeControlInfo(HWND TreeHwnd);

	//�c���[���j���[�̑I�����ʂɉ���������
	static void DoEachModule(HWND TreeHwnd);

	//�E�B���h�E�̒��̃c���[���j���[�\���̈�ȊO�ŉE�N���b�N���ꂽ�ꍇ�̃��j���[�o�^
	static HMENU RegisetrRihgtClickMenu();


	//�E�B���h�E�v���V�[�W��
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



private:
	//�����o�ϐ�

	static MainArgument stSMArgument;

	HWND hWnd;
};

