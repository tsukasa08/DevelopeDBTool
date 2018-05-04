#include "ShowMenu.h"

#include <shellapi.h >
#include <string>
#include <map>

#define WINDOWHEIGHT 500
#define WINDOWWIDTH 500
#define IDC_TREE 100
#define RCLICK_EXIT 105
#define RCLICK_HELP 106

//staticメンバ変数の定義
ShowMenu::MainArgument ShowMenu::stSMArgument;
XMLParser* ShowMenu::XmlObj;

ShowMenu::ShowMenu()
{
}


ShowMenu::~ShowMenu()
{
	delete XmlObj;
}

ShowMenu::ShowMenu(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	stSMArgument.hInstance = hInstance;
	stSMArgument.hPrevInstance = hPrevInstance;
	stSMArgument.lpCmdLine = lpCmdLine;
	stSMArgument.nCmdShow = nCmdShow;

}

BOOL ShowMenu::MainShowMenu() {


	RegisterScreenInfo();
	::ShowWindow(hWnd, stSMArgument.nCmdShow);

	return TRUE;
}


BOOL ShowMenu::RegisterScreenInfo() {

	LPCSTR ClassName = "ShowMenu";
	WNDCLASS StartMenuInfo;


	if (stSMArgument.hInstance){

		StartMenuInfo.style = CS_HREDRAW | CS_VREDRAW;
		StartMenuInfo.lpfnWndProc = ShowMenu::WndProc;
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

		hWnd = ::CreateWindow(ClassName,
			"メニュー選択画面",
			WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE,
			10,
			10,
			WINDOWHEIGHT,
			WINDOWWIDTH,
			NULL,
			NULL,
			stSMArgument.hInstance,
			NULL);

		return TRUE;

	}
	else{

		return FALSE;
	}
}

LRESULT ShowMenu::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	//ツリーメニューのハンドル
	static HWND TreeHwnd;
	//ステータスバーのハンドル
	static HWND hStatus;
	//右クリック時のメニューハンドル
	static HMENU RMenuHwnd;
	//ツリーメニューのイメージリスト
	static HIMAGELIST hImg;
	//ツリーメニュー詳細
	static TVINSERTSTRUCT tvis;
	//XMLファイルから取得したツリーメニュー別の名前を格納する
	static std::vector<CComBSTR> vecName;
	//XMLファイルから取得したツリーメニュー別の実行ファイルのパスを格納する
	static std::vector<CComBSTR> vecPass;


	switch (msg) {

	case WM_DESTROY:
		ImageList_Destroy(hImg);
		::DestroyMenu(RMenuHwnd);
		PostQuitMessage(0);
		break;

	case WM_CREATE:

		//ツリーメニューの作成
		::InitCommonControls();

		//右クリック押下時に表示するコンテキストメニュー登録
		RMenuHwnd = RegisetrRihgtClickMenu();
		if (RMenuHwnd == NULL){
			MessageBox(NULL, TEXT("右クリックポップアップメニュー作成に失敗しました。"), TEXT("システムエラー"), MB_OK);
			::PostQuitMessage(0);
		}

		TreeHwnd = CreateTreeControl(hWnd);
		if (TreeHwnd == NULL){
			MessageBox(NULL, TEXT("ツリーコントロールの作成に失敗しました。"), TEXT("システムエラー"), MB_OK);
			::PostQuitMessage(0);
		}

		hImg = RegisterTreeImages(TreeHwnd);

		tvis = SetTreeControlInfo(TreeHwnd, &vecPass, &vecName);

		if (tvis.hParent == NULL && tvis.hInsertAfter == NULL &&
			tvis.item.mask == NULL && tvis.item.hItem == NULL && tvis.itemex.mask == NULL && tvis.itemex.hItem == NULL) {
			::PostQuitMessage(0);
			break;
		}

		hStatus = ::CreateStatusWindow(
			WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP,
			TEXT("右クリックでヘルプを表示できます。"),
			hWnd,
			3);


		break;

	case WM_RBUTTONDOWN:{
		POINT pt;

		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		::ClientToScreen(hWnd, &pt);
		//右クリックした位置にポップアップメニュー表示
		::TrackPopupMenu(RMenuHwnd, 0, pt.x, pt.y, 0, hWnd, NULL);

	}


	case WM_SIZE:
		::SendMessage(hStatus, msg, wParam, lParam);

		::MoveWindow(TreeHwnd, 0, 0, 470, 350, FALSE);

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case RCLICK_EXIT:
			::PostQuitMessage(0);
			break;

		case RCLICK_HELP:
			ShellExecute(NULL, "open", "C:\\Users\\Tsukasa\\Desktop\\DB_source\\DevelopeDBTool\\Release\\HelpView.exe", NULL, NULL, SW_SHOWNORMAL);
			break;
		}
		break;

	case WM_NOTIFY:{

		NMHDR* pnmhdr = reinterpret_cast<LPNMHDR>(lParam);
		LV_KEYDOWN* pLVKey = reinterpret_cast<LV_KEYDOWN*>(pnmhdr);

		//読み込んだXMLファイルの実行パスをstd::mapに設定
		std::map<CComBSTR, CComBSTR> XMLFileContents;
		int i = 0;
		for (auto itr = vecName.begin(); itr != vecName.end(); itr++) {
			XMLFileContents[(*itr)] = vecPass[i];
			i++;
		}

		//選択状態でEnterキー押下の場合
		if (pLVKey->wVKey == VK_RETURN){
			DoEachModule(TreeHwnd, XMLFileContents);
		}

		//選択状態でダブルクリックされた場合
		if (pnmhdr->idFrom == IDC_TREE){

			switch (pnmhdr->code){
			case NM_DBLCLK:
				//選択結果によってそれぞれのモジュール起動
				DoEachModule(TreeHwnd, XMLFileContents);
			}
		}
	}

				   break;

	default:
		return(::DefWindowProc(hWnd, msg, wParam, lParam));
	}

	return (0L);
}

