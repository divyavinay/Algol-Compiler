
/* ************************************ PROGRAM DETAILS ******************************
Divya Basappa, CS 4110, 14th Feb 2017
Compiler Version:Microsoft (R) C/C++ Optimizing Compiler Version 17
IDE: Visual Studio

Methods:
GetChar: Reads input file and stores in buffer.
PrintToScreen: Prints token number and lexeme
BackUpFunction: Moves one position left in input_line

Program generate an intermediate file with token number and lexeme for the parser. Steps followed by the program
1.Reads a line from input file and stores in buffer.
2.Buffer is then passed to getChar where each char from the buffer is passed to scanner to get token
	if char is part of a id or literal it loops until a char with a different token is found.
	In which case BackupFunction is called to move left by ome char and then print.
3. The token number along with lexeme is stored in outputlisting for the parser.
* **************************************************************************************/

#include "stdafx.h"
#include <string>
#include "HashTable.h"
#include "Scanner.h"

using namespace std;

class BufferClass {

private:
	
	ofstream output_listing;
	int tokenNumber = 0;
	HashTable hashObj;
	ScannerClass scan;
	string FinalWord;
	int i = 0;						// iterator
	//fstream tokenFile;

public:
	int lineNumber = 1;
	
	/* *****************************************************************************
	Constructor takes an object of hashtable and Scanner as a parameters.The keywords are loaded into hashTable in ReadInput
	function of Hashtable class.
	******************************************************************************* */
	BufferClass(HashTable hash, ScannerClass scannerObj)
	{
		hashObj = hash;
		scan = scannerObj;
		hash.ReadInput();
	}

	/* *****************************************************************************
	Preconditions: File exists
	Postconditions: Reads the infile one line at a time and stores in buffer.

	After storing each line in the buffer passes each character to getToken method of scanner class.
	******************************************************************************** */
	void getChar(string input_line)
	{
		lineNumber++;
		i = 0;
		FinalWord.clear();
		do {
			while (tokenNumber == -2) {				//when the word is comment we get return value as -2
				IgnoreComment(input_line);
				if (i > input_line.size())				// on returning we check if array is out of bounds if yes we return to calling function
					return;
			}
			tokenNumber = scan.getToken(input_line[i], hashObj);			// get token number for current character
			if (FinalWord.empty() && input_line[i] != '\0' && input_line[i] != 32) {
				// if current character is not an end of string or white space add to string to display later
				FinalWord = FinalWord + input_line[i];
			}
			while (tokenNumber == 2 && i <= input_line.size()) {
				// if token number is 2 it is a literal and we loop to check if next char is also a literal
				literalloop(input_line);
			}
			while (tokenNumber == 6) {		//if token is a relational operator
				relationalOperators(input_line);
			}
			// Loop for identifiers
			for (int j = i + 1; (j <= input_line.size() && tokenNumber == 1); j++) {
				if (input_line[j] == '\0' || input_line[j] == 32) {
					/* if next char is a end of sting or white space, check if the string in final word is
					a keyword and get its token number else return 1 for identifier */
					tokenNumber = hashObj.Find(FinalWord);
					if (tokenNumber == 0)				// if not keyword returns 0
						tokenNumber = 1;
					else {
						if (tokenNumber != -2)			// checks if keyword is a comment
							PrintToScreen(tokenNumber, FinalWord, "");
					}
				}
				else {
					FinalWord = FinalWord + input_line[j];
					tokenNumber = scan.getToken(input_line[j], hashObj);	// get token for next character
					i = j;
				}
			}
			//if string length is more than 2 and current char returns a token that is not an id, call backup function to print the string.
			if (tokenNumber != 1 && FinalWord.size()>1 && tokenNumber != -2)
				BackUpFunction(FinalWord, hashObj);
			else if ((input_line[i] != 32 || input_line[i] != '\0') && tokenNumber == 0 && !FinalWord.empty()) {
				if (input_line[i] == ':')
				{
					i++;
					checkNextCharacter(input_line[i]);
				}
				else
					PrintToScreen(0, FinalWord, " Illegal Character");
			}
			// Print if it is not a comment and has a valid token
			if (tokenNumber != 0 && tokenNumber != -2) {
				if (tokenNumber == 17 && input_line[i + 1] == '=') {
					i++;
					checkNextCharacter(input_line[i]);
				}
				PrintToScreen(tokenNumber, FinalWord, "");
			}
			i++;
		} while (i <= input_line.size() && input_line[i] != '\0');
	}

