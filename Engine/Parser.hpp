#pragma once

#include "inputStruct.hpp"
#include "dictionary.hpp"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>


class parser {

public:
	static inputStruct parseInput(string);
	static void printParsed(inputStruct); // debug function
	static void printInputs(vector<string>); // debug function

private:
	static vector<string> splitString(string);
	static void processVerbs(string&, inputStruct&);
	static void processNouns(string&, inputStruct&);
	static string findWord(string, unordered_set<string>);
	static string commandToLower(string);
	static int countWords(const char*);
};