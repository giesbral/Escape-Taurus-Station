#include "fileparse.h"
#include "helperFunctions.h"

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using std::string;
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
using std::getline;

string samsModule(string roomFile, string header) {

    ifstream inputFile;
    string line;
    string check;
    string textblock;

    inputFile.open(roomFile);

    if (!inputFile) {
        cout << "Unable to open " << roomFile << endl;
		exit(1);
    }

    while(getline(inputFile, line)) {
        // if(header == "LONGFORM") {
        //     if(line == "LONGFORM") {
        //         while(line != "SHORTFORM") {
        //             getline(inputFile, line);
        //             if(line == "SHORTFORM") {
        //                 break;
        //             }
        //             textblock.append(line);
        //             textblock.append("\n");
        //         }
        //         textblock.pop_back();
        //         line = textblock;
        //         break;
        //     }
        // }
        // else if(header == "NAMES") {
        //     if(line == "NAMES") {
        //         while(line != "DESCRIPTION") {
        //             getline(inputFile, line);
        //             if(line == "DESCRIPTION") {
        //                 break;
        //             }
        //             textblock.append(line);
        //             textblock.append("\n");
        //         }
        //         textblock.pop_back();
        //         line = textblock;
        //         break;
        //     }
        // }
        // else if(header == "DESCRIPTION") {
        //     if(line == "DESCRIPTION") {
        //         while(line != "END") {
        //             getline(inputFile, line);
        //             if(line == "END") {
        //                 break;
        //             }
        //             textblock.append(line);
        //             textblock.append("\n");
        //         }
        //         textblock.pop_back();
        //         line = textblock;
        //         break;
        //     }
        // }
   //      else if(header == "SHORTFORM") {
   //          if(line == "SHORTFORM") {
   //              while(line != "OBJECTS") {
   //                  getline(inputFile, line);
   //                  if(line == "OBJECTS") {
   //                      break;
   //                  }
   //                  textblock.append(line);
   //                  textblock.append("\n");
   //              }
   //              textblock.pop_back();
   //              line = textblock;
   //              break;
   //          }
   //      }
   //      else if(header == "OBJECTS") {
			// if (line == "OBJECTS") {
			// 	while (line != "FORWARD" || line != "END") {
			// 		getline(inputFile, line);
			// 		if (line == "FORWARD" || line == "END") {
			// 			break;
			// 		}
			// 		textblock.append(line);
			// 		textblock.append("\n");
			// 	}
			// 	textblock.pop_back();
			// 	line = textblock;
			// 	break;
			// }
   //      }
        if(header == line) {
            while (line != "END") {
                getline(inputFile, line);
                if (line == "END") {
                    break;
                }
                textblock.append(line);
                textblock.append("\n");
            }
        if(!textblock.empty())
			textblock.pop_back();
        line = textblock;
        break;
        }
    }

    inputFile.close();

    return line;
}

//The design of this function was inspired by the discussion here: http://stackoverflow.com/questions/26576714/deleting-specific-line-from-file
void pickUpObject(string roomFile, string object, string header) {
    ifstream inputFile;
    ofstream tempFile;
    string line;
    string tempName;

    tempName = roomFile;
    tempName.append("Temp");

    inputFile.open(roomFile);
    if (inputFile.is_open()) {

    }
    else {
        cout << "Unable to open " << roomFile << endl;
        exit(1);
    }

    tempFile.open(tempName);
    if (tempFile.is_open()) {

    }
    else {
        cout << "Unable to open " << tempName << endl;
        exit(1);
    }

    int flag = 0;
	bool inTargetHeader = false;
    while(getline(inputFile, line)) {
		if (line == header)
			inTargetHeader = true;
		if (inTargetHeader == true && line == "END")
			inTargetHeader = false;
		if(stringToLowercase(line) == stringToLowercase(object) && inTargetHeader == true) {
            flag = 1;
            continue;
        }
        tempFile << line << "\n";
    }

    if(flag == 0) {
        //cout << "There is no " << object << " in the room." << endl;
    }

    inputFile.close();
    tempFile.close();

    const char *oldRoom = roomFile.c_str();
    const char *newRoom = tempName.c_str();
    remove(oldRoom);
    rename(newRoom, oldRoom);
}

void dropObject(string roomFile, string object, string header) {
    ifstream inputFile;
    ofstream tempFile;
    string line;
    string tempName;

    tempName = roomFile;
    tempName.append("Temp");

    inputFile.open(roomFile);
    if (inputFile.is_open()) {

    }
    else {
        cout << "Unable to open " << roomFile << endl;
        exit(1);
    }

    tempFile.open(tempName);
    if (tempFile.is_open()) {

    }
    else {
        cout << "Unable to open " << tempName << endl;
        exit(1);
    }

    while(getline(inputFile, line)) {
            if(line == header) {
                tempFile << line << "\n";
                tempFile << object << "\n";
                continue;
            }
            tempFile << line << "\n";
    }


    inputFile.close();
    tempFile.close();

    const char *oldRoom = roomFile.c_str();
    const char *newRoom = tempName.c_str();
    remove(oldRoom);
    rename(newRoom, oldRoom);

}














































































