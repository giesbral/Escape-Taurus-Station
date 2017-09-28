#include "fileops.h"
#include "dirent.h"
#include "cstdlib"
#include <ctime>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(_WIN32)
#include <direct.h> //windows _mkdir
#endif

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ifstream;
using std::fstream;
using std::ofstream;
using std::to_string;
using std::ios;

int saveGame(std::string sourceFolder) {

    srand(time(NULL));
    int saveFileNum;
    saveFileNum = rand();
    string saveFile;
    string openFile;
    char num[] = "0123456789";

    DIR *dir = NULL;
    struct dirent *dirinfo = NULL;

    //opens parent directory
	std::string dirToOpen = "../" + sourceFolder;
    dir = opendir(dirToOpen.c_str());

    #if defined(_WIN32)
    _mkdir(("../" + string(to_string(saveFileNum))).c_str());
    #else
    mkdir(("../" + string(to_string(saveFileNum))).c_str(), 0777);
    #endif

    if(dir == NULL) {
        cout << "Error Reading Parent Directory" << endl;
        exit(1);
    }

    ifstream f1;
    ofstream f2;

    while(dirinfo = readdir(dir)) {
        if(dirinfo == NULL) {
            cout << "Error Reading Parent Directory" << endl;
            exit(1);
        }

        if(!strcmp(dirinfo->d_name, ".") || !strcmp(dirinfo->d_name, "..") ||
           !strcmp(dirinfo->d_name, ((to_string(saveFileNum)).c_str())) ||
           !strcmp(dirinfo->d_name, "PARENT_FOLDER")  ||
           !strcmp(dirinfo->d_name, "Engine")  ||
           !strcmp(dirinfo->d_name, "Example Room")  ||
           !strcmp(dirinfo->d_name, "Parser")  ||
		   !strcmp(dirinfo->d_name, "Debug")  ||		//a VisualStudio folder
           strpbrk(dirinfo->d_name, num) != NULL ) {

        //The following can only be used on linux systems
//        if ( !strcmp(dirinfo->d_name, ".") || !strcmp(dirinfo->d_name, "..") ||
//           !strcmp(dirinfo->d_type, "DT_DIR") ) {

        }

        //Design for this section was influenced from the discussion here:
        // http://stackoverflow.com/questions/3680730/c-fileio-copy-vs-systemcp-file1-x-file2-x
        else {
            //cout << dirinfo->d_name << endl;
            openFile = dirinfo->d_name;
            openFile = "../" + sourceFolder + "/" + openFile;
            string randNum;
            saveFile = to_string(saveFileNum);
            randNum = saveFile;
            saveFile.append(dirinfo->d_name);
            f1.open((openFile.c_str()), ios::in | ios::binary);
            f2.open(("../" + randNum + "/" + dirinfo->d_name), ios::out | ios::app | ios::binary);

            if (f1.is_open()) {

            }
            else {
                cout << "Input File is not open" << endl;
                exit(1);
            }
            if (f2.is_open()) {

            }
            else {
                cout << "Output File is not open" << endl;
                exit(1);
            }
            //The following line appends the random number to beginning of files
            //and is not necessary if we place the copied files in a separate folder
            //ofstream f2(("../" + randNum + "/" + saveFile), fstream::trunc|fstream::binary);
            f2 << f1.rdbuf();


        }

        f1.close();
        f2.close();


    }



    closedir(dir);

    return saveFileNum;
}

std::string loadGame() {
	std::string gameFile;
	
	cout << "Please enter the game file number you would like to load: ";
	std::getline(std::cin, gameFile);
	
	return gameFile;
}