//ツリーコントロール作成
HWND ShowMenu::CreateTreeControl(HWND hWnd){

	HWND TreeHwnd;

	TreeHwnd = ::CreateWindowEx(
		WS_EX_CLIENTEDGE,
		WC_TREEVIEW,
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
		0,
		0,
		10,
		10,
		hWnd,
		reinterpret_cast<HMENU>(IDC_TREE),
		stSMArgument.hInstance,
		NULL);

	if (TreeHwnd == NULL){
		return TreeHwnd;
	}

	return TreeHwnd;

}

//ツリーコントロールのイメージ登録
HIMAGELIST ShowMenu::RegisterTreeImages(HWND TreeHwnd){

	HIMAGELIST hImg;

	hImg = ImageList_Create(16, 16, ILC_COLOR4 | ILC_MASK, 2, 1);
	ImageList_AddIcon(hImg, LoadIcon(NULL, IDI_INFORMATION));
	ImageList_AddIcon(hImg, LoadIcon(NULL, IDI_WINLOGO));

	TreeView_SetImageList(TreeHwnd, hImg, TVSIL_NORMAL);

	return hImg;
}

//ツリーコントロールの情報を設定
TVINSERTSTRUCT ShowMenu::SetTreeControlInfo(HWND TreeHwnd, std::vector<CComBSTR>* vecPass, std::vector<CComBSTR>* vecName){

	TVINSERTSTRUCT tvis = {NULL};
	HTREEITEM hitem;

	//XML読み込み関数
	XmlObj = new XMLParser();
	if (XmlObj->ReadXml(vecName, vecPass) == false) {
		return tvis;
	}

	tvis.hParent = TVI_ROOT;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_TEXT;
	tvis.item.pszText = TEXT("操作一覧");
	hitem = TreeView_InsertItem(TreeHwnd, &tvis);

	tvis.hParent = hitem;

	LPSTR lpWkBuf;
	_bstr_t bstBuf;
	for (auto itr = vecName->begin(); itr != vecName->end(); itr++) {
		//BSTR->LPCSTR変換
		bstBuf = *itr;
		lpWkBuf = static_cast<char*>(bstBuf);
		tvis.item.pszText = lpWkBuf;

		tvis.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvis.item.iImage = 1;
		tvis.item.iSelectedImage = 1;
		TreeView_InsertItem(TreeHwnd, &tvis);
	}

	return tvis;

}

//ツリーコントロールの選択結果によってそれぞれのモジュール起動
void ShowMenu::DoEachModule(HWND TreeHwnd, std::map<CComBSTR, CComBSTR> XMLFileContents){

	HTREEITEM hItem;
	TVITEM ti;
	char szStr[100];

	memset(&ti, 0, sizeof(TVITEM));
	ti.mask = TVIF_HANDLE | TVIF_TEXT;
	ti.pszText = szStr;
	ti.cchTextMax = 100;

	hItem = TreeView_GetSelection(TreeHwnd);
	ti.hItem = hItem;
	TreeView_GetItem(TreeHwnd, &ti);

	std::string tmp(szStr);

	//std::stringをBSTRに変換(BSTRはワイド文字)
	int strlen = ::MultiByteToWideChar(
		CP_ACP,
		NULL,
		tmp.c_str(),
		tmp.length(),
		NULL,
		NULL);

	//BSTRのバッファを準備
	BSTR CComtmp = ::SysAllocStringLen(NULL, strlen);

	::MultiByteToWideChar(
		CP_ACP,
		NULL,
		tmp.c_str(),
		tmp.length(),
		CComtmp,
		strlen);

	//選択されたパス名の取得
	BSTR tmpPassName = XMLFileContents[CComtmp];
	_bstr_t bstmpPassName = tmpPassName;
	std::string PassName(static_cast<char*>(bstmpPassName));

	//取得したパス名を使用してモジュール起動
	::ShellExecute(NULL, "open", PassName.c_str(), NULL, NULL, SW_SHOWNORMAL);

}

//右クリック時のメニュー登録
HMENU ShowMenu::RegisetrRihgtClickMenu(){

	HMENU tmpHWND = ::CreatePopupMenu();

	if (tmpHWND == NULL){
		return tmpHWND;
	}

	::AppendMenu(tmpHWND, MF_STRING, RCLICK_HELP, "Help");
	::AppendMenu(tmpHWND, MF_STRING, RCLICK_EXIT, "Exit");

	return tmpHWND;
}