#ifndef FILEPARSE_H
#define FILEPARSE_H

#include <string>

std::string samsModule(std::string roomFile, std::string header);

void pickUpObject(std::string roomFile, std::string object, std::string header);

void dropObject(std::string roomFile, std::string object, std::string header);

#endif
