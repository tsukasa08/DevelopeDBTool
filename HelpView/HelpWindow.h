#pragma once

#include <Windows.h>
#include <string>
#include <vector>

class HelpWindow
{
public:

	//WinMain関数の引数の構造体
	struct MainArgument {
		HINSTANCE hInstance;
		HINSTANCE hPrevInstance;
		PSTR lpCmdLine;
		int nCmdShow;
	};


	HelpWindow();
	~HelpWindow();

	//引き数ありのコンストラクタ
	HelpWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);

	//ヘルプウィンドウのメイン関数
	BOOL HelpWindowMain();

private:

	//画面登録
	BOOL RegisterHelpWindowInfo();

	//テキスト描画関数
	static BOOL Paint(HWND hWnd, std::vector<std::string> vecHelpContents);

	//ヘルプウィンドウに描画する内容が記述されたテキストファイル読み取り関数
	static BOOL ReatHelpContentsFile(std::string HelpTxtFilePass, std::vector<std::string>* vecHelpContents);

	//ウィンドウプロシージャ
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	//メンバ変数

	static MainArgument stSMArgument;

	HWND HelpWindowhWnd;
};

