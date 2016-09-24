#include "stdafx.h"
#include "readtickers.h"


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}


readtickers::readtickers() {
	cout << "Ticker instance started" << endl;
	getFile();
	filterTickers(&_mTickers, '|');
}


void readtickers::getFile() {
	cout << "Fetching ticker data" << endl;
	
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	/*
	Sets up the HTTP request to nasdaq to get the tickers and puts the HTTP response (CSV) in &_mTickers.
	*/
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "ftp://ftp.nasdaqtrader.com/SymbolDirectory/nasdaqlisted.txt");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &_mTickers);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
}
void readtickers::filterTickers(string *s, const char delim) {

	cout << "Filtering the csv" << endl;
		istringstream stream(*s);
		string line, temp = "";
		while (getline(stream, line)) {
			temp += line.substr(0, line.find(delim)) + "\n";
		} 
		size_t nFpos = temp.find("File");
		size_t secondNL = temp.find('\n', nFpos);
		size_t firstNL = temp.rfind('\n', nFpos);

		temp.erase(firstNL, secondNL - firstNL);
		stream.clear();
		istringstream istream(temp);
		int count = 0;
		
		cout << "Populating the vector." << endl;

		while (getline(istream, line)) {
			if (count > 0)
			{
				_mSymbols.push_back(line);
			}
			else {
				count++;
			}
		}
}

vector<string>* readtickers::getSymbols() {
	return &_mSymbols;
}
readtickers::~readtickers() {
	cout << "Ticker destroyed";
}
