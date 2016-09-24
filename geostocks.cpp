// geostocks.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdio.h> 
#include <curl/curl.h> 
#include <string>
#include "historical.h"
#include "readtickers.h"
#include "hourquotes.h"
#include <ctime>
#include <vector>
#include <thread>
#include <algorithm>
#include "database_con.h"
using namespace std;

void writer(string* pInput);
void replaceAll(std::string& str, const std::string& from, const std::string& to);

int main(void)
{
	int start_s = clock();

	/* 
	Create a readtickers instance that in itself gets a csv that contains all tickers we want.
	It then filters the tickers to only symbols and puts them in a string vector that is accessible by 
	getSymbols().
	*/


	/////////////////////////////////
	
	readtickers rt;
	vector<string> vSymbols;
	vector<string>::iterator it;
	vSymbols = *rt.getSymbols();
	historical hs;
	std::thread threads[10];
	int i = 0;
	std::shared_ptr<historical> p1(new historical);
	std::shared_ptr<historical> p2(new historical);
	std::shared_ptr<historical> p3(new historical);
	std::shared_ptr<historical> p4(new historical);
	std::thread t1(&historical::go, p1, "AAP");
	std::thread t3(&historical::go, p2, "AAPL");
	std::thread t4(&historical::go, p3, "IBM");
	std::thread t5(&historical::go, p4, "GOOG");
	t1.join();
	t3.join();
	t4.join();
	t5.join();
	//writer(his.getHistorical()); // this writes to the file test.csv
	int stop_s = clock();
	cout << endl << endl << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000 << endl;


	return 0;
}

void writer(string* pInput) {
	ofstream mf("test.csv");
	if (mf.is_open()) {
		mf << *pInput;
		mf.close();
	}
	else cout << "Unable to open file" << endl;
}

void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}