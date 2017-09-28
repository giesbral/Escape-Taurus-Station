#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <string>
#include <vector>

//text centering used for main menu
void centerstring(const char*);

std::vector<std::string> stringSplitToVector(std::string);

bool doesFileExist(const char*);

std::string stringToLowercase(std::string s);

#endif