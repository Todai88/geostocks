#pragma once

//======================
//include guard
#ifndef  __HOURQUOTES_H_INCLUDED__
#define __HOURQUOTES_H_INCLUDED__

//======================
// forward declared dependencies

//======================
// included dependencies::
#include <iostream>
#include <stdio.h> 
#include <curl/curl.h> 
#include <string>
#include <vector>
//======================
// hourquotes:

class hourquotes {
public:
	hourquotes(std::vector<std::string> symbols);
	std::string* getQuote();
	~hourquotes();
private:
	std::string _mQuote;
	std::string _mQuote2;
};

#endif