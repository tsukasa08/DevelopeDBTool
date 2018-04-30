#include <fstream>
#include "HelpWindow.h"

#define WINDOWHEIGHT 600
#define WINDOWWIDTH 900
#define BTNCLICK 100

//staticメンバ変数定義
HelpWindow::MainArgument HelpWindow::stSMArgument;

HelpWindow::HelpWindow()
{
}


HelpWindow::~HelpWindow()
{
}

//引き数ありのコンストラクタ
HelpWindow::HelpWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow){

	stSMArgument.hInstance = hInstance;
	stSMArgument.hPrevInstance = hPrevInstance;
	stSMArgument.lpCmdLine = lpCmdLine;
	stSMArgument.nCmdShow = nCmdShow;
}

//ヘルプウィンドウの登録関数
BOOL HelpWindow::RegisterHelpWindowInfo(){

	LPCSTR ClassName = "HelpWindow";
	WNDCLASS StartMenuInfo;


	if (stSMArgument.hInstance){

		StartMenuInfo.style = CS_HREDRAW | CS_VREDRAW;
		StartMenuInfo.lpfnWndProc = HelpWindow::WndProc;
		StartMenuInfo.cbClsExtra = 0;
		StartMenuInfo.cbWndExtra = 0;
		StartMenuInfo.hInstance = stSMArgument.hInstance;
		StartMenuInfo.hIcon = NULL;
		StartMenuInfo.hCursor = LoadCursor(NULL, IDC_ARROW);
		StartMenuInfo.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
		StartMenuInfo.lpszMenuName = NULL;
		StartMenuInfo.lpszClassName = ClassName;

		if (!RegisterClass(&StartMenuInfo)){
			return FALSE;
		}

		HelpWindowhWnd = ::CreateWindow(ClassName,
			"ヘルプウィンドウ",
			WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,
			300,
			300,
			WINDOWWIDTH,
			WINDOWHEIGHT,
			NULL,
			NULL,
			stSMArgument.hInstance,
			NULL);

		if (HelpWindowhWnd == NULL){
			return FALSE;
		}

		return TRUE;

	}
	else{

		return FALSE;
	}

}

//ヘルプウィンドウのメイン関数
BOOL HelpWindow::HelpWindowMain(){

	if (RegisterHelpWindowInfo() == FALSE){
		return FALSE;
	}

	::ShowWindow(HelpWindowhWnd, stSMArgument.nCmdShow);

	return TRUE;
}

BOOL HelpWindow::Paint(HWND hWnd, std::vector<std::string> vecHelpContents){

	static const int Interval = 30;
	PAINTSTRUCT ps;
	RECT rc;
	::GetClientRect(hWnd, &rc);

	HDC hdc = ::BeginPaint(hWnd, &ps);

	rc.top += Interval;

	for (auto itr = vecHelpContents.begin(); itr != vecHelpContents.end(); itr++){
		::DrawText(hdc, (*itr).c_str(), -1, &rc, DT_WORDBREAK | DT_LEFT);
		rc.top += Interval;
	}
	

	::EndPaint(hWnd, &ps);

	return TRUE;
}

LRESULT HelpWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	//ボタンのウィンドウハンドル
	static HWND BtnHwnd;
	//ヘルプウィンドウに描画する内容を保持したテキストファイルのパス
	static std::string HelpTxtFilePass;
	//ヘルプウィンドウに描画する内容を保持する
	static std::vector<std::string> vecHelpContents;

	switch (msg) {

	case WM_CREATE:

		BtnHwnd = ::CreateWindow(
			TEXT("BUTTON"),
			TEXT("終了"),
			WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			800,
			490,
			50,
			50,
			hWnd,
			reinterpret_cast<HMENU>(BTNCLICK),
			stSMArgument.hInstance,
			NULL);

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case BTNCLICK:
			::PostQuitMessage(0);
			break;
		}
		break;

	case WM_PAINT:

		HelpTxtFilePass = "C:\\Users\\Tsukasa\\Desktop\\DB_source\\DevelopeDBTool\\data\\HelpContents.txt";

		if (ReatHelpContentsFile(HelpTxtFilePass, &vecHelpContents) == FALSE){
			::PostQuitMessage(0);
			break;
		}

		//クライアント領域を取得し、テキストに描画する
		if (Paint(hWnd, vecHelpContents) == FALSE){
			::MessageBox(NULL, TEXT("ウィンドウ描画に失敗しました。"), TEXT("システムエラー"), MB_OK);
			::PostQuitMessage(0);
		}
		break;

	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	default:
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0L;
}

//ヘルプ内容が記述されたテキストファイルの読み込み関数
BOOL HelpWindow::ReatHelpContentsFile(std::string HelpTxtFilePass, std::vector<std::string>* vecHelpContents){

	std::ifstream ifs(HelpTxtFilePass);

	if (!ifs){
		::MessageBox(NULL, TEXT("ヘルプウィンドウ用のテキストファイルの読み込みに失敗しました。"), TEXT("システムエラー"), MB_OK);
		return FALSE;
	}

	std::string tmp;

	while (std::getline(ifs, tmp)){
		vecHelpContents->push_back(tmp);
	}

	return TRUE;
}