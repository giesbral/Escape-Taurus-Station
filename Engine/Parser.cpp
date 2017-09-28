#include "Parser.hpp"

// sets of synonyms for verbs

inputStruct parser::parseInput(string userInput)
{
	inputStruct commands;
	string finput = commandToLower(userInput);

	if (userInput.empty() == false)
	{
		processVerbs(finput, commands);

		if (commands.verb != Exit)
		{
			if (finput.empty() == false || (commands.verb == Go))
			{
				processNouns(finput, commands);
			}
		}		
	}
	else
	{
		commands.verb = No_Input;
	}

	return commands;
}

void parser::printParsed(inputStruct commands)
{
	switch (commands.verb)
	{
	case Look :
		cout << "Verb Command: Look" << endl;
		break;
	case Look_At:
		cout << "Verb Command: Look_At" << endl;
		break;
	case Go :
		cout << "Verb Command: Go" << endl;
		break;
	case Take :
		cout << "Verb Command: Take" << endl;
		break;
	case Drop :
		cout << "Verb Command: Drop" << endl;
		break;
	case Use :
		cout << "Verb Command: Use" << endl;
		break;
	case Combine :
		cout << "Verb Command: Combine" << endl;
		break;
	case Consume :
		cout << "Verb Command: Consume" << endl;
		break;
	case Ignite :
		cout << "Verb Command: Ignite" << endl;
		break;
	case Read :
		cout << "Verb Command: Read" << endl;
		break;
	case Wear :
		cout << "Verb Command: Wear" << endl;
		break;
	case Search :
		cout << "Verb Command: Search" << endl;
		break;
	case Help :
		cout << "Verb Command: Help" << endl;
		break;
	case Inventory :
		cout << "Verb Command: Inventory" << endl;
		break;
	case Menu :
		cout << "Verb Command: Menu" << endl;
		break;
	case Exit :
		cout << "Verb Command: Exit" << endl;
		break;
	case Load :
		cout << "Verb Command: Load" << endl;
		break;
	case Save:
		cout << "Verb Command: Save" << endl;
		break;
	case Invalid:
		cout << "Verb Command: Invalid, User Entry: " << commands.verbInput << endl;
		break;
	case No_Input:
		cout << "Verb Command: None" << endl;
		break;
	}

	for (unsigned int i = 0; i < commands.nouns.size(); i++)
	{
		cout << "Noun " << i << ": " << commands.nouns[i] << endl;
	}
}

void parser::printInputs(vector<string> inputs)
{
	cout << "Entered: " << endl;

	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		cout << inputs[i] << endl;
	}
}

