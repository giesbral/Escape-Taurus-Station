#pragma once

#include "inputStruct.hpp"
#include <unordered_set>

namespace dictionary {

	struct wordSet
	{
		const Verbs verb;
		const unordered_set<string> synonyms;
	};

	static const wordSet lookStruct { Look,{ "look" } };
	static const wordSet lookAtStruct { Look_At,{ "look at", "examine", "view", "inspect", "check" } };
	static const wordSet goStruct{ Go,{ "go", "fore", "forward", "f", "aft", "a", "port", "p", "starboard", "s", "walk", "walk to", "go to" } };
	static const wordSet takeStruct{ Take,{ "take", "get", "grab", "add", "pick up" } };
	static const wordSet dropStruct{ Drop,{ "drop", "leave", "abandon", "discard", "ditch", "dump", "jettison", "throw away", "throw out" } };
	static const wordSet useStruct{ Use,{ "use", "activate", "operate", "apply", "employ", "turn on" } };
	static const wordSet combineStruct{ Combine,{ "combine", "install", "connect", "couple", "fuse", "unite", "link", "join" } };
	static const wordSet consumeStruct{ Consume,{ "consume", "eat", "ingest", "bite" } };
	static const wordSet igniteStruct{ Ignite,{ "ignite", "light", "kindle", "burn" } };
	static const wordSet readStruct{ Read,{ "read", "peruse" } };
	static const wordSet wearStruct{ Wear,{ "wear", "put on", "don", "equip" } };
	static const wordSet searchStruct{ Search,{"search", "look in", "find", "look for", "open"} };
	static const wordSet helpStruct{ Help,{ "help" } };
	static const wordSet invStruct{ Inventory,{ "inventory", "inv" } };
	static const wordSet menuStruct{ Menu,{ "menu" } };
	static const wordSet exitStruct{ Exit,{ "exit" } };
	static const wordSet loadStruct{ Load,{ "load", "loadgame" } };
	static const wordSet saveStruct{ Save,{ "save", "savegame" } };

	static const vector<string> prepositions{ "with", "on", "in", "onto", "to", "into", "for", "of", "from", "by", "at", "up", "and" }; // yes, "and" isn't a preposition, but it is a time-saver to put it here
	static const vector<string> articles{ "the", "an", "a" };
	static const unordered_set<string> directions{ "fore", "forward", "f", "aft", "a", "port", "p", "starboard", "s" };

	static const vector<struct wordSet>  single_dictionary{
		lookStruct,
		helpStruct,
		invStruct,
		menuStruct,
		exitStruct,
		loadStruct,
		saveStruct
	};

	static const vector<struct wordSet>  compound_dictionary{
		lookAtStruct,
		goStruct,
		takeStruct,
		dropStruct,
		useStruct,
		combineStruct,
		consumeStruct,
		igniteStruct,
		readStruct,
		wearStruct,
		searchStruct
	};
};