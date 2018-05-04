#include "XMLParser.h"
#include <string>


XMLParser::XMLParser()
{
}


XMLParser::~XMLParser()
{
	::CoUninitialize();
}

//XML�t�@�C���ǂݍ���
bool XMLParser::ReadXml(std::vector<CComBSTR>* vecName, std::vector<CComBSTR>* vecPass) {

	HRESULT hResult;
	hResult = ::CoInitialize(NULL);

	if (hResult != S_OK) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
		return false;
	}

	std::wstring XMLPassName = L"C:\\Users\\Tsukasa\\Desktop\\DB_source\\DevelopeDBTool\\data\\TreeMenu.xml";
	IXMLDOMDocumentPtr pXMLDOMDocument;

	hResult = pXMLDOMDocument.CreateInstance(CLSID_DOMDocument);

	if (hResult != S_OK) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
		::CoUninitialize();
		return false;
	}

	pXMLDOMDocument->put_async(VARIANT_FALSE); // load�֐������[�h����������܂ő҂悤�ɂ���B

	VARIANT_BOOL vbResult;
	hResult = pXMLDOMDocument->load(CComVariant(XMLPassName.c_str()), &vbResult);


	if (hResult != S_OK) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
		::CoUninitialize();
		return false;
	}

	if (!vbResult) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
		::CoUninitialize();
		return false;
	}

	//xsd�X�L�[�}�ƎC�荇�킹�̏������s��

	//�^�O�����Œ肵�Ď擾����ȊO�̕��@����
	IXMLDOMNodeListPtr RootEle;
	hResult = pXMLDOMDocument->getElementsByTagName(L"INDIVIDUAL", &RootEle);

	if (hResult != S_OK) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̃^�O���̃m�[�h���X�g�̎擾�Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
		::CoUninitialize();
		return false;
	}

	if (RootEle == NULL) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̃^�O���̃m�[�h���X�g�̎擾�Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
		::CoUninitialize();
		return false;
	}

	long lCountNode = 0;
	hResult = RootEle->get_length(&lCountNode);

	if (hResult != S_OK) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̃^�O���̃m�[�h�̐��̎擾�Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
		::CoUninitialize();
		return false;
	}

	if (lCountNode == 0) {
		::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̃^�O���̃m�[�h�̐��̎擾�Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
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
			::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̃m�[�h���X�g�̉�͂Ɏ擾�Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
			return false;
		}

		//�m�[�h�^�C�v�̎擾
		hResult = pXMLDOMNode->get_nodeType(&eNodeType);
		if (hResult != S_OK) {
			::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̃m�[�h�^�C�v�̎擾�Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
			return false;
		}

		if (eNodeType != NODE_ELEMENT) {
			::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̃m�[�h�^�C�v�̎擾�Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
			return false;
		}

		//�G�������g�^�ւ̕ϊ�
		hResult = pXMLDOMNode->QueryInterface(IID_IXMLDOMElement, (void**)&pXMLDOMElement);

		if (hResult != S_OK) {
			::MessageBoxA(NULL, TEXT("TreeMenu.xml�t�@�C���̃G�������g�^�ւ̕ϊ��Ɏ��s���܂����B"), TEXT("�V�X�e���G���["), MB_OK);
			return false;
		}

		//name �̎擾
		hResult = pXMLDOMElement->getAttribute(L"name", &varValue);
		if (SUCCEEDED(hResult) && varValue.vt == VT_BSTR){
			vecName->push_back(varValue.bstrVal);
		}

		//pass�̎擾
		hResult = pXMLDOMElement->getAttribute(L"pass", &varValue);
		if (SUCCEEDED(hResult) && varValue.vt == VT_BSTR) {
			vecPass->push_back(varValue.bstrVal);
		}

	}
	return true;
}