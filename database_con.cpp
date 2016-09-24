#include "stdafx.h"
#include "database_con.h"

////////////////////////////////////////////////////////////////////////
// Show errors from the SQLHANDLE

void database_con::show_error(unsigned int handletype, const SQLHANDLE& handle)
{
	SQLWCHAR sqlstate[1024];
	SQLWCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL))
		wcout << "Message: " << message << "\nSQLSTATE: " << sqlstate << endl;
}

std::wstring database_con::StringToWString(const std::string& s)
{
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}

////////////////////////////////////////////////////////////////////////
// Builds the stored procedure query.

std::wstring database_con::buildQuery(vector<std::wstring> input, string symbol)
{
	std::wstringstream builder;
	builder << L"EXEC sp_addHistorical " << "@Symbol='" << L"" << StringToWString(symbol) << "'," <<
		"@Date='" << (wstring)L"" << input.at(0) << "'," <<
		"@Open=" << (wstring)L"" << input.at(1) << "," <<
		"@Close=" << (wstring)L"" << input.at(2) << "," <<
		"@MaxPrice=" << (wstring)L"" << input.at(3) << "," <<
		"@MinPrice=" << (wstring)L"" << input.at(4) << "," <<
		"@Volume=" << (wstring)L"" << input.at(5) << ";";
	return builder.str();
}

void database_con::executeQuery(wstring query) {

	if (SQL_SUCCESS != SQLExecDirectW(stmt, const_cast<SQLWCHAR*>(query.c_str()), SQL_NTS)) {
		std::cout << "Execute error " << std::endl;
		show_error(SQL_HANDLE_STMT, stmt);
		std::wcout << L"Unsuccessful Query: " << query << std::endl;
	}
	// Close Cursor before next iteration starts:
	SQLRETURN closeCursRet = SQLFreeStmt(stmt, SQL_CLOSE);
	if (!SQL_SUCCEEDED(closeCursRet))
	{
		
		show_error(SQL_HANDLE_STMT, stmt);
		std::wcout << L"Unseccessful STMT: " << query << std::endl;
		// maybe add some handling for the case that closing failed.
	}
}
void database_con::assignThread(std::vector<vector<std::wstring>> historical, std::string symbol) {
	int nThreads = 5;
	std::thread threads[5];
	if (historical.size() < 5) {
		threads[historical.size()];
		nThreads = historical.size();
	}
	

	for (int i = 0; i < nThreads; i++) {
		threads[i] = std::thread(&database_con::executeQuery, this, buildQuery(historical.back (), symbol));
		historical.pop_back();
	}
	for (auto& th : threads) th.join();
}
////////////////////////////////////////////////////////////////////////
// Constructs a database connector object with the historical data and its symbol

database_con::database_con(std::vector<std::vector<std::wstring>> historical, string symbol) {
	_mHistorical = historical;
	_mSymbol = symbol;
	int nThreads = 5;
	std::thread threads[5];
	if (historical.size() < 5) {
		threads[historical.size()];
		nThreads = historical.size();
	}


	for (int i = 0; i < nThreads; i++) {
		threads[i] = std::thread(&database_con::runThreads, this);
	}
	for (auto& th : threads) th.join();
}
void database_con::runThreads(){
	/*
	Set up the handlers
	*/

	/* Allocate an environment handle */
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
	/* We want ODBC 3 support */
	SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
	/* Allocate a connection handle */
	SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);


	/* Connect to the DSN */
	SQLDriverConnectW(dbc, NULL, L"DRIVER={SQL Server};SERVER=DESKTOP-L5OT4OH\\SQLEXPRESS;DATABASE=stocks;UID=geo;PWD=geostocks;", SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
	/* Check for success */
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt))
	{
		show_error(SQL_HANDLE_DBC, dbc);
		std::cout << "Failed to connect";
	}
	executeQuery(buildQuery(_mHistorical.back(), _mSymbol));
	_mHistorical.pop_back();

	/*while (!historical.empty()) {
		assignThread(historical, symbol);
	} 
	for (_mVecHistIter = historical.begin();
	    _mVecHistIter != historical.end();
		_mVecHistIter++) {
		//assignThread(*_mVecHistIter, symbol);
		//std::thread t(&database_con::executeQuery, this, buildQuery(*_mVecHistIter, symbol));
		
		//std::thread t2(&database_con::executeQuery, this, buildQuery(historical.at(_mVecHistIter - historical.begin()+1), symbol));
		
		executeQuery(buildQuery(*_mVecHistIter, symbol));
		//t.join();		
		//t2.join();
		/*
		t3.join();
		t4.join();
		t5.join();
		*/

		//executeQuery(buildQuery(*_mVecHistIter, symbol));	
		

	/*_mSymbol = symbol;
	std::wstringstream stream(StringToWString(historical));
	std::wstring line;
	int row = 0;
	while (std::getline(stream, line)) {
		if (row > 0) {
			vector<wstring> vHistorical = parseData(L"" + line, ',');
			std::wstring SQL = buildQuery(vHistorical, _mSymbol);
			if (SQL_SUCCESS != SQLExecDirectW(stmt, const_cast<SQLWCHAR*>(SQL.c_str()), SQL_NTS)) {
				std::cout << "Execute error " << std::endl;
				show_error(SQL_HANDLE_STMT, stmt);
				std::wcout << L"Unsuccessful Query: " << SQL << std::endl;
			}
			// Close Cursor before next iteration starts:
			SQLRETURN closeCursRet = SQLFreeStmt(stmt, SQL_CLOSE);
			if (!SQL_SUCCEEDED(closeCursRet))
			{
				show_error(SQL_HANDLE_STMT, stmt);
				// maybe add some handling for the case that closing failed.
			}
		}
		row++;
	}*/
	
}

database_con::~database_con() {
	std::cout << "The database object has been deleted" << std::endl;
}
