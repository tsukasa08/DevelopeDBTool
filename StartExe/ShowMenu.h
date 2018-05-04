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


	//メンバ関数
	ShowMenu();
	~ShowMenu();

	//メイン関数の引数のコピー
	ShowMenu(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow);

	//ShowMenuのメイン関数
	BOOL MainShowMenu();

private:

	//画面登録
	BOOL RegisterScreenInfo();

	//ツリーコントロール作成
	static HWND CreateTreeControl(HWND hWnd);

	//ツリーコントロールイメージ登録
	static HIMAGELIST RegisterTreeImages(HWND TreeHwnd);

	//ツリーコントロール情報の設定
	static TVINSERTSTRUCT SetTreeControlInfo(HWND TreeHwnd, std::vector<CComBSTR>* vecPass, std::vector<CComBSTR>* vecName);

	//ツリーメニューの選択結果に応じた処理
	static void DoEachModule(HWND TreeHwnd, std::map<CComBSTR, CComBSTR> XMLFileContents);

	//ウィンドウの中のツリーメニュー表示領域以外で右クリックされた場合のメニュー登録
	static HMENU RegisetrRihgtClickMenu();


	//ウィンドウプロシージャ
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);



private:
	//メンバ変数

	static MainArgument stSMArgument;

	HWND hWnd;

	//XMLParserクラスのオブジェクトポインタ
	static XMLParser* XmlObj;
};