//http://stackoverflow.com/a/236803
std::vector<string> parser::splitString(string inputString)
{
	vector<string> splitString;

	stringstream sStream;
	sStream.str(inputString);

	const char delimiter = ' ';
	string token;

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

void parser::processVerbs(string &input, inputStruct &commands)
{
	// user commanded "verb" is invalid until proved otherwise
	commands.verb = Invalid;

	bool compoundVerb = false;
	string foundCommand;
	string firstTerm;
	string firstWord = splitString(input)[0];

	if (countWords(&input[0]) > 1)
	{
		string secondWord = splitString(input)[1];

		for (unsigned int i = 0; i < dictionary::prepositions.size(); ++i)
		{
			if (secondWord == dictionary::prepositions[i])
			{
				firstTerm = firstWord + " " + secondWord;
				compoundVerb = true;
				break;
			}
		}
	}

	if (compoundVerb == true)
	{
		// check for multi-words verbs
		for (unsigned int i = 0; i < dictionary::compound_dictionary.size(); ++i)
		{
			foundCommand = findWord(firstTerm, dictionary::compound_dictionary[i].synonyms);

			if (foundCommand.empty() == false)
			{
				commands.verb = dictionary::compound_dictionary[i].verb;
				commands.verbInput = foundCommand;
				input.erase(input.find(foundCommand), foundCommand.length());

				break;
			}
		}
	}
	else
	{
		firstTerm = firstWord;

		// check for multi-words verbs
		for (unsigned int i = 0; i < dictionary::compound_dictionary.size(); ++i)
		{
			foundCommand = findWord(firstTerm, dictionary::compound_dictionary[i].synonyms);

			if (foundCommand.empty() == false)
			{
				commands.verb = dictionary::compound_dictionary[i].verb;
				commands.verbInput = foundCommand;
				input.erase(input.find(foundCommand), foundCommand.length());

				break;
			}
		}

		// if not a multi-word verb, check for single word verbs
		if (commands.verb == Invalid)
		{
			for (unsigned int i = 0; i < dictionary::single_dictionary.size(); ++i)
			{
				foundCommand = findWord(firstTerm, dictionary::single_dictionary[i].synonyms);

				if (foundCommand.empty() == false)
				{
					commands.verb = dictionary::single_dictionary[i].verb;
					commands.verbInput = foundCommand;
					input.erase(input.find(foundCommand), foundCommand.length());

					break;
				}
			}
		}
	}

	// if user entered an invalid command...  
	if (commands.verb == Invalid)
	{
		commands.verbInput = firstTerm; 	
	}

	return;
}

void parser::processNouns(string &input, inputStruct &commands)
{
	string foundWord;

	// split input into individual words
	vector<string> sString = splitString(input);

	// process direction commands
	if (commands.verb == Go || commands.verb == Look_At)
	{
		foundWord = findWord(commands.verbInput, dictionary::directions);

		if (foundWord.empty() == false)
		{
			commands.nouns.push_back(commands.verbInput);
			return;
		}
		
		if (countWords(&input[0]) == 1)
		{
			foundWord = findWord(sString[0], dictionary::directions);

			if (foundWord.empty() == false)
			{
				commands.nouns.push_back(sString[0]);
				return;
			}
		}		
	}

	// if command.verb is invalid, do not process input as a normal command (will put input as a single element into the nouns list)
	if (commands.verb != Invalid)
	{		
		// remove articles
		for (vector<string>::iterator itr = sString.begin(); itr != sString.end(); ++itr)
		{
			for (unsigned int i = 0; i < dictionary::articles.size(); ++i)
			{
				// if article is found remove it from list of inputs
				if (*itr == dictionary::articles[i])
				{
					itr = sString.erase(itr);
					break;
				}
			}
		}	

		// check for prepositions in the input
		string tempStringNoun1;
		string tempStringNoun2;

		// loop through the input string
		for (unsigned int i = 0; i < sString.size(); ++i)
		{
			// loop through the prepositions list
			for (unsigned int j = 0; j < dictionary::prepositions.size(); ++j)
			{
				// if a preposition is found, get the nouns before and after it
				if (sString[i] == dictionary::prepositions[j])
				{
					// if the preposition is not the first or last word in the input
					if (i != 0 && i != (sString.size() - 1))
					{
						// get the noun before the preposition
						for (unsigned int k = 0; k < i; ++k)
						{
							if (k == 0)
							{
								tempStringNoun1 += (sString[k]);
							}
							else
							{
								tempStringNoun1 += (" " + sString[k]);
							}
						
						}

						// get the noun after the preposition
						for (unsigned int k = i + 1; k < sString.size(); ++k)
						{
							if (k == (i + 1))
							{
								tempStringNoun2 += (sString[k]);
							}
							else
							{
								tempStringNoun2 += (" " + sString[k]);
							}
						}

						// add both nouns to noun list and return
						commands.nouns.push_back(tempStringNoun1);
						commands.nouns.push_back(tempStringNoun2);
						return;
					}
				}
			}
		}
	}

	// if no prepositions, assume single noun following the verb
	string tempStringNoun;

	for (unsigned int i = 0; i < sString.size(); ++i)
	{
		if (i == 0)
		{
			tempStringNoun += (sString[i]);
		}
		else
		{
			tempStringNoun += (" " + sString[i]);
		}
	}

	// add single noun to noun list
	commands.nouns.push_back(tempStringNoun);	

	return;
}

string parser::findWord(string input, unordered_set<string> words)
{
	string wordString;

	unordered_set<string>::iterator itr = words.find(input);

	if (itr != words.end())
	{
		wordString = *itr;
	}

	return wordString;
}

string parser::commandToLower(string input)
{
	string tempString = input;

	transform(tempString.begin(), tempString.end(), tempString.begin(), ::tolower);

	return tempString;
}

int parser::countWords(const char * str)
{
	if (str == NULL)
	{
		return -1;
	}

	bool inSpaces = true;
	int numWords = 0;

	while (*str != '\0')
	{
		if (isspace(*str))
		{
			inSpaces = true;
		}
		else if (inSpaces)
		{
			numWords++;
			inSpaces = false;
		}

		++str;
	}

	return numWords;
}