	/* *****************************************************************************
	Preconditions: Token for current char is 17
	Postconditions: Reads the next char and gets the token

	If the current token is ! doesnt print until the next char in case it is =
	******************************************************************************** */
	void checkNextCharacter(char current_char)
	{
		FinalWord = FinalWord + current_char;
		tokenNumber = scan.getToken(current_char, hashObj);
	}

	/* *****************************************************************************
	Preconditions: Comment keyword is found.
	Postconditions: Reads the buffer one character at a time until a ; is found. If no ;
	at the end of line returns an error

	Loops until ; is found. Each character is added to a string to display at the end.
	******************************************************************************** */
	void IgnoreComment(string input_line)
	{
		while (input_line[i] != ';')
		{
			FinalWord = FinalWord + input_line[i];			//add each character to string to print later
			i++;
			if (i > input_line.size())						// condition to check end of line
			{
				PrintToScreen(0, FinalWord, "Error!! No closing comment");
				return;
			}
		}
		tokenNumber = scan.getToken(input_line[i], hashObj);	// check to see if current char is ;
		if (tokenNumber == 16)
		{
			PrintToScreen(0, FinalWord, "");			// print the ; after the comment
														//tokenNumber = temp_token_number;
		}
	}

	/* *****************************************************************************
	Preconditions: Current character is a literal.
	Postconditions: Reads the buffer one character at a time until the current character is a literal
	and adds the current character to string to display later.

	Loops and reads each character and gets token. Loop exits when token is not a literal.
	******************************************************************************** */
	void literalloop(string input_line)
	{
		i++;
		FinalWord = FinalWord + input_line[i];				// add current char to string
		if (input_line[i] == '\0')							// if end of string reached return to calling function
		{
			PrintToScreen(tokenNumber, FinalWord, "");
			return;
		}
		else
		{
			tokenNumber = scan.getToken(input_line[i], hashObj);	// get the token number of current character
			if (tokenNumber != 2)
				// when current character is not a literal call backup functions which backs up by a character
				BackUpFunction(FinalWord, hashObj);
		}
	}

	/* *****************************************************************************
	Preconditions: Current character is a relational operator.
	Postconditions:Returns token number 6
	******************************************************************************** */
	void relationalOperators(string input_line)
	{
		if (input_line[i] == '\0' || input_line[i] == 32)
			return;
		else
		{
			tokenNumber = scan.getToken(input_line[i], hashObj);
			if (tokenNumber == 6)
				PrintToScreen(tokenNumber, FinalWord, "");
			else
				PrintToScreen(tokenNumber, FinalWord, "Illegal token");
		}
	}

	/* *****************************************************************************
	Prints the token number and lexeme.And clears the string and sets token number to 0
	******************************************************************************** */
	void PrintToScreen(int token_number, string lexeme, string error_msg)
	{	
		output_listing.open("outputListing.txt", std::ios::app);
		if (output_listing)
			output_listing << token_number<< string(lexeme) << endl;
		//cout << token_number << lexeme;
		FinalWord.clear();
		tokenNumber = 0;
		output_listing.close();
	}

	/* *****************************************************************************
	Preconditions: Final word is not empty.
	Postconditions:Goes left by one char and returns the token for the string in Finalword.

	******************************************************************************** */
	int BackUpFunction(string finalword, HashTable obj)
	{
		FinalWord = finalword.erase(finalword.size() - 1);			// remove the last char in final word
		if (FinalWord.size() > 1)
			tokenNumber = obj.Find(finalword);						// check if the string is a keyword
		else
			tokenNumber = scan.getToken(FinalWord[0], obj);		// if the string contains only one char get its token
		if (tokenNumber == 0 && FinalWord != " ")
		{
			if (isalpha(FinalWord[0]))
				// if its not a keyword or a spl char with token check if its a letter if yes set token to id
				tokenNumber = 1;
			else
				tokenNumber = 2;      //else token is a digit
		}
		i = i - 1;
		PrintToScreen(tokenNumber, FinalWord, "");
		return tokenNumber;
	}

	/* *****************************************************************************
	Preconditions: Token number exits
	Postconditions:Writes to an outputListing file for the parser
	******************************************************************************** */
	void GenerateOutputFile()
	{
		ifstream inFileScanner;
		string buffer;
		inFileScanner.open("InputFile.txt");
		if (inFileScanner.is_open())
		{
			while (getline(inFileScanner, buffer))
			{
				getChar(buffer);
			}

			if (inFileScanner.eof())
				PrintToScreen(-1, "EOF", "");		
		}
	}
};

