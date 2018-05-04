#pragma once

#import <msxml6.dll> raw_interfaces_only

#include <atlbase.h>
#include <conio.h>
#include <map>
#include <vector>


class XMLParser
{
public:
	XMLParser();
	~XMLParser();

	//XMLファイル読み込み関数
	bool ReadXml(std::vector<CComBSTR>* vecName, std::vector<CComBSTR>* vecPass);
};