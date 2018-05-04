#include "XMLParser.h"
#include <string>


XMLParser::XMLParser()
{
}


XMLParser::~XMLParser()
{
	::CoUninitialize();
}

//XMLファイル読み込み
bool XMLParser::ReadXml(std::vector<CComBSTR>* vecName, std::vector<CComBSTR>* vecPass) {

	HRESULT hResult;
	hResult = ::CoInitialize(NULL);

	if (hResult != S_OK) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルの読み込みに失敗しました。"), TEXT("システムエラー"), MB_OK);
		return false;
	}

	std::wstring XMLPassName = L"C:\\Users\\Tsukasa\\Desktop\\DB_source\\DevelopeDBTool\\data\\TreeMenu.xml";
	IXMLDOMDocumentPtr pXMLDOMDocument;

	hResult = pXMLDOMDocument.CreateInstance(CLSID_DOMDocument);

	if (hResult != S_OK) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルの読み込みに失敗しました。"), TEXT("システムエラー"), MB_OK);
		::CoUninitialize();
		return false;
	}

	pXMLDOMDocument->put_async(VARIANT_FALSE); // load関数がロードを完了するまで待つようにする。

	VARIANT_BOOL vbResult;
	hResult = pXMLDOMDocument->load(CComVariant(XMLPassName.c_str()), &vbResult);


	if (hResult != S_OK) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルの読み込みに失敗しました。"), TEXT("システムエラー"), MB_OK);
		::CoUninitialize();
		return false;
	}

	if (!vbResult) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルの読み込みに失敗しました。"), TEXT("システムエラー"), MB_OK);
		::CoUninitialize();
		return false;
	}

	//xsdスキーマと擦り合わせの処理を行う

	//タグ名を固定して取得する以外の方法検討
	IXMLDOMNodeListPtr RootEle;
	hResult = pXMLDOMDocument->getElementsByTagName(L"INDIVIDUAL", &RootEle);

	if (hResult != S_OK) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルのタグ名のノードリストの取得に失敗しました。"), TEXT("システムエラー"), MB_OK);
		::CoUninitialize();
		return false;
	}

	if (RootEle == NULL) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルのタグ名のノードリストの取得に失敗しました。"), TEXT("システムエラー"), MB_OK);
		::CoUninitialize();
		return false;
	}

	long lCountNode = 0;
	hResult = RootEle->get_length(&lCountNode);

	if (hResult != S_OK) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルのタグ名のノードの数の取得に失敗しました。"), TEXT("システムエラー"), MB_OK);
		::CoUninitialize();
		return false;
	}

	if (lCountNode == 0) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルのタグ名のノードの数の取得に失敗しました。"), TEXT("システムエラー"), MB_OK);
		::CoUninitialize();
		return false;
	}

	IXMLDOMNodePtr pXMLDOMNode = NULL;
	DOMNodeType eNodeType;
	IXMLDOMElementPtr pXMLDOMElement = NULL;
	CComVariant varValue;

	for (int i = 0; i < lCountNode; i++) {

		hResult = RootEle->get_item(i, &pXMLDOMNode);
		if (hResult != S_OK) {
			::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルのノードリストの解析に取得に失敗しました。"), TEXT("システムエラー"), MB_OK);
			return false;
		}

		//ノードタイプの取得
		hResult = pXMLDOMNode->get_nodeType(&eNodeType);
		if (hResult != S_OK) {
			::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルのノードタイプの取得に失敗しました。"), TEXT("システムエラー"), MB_OK);
			return false;
		}

		if (eNodeType != NODE_ELEMENT) {
			::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルのノードタイプの取得に失敗しました。"), TEXT("システムエラー"), MB_OK);
			return false;
		}

		//エレメント型への変換
		hResult = pXMLDOMNode->QueryInterface(IID_IXMLDOMElement, (void**)&pXMLDOMElement);

		if (hResult != S_OK) {
			::MessageBoxA(NULL, TEXT("TreeMenu.xmlファイルのエレメント型への変換に失敗しました。"), TEXT("システムエラー"), MB_OK);
			return false;
		}

		//name の取得
		hResult = pXMLDOMElement->getAttribute(L"name", &varValue);
		if (SUCCEEDED(hResult) && varValue.vt == VT_BSTR){
			vecName->push_back(varValue.bstrVal);
		}

		//passの取得
		hResult = pXMLDOMElement->getAttribute(L"pass", &varValue);
		if (SUCCEEDED(hResult) && varValue.vt == VT_BSTR) {
			vecPass->push_back(varValue.bstrVal);
		}

	}
	return true;
}