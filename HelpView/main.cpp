#include <Windows.h>
#include "HelpWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	HelpWindow* HelpWindowObj = new HelpWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	if (HelpWindowObj->HelpWindowMain() == FALSE){
		::MessageBox(NULL, TEXT("�w���v�E�B���h�E�̍쐬�Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
		return 0;
	}

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete HelpWindowObj;

	return 0;
}