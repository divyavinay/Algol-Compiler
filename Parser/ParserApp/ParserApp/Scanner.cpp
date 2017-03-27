// Scanner.cpp : Defines the entry point for the console application.
//

/* ************************************* PROGRAM DETAILS ******************************
Methods:
GetChar: Reads input file and stores in buffer.
GetToken: Compares to all legal tokens and returns a Token number. For all illegal Tokens token number returned is -1

*************************************************************************************** */


#include "stdafx.h"
#include <string>
#include "HashTable.h"

using namespace std;

enum States {
	S0							//initial state
	, S1						// All identifiers/ keywords (Final State) 
	, S2						// anything thats not a " or a eof
	, S3						// accepts " and returns 2 (Final State)					
	, S4						// digits 1 to 9 (Final State)
	, S5						//  0 (Final State)
	, S6						// + ,- (Final State)
	, S7					// * , / (Final State)
	, S8					// =, <, >(Final State)
	, S9					// ! (Final State)
	, S10					// = (Final State) for !=
	, S11					// ( (Final State)
	, S12					// ) (Final State)
	, S13					// ; (Final State)
	, S14					//:
	, S15					// = (Final State) for :=
	, S16					// . (Final State)
	, S20
};

class ScannerClass
{
private:
	int tokenNumber = 0;
	States currentState;

public:
	/* *****************************************************************************
	Constructor sets the inital state to S0
	******************************************************************************* */
	ScannerClass()
	{
		currentState = S0;
	}

	/* *****************************************************************************
	Preconditions: Input char is not null
	Postconditions:Returns the token for the character
	******************************************************************************* */
	int getToken(char input_char, HashTable obj) {
		if (currentState == S0) {
			if (input_char == 32) currentState = S0;
			else if (isalpha(input_char))currentState = S1;
			else if (input_char == '"') currentState = S2;
			else if (isdigit(input_char) && input_char != 0) currentState = S4; // prevents the first char from being 0
			else if (input_char == '0') currentState = S5;
			else if (input_char == '+' || input_char == '-') currentState = S6;
			else if (input_char == '*' || input_char == '/') currentState = S7;
			else if (input_char == '>' || input_char == '<' || input_char == '=') currentState = S8;
			//else if (input_char == '\0' || input_char == ' ') currentState = S20;
			else if (input_char == '!') currentState = S9;
			else if (input_char == '(') currentState = S11;
			else if (input_char == ')') currentState = S12;
			else if (input_char == ';') currentState = S13;
			else if (input_char == ':') currentState = S14;
			else if (input_char == '.') currentState = S16;
			tokenNumber = 0;
		}

		switch (currentState) {
		case S0:
			break;

		case S1: // Checks if character is an identifier
			if (isalpha(input_char))
				tokenNumber = 1;
			currentState = S0;
			break;

		case S2: // Checks if
			currentState = S3;
			tokenNumber = 2;
			break;

		case S3:if (input_char == '"')
		{
			currentState = S0;
			tokenNumber = 2;
		}
				break;

		case S4: //Checks if character is a digit.
			if (isdigit(input_char))
				tokenNumber = 2;
			else
				tokenNumber = 0;
			currentState = S0;
			break;

		case S5: // checks if character is 0
			currentState = S0;
			tokenNumber = 2;
			break;

		case S6: // checks for additional operators
			currentState = S0;
			tokenNumber = 4;
			break;

		case S7:
			currentState = S0;
			tokenNumber = 5;
			break;

		case S8: // checks for relational operators
			currentState = S0;
			tokenNumber = 6;
			break;

		case S9: // checks for boolean not and changes state to S10 to check if next char is =
			currentState = S10;
			tokenNumber = 17;
			break;

		case S10:if (input_char == '=')
			tokenNumber = 6;
				 else if (isalpha(input_char))
					 tokenNumber = 1;
				 else if (isdigit(input_char))
					 tokenNumber = 2;
			currentState = S0;
			break;

		case S11: // Checks for '('
			currentState = S0;
			tokenNumber = 14;
			break;

		case S12: // Checks for ')'
			currentState = S0;
			tokenNumber = 15;
			break;

		case S13: // Checks for ';'
			currentState = S0;
			tokenNumber = 16;
			break;

		case S14: //checks for ':' and changes state to S15 in case next char is '='
			currentState = S15;
			tokenNumber = 0;
			break;

		case S15: if (input_char == '=')
			tokenNumber = 19;
			currentState = S0;
			break;

		case S16:// Checks for '.'
			tokenNumber = 18;
			currentState = S0;
			break;

		case S20: currentState = S0;

		default:
			break;
		}
		return tokenNumber;
	}
};
