#pragma once

//======================
//include guard
#ifndef  __READTICKERS_H_INCLUDED__
#define __READTICKERS_H_INCLUDED__

//======================
// forward declared dependencies

//======================
// included dependencies::
#include <iostream>
#include <fstream>
#include <stdio.h> 
#include <curl/curl.h> 
#include <string>
#include <vector>
#include <sstream>
//======================
// readtickers::

using namespace std;
class readtickers {
public:
	vector<string>* getSymbols();
	readtickers();
	~readtickers();
private:
	void filterTickers(string *s, const char delim);
	void getFile();
	void readFile();
	string _mTickers;
	vector<string> _mSymbols;
	vector<string>::iterator _mSymbolsIter;
};

#endif