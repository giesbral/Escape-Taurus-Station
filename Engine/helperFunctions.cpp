#include "helperFunctions.h"
#include <iostream>
#include <stdlib.h> 
#include <cstring>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <fstream>

//source: http://www.dreamincode.net/forums/topic/13749-center-text-with-cout/
void centerstring(const char* s)
{
	int l = strlen(s);
	int pos=(int)((80-l)/2);
	for(int i=0;i<pos;i++)
		std::cout<<" ";
	std::cout<<s;
}

//http://stackoverflow.com/a/236803
//and from Parser.cpp
std::vector<std::string> stringSplitToVector(std::string inputString)
{
	std::vector<std::string> splitString;

	std::stringstream sStream;
	sStream.str(inputString);

	const char delimiter = '\n';
	std::string token;

	// separate each word in user input
	while (getline(sStream, token, delimiter))
	{
		// add transformed input to string array
		if (token != "")
		{
			splitString.push_back(token);
		}		
	}

	return splitString;
}

//http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
bool doesFileExist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

//http://stackoverflow.com/questions/313970/how-to-convert-stdstring-to-lower-case
std::string stringToLowercase(std::string s)
{
	std::string tempString = s;

	transform(tempString.begin(), tempString.end(), tempString.begin(), ::tolower);

	return tempString;
}