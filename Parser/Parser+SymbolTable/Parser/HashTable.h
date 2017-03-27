#pragma once
/* ************************************************************************
Divya Basappa, CS 4110, 14th Feb 2017
Compiler Version:Microsoft (R) C/C++ Optimizing Compiler Version 17
IDE: Visual Studio

class contains a HashTable which stores all the predefind KEYWORDS.

Methods:
ReadInput:- Reads keywords from the input file
Hash:- Gets the weight of the keyord, that is adds the ascii value of all characters in each word and mods with table size
Insert:-  Insert the keyword and the respective location in Hashtable based on Hssh key
Find:- Determines in the word is in the Hashtable or not.

************************************************************************* */

#ifndef HASHTABLE_HEADER
#define HASHTABLE_HEADER 

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class HashTable {

private:
	static const int TABLESIZE = 100;
	struct item {
		string keyword;
		int token;
		item *next;
	};
	item* keywordHashTable[TABLESIZE];

public:

	/* ********************************************************************
	Constructor for HashTable initializes all keywords to "empty" and token numbers to 0
	This is needed as keywords are of type string
	************************************************************************ */

	HashTable()
	{
		for (int i = 0; i < TABLESIZE; i++)
		{
			keywordHashTable[i] = new item;
			keywordHashTable[i]->keyword = "empty";
			keywordHashTable[i]->token = 0;
			keywordHashTable[i]->next = NULL;
		}
	}

	/* *****************************************************************
	The format of the file is " BEGIN  7 ". Each line contains the keyword along with the respective token number.
	Since Baby ALGO is not case sensitive. BEGIN/Begin/begin all 3 keywords will be in input file.

	Precondition: File exists
	Postcondition: Gets each line from keywords.txt and streams the string part to the keyword_input  and the int part to token_input.
	************************************************************************ */
	void ReadInput()
	{
		fstream inFile;
		inFile.open("keywords.txt");
		string line;			//Temp variable just to store each line from getline
		if (!inFile.is_open())
		{
			cout << "Input file connection failed.Check if file exists" << endl;
		}
		else
		{
			while (getline(inFile, line))
			{
				istringstream ss(line);
				string keyword_input;
				int token_input;
				ss >> keyword_input >> token_input;
				Insert(keyword_input, token_input);
			}
		}
	}

	/* *************************************************************
	Pre - Conditions: The string is not equal to end of file
	Post - Conditions : Returns an int value in which the identifier is to be placed in the hash table.

	The ascii value of each char in the string is added and mod operation is performed on the sum

	*********************************************************** */

	int Hash(string id)
	{
		int hash = 0;
		int index;
		for (int i = 0; i < id.length(); i++)
		{
			hash = hash + (int)id[i];
		}
		index = hash % TABLESIZE;
		return index;
	}

	/* **********************************************************************
	Precondition: InputFile not empty and line is read from input file
	PostCondition: keyword is inserted into HashTable
	************************************************************************** */

	void Insert(string word, int token_number)
	{
		int index = Hash(word);
		if (keywordHashTable[index]->keyword == "empty")		// Condition checks if the hashed slot in table is empty
		{
			keywordHashTable[index]->keyword = word;
			keywordHashTable[index]->token = token_number;
		}
		else
		{
			item *Ptr = keywordHashTable[index];		// initials to the head of the linked list where the postion in the hashtable is equal to the hashed value
			while (Ptr->next != NULL)					// while the next ptr of the linked list is not empty move forward in the linked list
			{
				Ptr = Ptr->next;
			}

			item* n = new item;
			n->keyword = word;
			n->token = token_number;
			n->next = NULL;
			Ptr->next = n;		// Assign the new node to the Ptr->next
		}
	}

	/* ***************************************************************************************
	Preconditions: keywords have been entered into the Hashtable
	Postcondition: Returns the token number of the key word
	****************************************************************************************** */

	int Find(string word)
	{
		int i = Hash(word);		// gets the hash value of string
		item* Ptr;
		Ptr = keywordHashTable[i];
		while (Ptr->keyword != "empty")
		{
			if (Ptr->keyword == word)	//if keyword is present
				return Ptr->token;
			else
			{
				if (Ptr->next == NULL)
					return 0;
				else
					Ptr = Ptr->next;
			}
		}
		return Ptr->token;
	}
};

#endif 
