#include <Windows.h>
#include "ShowMenu.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

	ShowMenu* obj = new ShowMenu(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	obj->MainShowMenu();

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	delete obj;

	return 0;
}