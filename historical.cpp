#include "stdafx.h"
#include "historical.h"



static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

////////////////////////////////////////////////////////////////////////
// Adds a substring of the string before the delimiter to a vector<wstring> that is returned.

std::vector<wstring> historical::parseData(wstring line, char delim) {
	size_t pos = 0;
	std::vector<std::wstring> vOut;
	while ((pos = line.find(delim)) != std::string::npos) {
		vOut.push_back(line.substr(0, pos));
		line.erase(0, pos + 1);
	}
	vOut.push_back(line.substr(0, pos));
	return vOut;
}
////////////////////////////////////////////////////////////////////////
// Converts a std::string to a std::wstring

std::wstring historical::StringToWString(const std::string& s)
{
	std::wstring temp(s.length(), L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp;
}

void historical::toVector() {
	std::cout << "Populating the vector " << _mSymbol << std::endl;
	std::wstringstream stream(StringToWString(_mHistorical));
	std::wstring line;
	int row = 0;
	while (std::getline(stream, line)) {
		if (row > 0) {
			bigVector.push_back(parseData(line, ','));
		}
		else row++;
	}
	std::cout << "Vector " << _mSymbol << " is populated" << std::endl;
}
historical::historical() {
	
}
void historical::go(string symbol){
	_mSymbol = symbol;
	cout << "Created a historical instance.." << endl;

	_mQueryString = "http://ichart.yahoo.com/table.csv?s=" + _mSymbol + "&a=00&b=01&c=1970&d=01&e=01&f=2016&d=m&ignore=.csv"; // setup the query by adding the string param. (2010-2016).	
	//http://ichart.yahoo.com/table.csv?s=aapl&a=00&b=01&c=2010&d=01&e=01&f=2016&d=m&ignore=.csv
	CURL *curl; // initialize cURL
	CURLcode res; // setup a response

	/*
	Setting up a constant char pointer that is a copy of the set-up string.
	*/

	const char *cstr = _mQueryString.c_str();
	curl = curl_easy_init();
	if (curl) {
		cout << "This is the URL " << endl << cstr << endl;
		curl_easy_setopt(curl, CURLOPT_URL, cstr);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &_mHistorical);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		toVector();
		vector<vector<wstring>>::iterator vIt;
		writeData(bigVector, symbol);
	}
}

std::vector<std::vector<std::wstring>> historical::getHistorical() {
	return bigVector;
}
historical::~historical() {
	cout << "The historical object is deleted" << std::endl;
} 
void historical::writeData(std::vector<std::vector<std::wstring>> in, const string& symbol) {
	std::cout << "Sending data to database connector" << std::endl;
	database_con db(in, symbol);
}

void historical::writer(string* pInput) {
	ofstream mf("test.csv");
	if (mf.is_open()) {
		mf << *pInput;
		mf.close();
	}
	else cout << "Unable to open file" << endl;
}