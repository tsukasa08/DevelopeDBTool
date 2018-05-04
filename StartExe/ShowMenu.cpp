#include "ShowMenu.h"

#include <shellapi.h >
#include <string>
#include <map>

#define WINDOWHEIGHT 500
#define WINDOWWIDTH 500
#define IDC_TREE 100
#define RCLICK_EXIT 105
#define RCLICK_HELP 106

//static�����o�ϐ��̒�`
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
			"���j���[�I�����",
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

	//�c���[���j���[�̃n���h��
	static HWND TreeHwnd;
	//�X�e�[�^�X�o�[�̃n���h��
	static HWND hStatus;
	//�E�N���b�N���̃��j���[�n���h��
	static HMENU RMenuHwnd;
	//�c���[���j���[�̃C���[�W���X�g
	static HIMAGELIST hImg;
	//�c���[���j���[�ڍ�
	static TVINSERTSTRUCT tvis;
	//XML�t�@�C������擾�����c���[���j���[�ʂ̖��O���i�[����
	static std::vector<CComBSTR> vecName;
	//XML�t�@�C������擾�����c���[���j���[�ʂ̎��s�t�@�C���̃p�X���i�[����
	static std::vector<CComBSTR> vecPass;


	switch (msg) {

	case WM_DESTROY:
		ImageList_Destroy(hImg);
		::DestroyMenu(RMenuHwnd);
		PostQuitMessage(0);
		break;

	case WM_CREATE:

		//�c���[���j���[�̍쐬
		::InitCommonControls();

		//�E�N���b�N�������ɕ\������R���e�L�X�g���j���[�o�^
		RMenuHwnd = RegisetrRihgtClickMenu();
		if (RMenuHwnd == NULL){
			MessageBox(NULL, TEXT("�E�N���b�N�|�b�v�A�b�v���j���[�쐬�Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
			::PostQuitMessage(0);
		}

		TreeHwnd = CreateTreeControl(hWnd);
		if (TreeHwnd == NULL){
			MessageBox(NULL, TEXT("�c���[�R���g���[���̍쐬�Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
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
			TEXT("�E�N���b�N�Ńw���v��\���ł��܂��B"),
			hWnd,
			3);


		break;

	case WM_RBUTTONDOWN:{
		POINT pt;

		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		::ClientToScreen(hWnd, &pt);
		//�E�N���b�N�����ʒu�Ƀ|�b�v�A�b�v���j���[�\��
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

		//�ǂݍ���XML�t�@�C���̎��s�p�X��std::map�ɐݒ�
		std::map<CComBSTR, CComBSTR> XMLFileContents;
		int i = 0;
		for (auto itr = vecName.begin(); itr != vecName.end(); itr++) {
			XMLFileContents[(*itr)] = vecPass[i];
			i++;
		}

		//�I����Ԃ�Enter�L�[�����̏ꍇ
		if (pLVKey->wVKey == VK_RETURN){
			DoEachModule(TreeHwnd, XMLFileContents);
		}

		//�I����ԂŃ_�u���N���b�N���ꂽ�ꍇ
		if (pnmhdr->idFrom == IDC_TREE){

			switch (pnmhdr->code){
			case NM_DBLCLK:
				//�I�����ʂɂ���Ă��ꂼ��̃��W���[���N��
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

//�c���[�R���g���[���쐬
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

//�c���[�R���g���[���̃C���[�W�o�^
HIMAGELIST ShowMenu::RegisterTreeImages(HWND TreeHwnd){

	HIMAGELIST hImg;

	hImg = ImageList_Create(16, 16, ILC_COLOR4 | ILC_MASK, 2, 1);
	ImageList_AddIcon(hImg, LoadIcon(NULL, IDI_INFORMATION));
	ImageList_AddIcon(hImg, LoadIcon(NULL, IDI_WINLOGO));

	TreeView_SetImageList(TreeHwnd, hImg, TVSIL_NORMAL);

	return hImg;
}

//�c���[�R���g���[���̏���ݒ�
TVINSERTSTRUCT ShowMenu::SetTreeControlInfo(HWND TreeHwnd, std::vector<CComBSTR>* vecPass, std::vector<CComBSTR>* vecName){

	TVINSERTSTRUCT tvis = {NULL};
	HTREEITEM hitem;

	//XML�ǂݍ��݊֐�
	XmlObj = new XMLParser();
	if (XmlObj->ReadXml(vecName, vecPass) == false) {
		return tvis;
	}

	tvis.hParent = TVI_ROOT;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_TEXT;
	tvis.item.pszText = TEXT("����ꗗ");
	hitem = TreeView_InsertItem(TreeHwnd, &tvis);

	tvis.hParent = hitem;

	LPSTR lpWkBuf;
	_bstr_t bstBuf;
	for (auto itr = vecName->begin(); itr != vecName->end(); itr++) {
		//BSTR->LPCSTR�ϊ�
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

//�c���[�R���g���[���̑I�����ʂɂ���Ă��ꂼ��̃��W���[���N��
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

	//std::string��BSTR�ɕϊ�(BSTR�̓��C�h����)
	int strlen = ::MultiByteToWideChar(
		CP_ACP,
		NULL,
		tmp.c_str(),
		tmp.length(),
		NULL,
		NULL);

	//BSTR�̃o�b�t�@������
	BSTR CComtmp = ::SysAllocStringLen(NULL, strlen);

	::MultiByteToWideChar(
		CP_ACP,
		NULL,
		tmp.c_str(),
		tmp.length(),
		CComtmp,
		strlen);

	//�I�����ꂽ�p�X���̎擾
	BSTR tmpPassName = XMLFileContents[CComtmp];
	_bstr_t bstmpPassName = tmpPassName;
	std::string PassName(static_cast<char*>(bstmpPassName));

	//�擾�����p�X�����g�p���ă��W���[���N��
	::ShellExecute(NULL, "open", PassName.c_str(), NULL, NULL, SW_SHOWNORMAL);

}

//�E�N���b�N���̃��j���[�o�^
HMENU ShowMenu::RegisetrRihgtClickMenu(){

	HMENU tmpHWND = ::CreatePopupMenu();

	if (tmpHWND == NULL){
		return tmpHWND;
	}

	::AppendMenu(tmpHWND, MF_STRING, RCLICK_HELP, "Help");
	::AppendMenu(tmpHWND, MF_STRING, RCLICK_EXIT, "Exit");

	return tmpHWND;
}