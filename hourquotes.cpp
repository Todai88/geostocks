#include "stdafx.h"
#include "hourquotes.h"


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}


hourquotes::hourquotes(std::vector<std::string> symbols) {

	std::cout << "Adding symbols to query: " << std::endl;
	std::string symbol;
	for (unsigned int it = 1; it <= 3080; it++){
		symbol += symbols.at(it) + "+";
		if (it == 1500){
	std::string query = "http://download.finance.yahoo.com/d/quotes.csv?s=" + symbol + "&f=snoabpc";
	CURL *curl; // initialize cURL
	CURLcode res; // setup a response
	const char *pQuery = query.c_str();
	curl = curl_easy_init();
				if (curl) {
					std::cout << "Working on setting up first query" << std::endl;
					curl_easy_setopt(curl, CURLOPT_URL, pQuery);
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
					std::cout << "Performing the query!" << std::endl;
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, &_mQuote);
					res = curl_easy_perform(curl);
					curl_easy_cleanup(curl);
					std::cout << "I'm done loading quotes" << std::endl;
					symbol.clear();
						}
					}
		if (it == 3080) {
			std::string query = "http://download.finance.yahoo.com/d/quotes.csv?s=" + symbol + "&f=snoabpc";

			CURL *curl; // initialize cURL
			CURLcode res; // setup a response
			const char *pQuery = query.c_str();
			curl = curl_easy_init();
			if (curl) {
				std::cout << "Working on setting up the second query" << std::endl;
				curl_easy_setopt(curl, CURLOPT_URL, pQuery);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
				std::cout << "Performing the query!" << std::endl;
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &_mQuote2);
				res = curl_easy_perform(curl);
				curl_easy_cleanup(curl);
				std::cout << "I'm done loading quotes" << std::endl;
			}
		}
	}
	_mQuote += _mQuote2;
}

std::string* hourquotes::getQuote() {
	return &_mQuote;
}
hourquotes::~hourquotes() {

}