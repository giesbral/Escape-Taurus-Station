#pragma once

// inputStruct.hpp 
//
// describes container for holding parsed user commands

#include <string>
#include <vector>

using namespace std;

enum Verbs {
	Look,
	Look_At,
	Go, 
	Take, 
	Drop, 
	Use, 
	Combine, 
	Consume, 
	Ignite, 
	Read, 
	Wear,
	Search,
	Help, 
	Inventory, 
	Menu, 
	Exit, 
	Load, 
	Save,
	Invalid,
	No_Input
};

struct inputStruct {
	Verbs verb;
	string verbInput;
	vector<string> nouns;
};