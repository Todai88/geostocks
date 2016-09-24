#pragma once

#ifndef  __DATABASE_CON_H_INCLUDED__
#define __DATABASE_CON_H_INCLUDED__

#include <windows.h> 
#include <iostream>
#include <windows.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <sstream>
#include <vector> 
#include <thread>
#include <functional>
using namespace std;
class database_con {
public:
	void show_error(unsigned int handletype, const SQLHANDLE& handle);
	database_con(std::vector<std::vector<std::wstring>>, string symbol);
	~database_con();
private:
	/*
	Private member variables
	*/
	SQLHENV env;
	SQLHDBC dbc;
	SQLHSTMT stmt;
	SQLRETURN ret;
	string _mSymbol;
	vector<vector<wstring>> _mHistorical;
	vector<vector<wstring>>::iterator _mVecHistIter;
	SQLSMALLINT columns;


	/*
	Private member functions
	*/
	void database_con::executeQuery(wstring query);
	std::wstring database_con::buildQuery(vector<std::wstring> input, string symbol);
	std::wstring database_con::StringToWString(const std::string& s);
	void database_con::assignThread(std::vector<vector<std::wstring>> historical, std::string symbol);
	void database_con::runThreads();
};



#endif