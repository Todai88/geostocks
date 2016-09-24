#pragma once

//======================
//include guard
#ifndef  __HISTORICAL_H_INCLUDED__
#define __HISTORICAL_H_INCLUDED__

//======================
// forward declared dependencies

//======================
// included dependencies::
#include <iostream>
#include <fstream>
#include <stdio.h> 
#include <curl/curl.h> 
#include <string>
#include "database_con.h"
//======================
// historical::

using namespace std;
class historical {
public:
	void historical::go(string symbol);
	historical();
	std::vector<std::vector<std::wstring>> historical::getHistorical();
	~historical();
private:
	void writer(string* input);
	std::vector<std::vector<std::wstring>> bigVector;
	std::vector<wstring> historical::parseData(wstring line, char delim);
	std::wstring historical::StringToWString(const std::string& s);
	void historical::toVector();
	string _mSymbol;
	string* pBuffer;
	string _mHistorical;
	string _mQueryString;
	void historical::writeData(std::vector<std::vector<std::wstring>> in, const string& symbol);
};


#endif