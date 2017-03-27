
#ifndef SYMBOLTABLE_HEADER
#define SYMBOLTABLE_HEADER

using namespace std;

/* ****************************************** PROGRAM SUMMARY *******************************************
Divya Basappa, CS 4110, 25th Jan 2017
Compiler Version:Microsoft (R) C/C++ Optimizing Compiler Version 17
IDE: Visual Studio

Data structures used : HASH table, STACK, ARRAY
The array is an array of objects of SymbolTable class.Each of these objects are used to store a symbol table of different scopes
The hash table is used for the symbol table.
The stack is used to maintain the current scope.
There are 2 stacks i) in_scope_stack: maintains the current scope.
ii) out_of_scope : Used to print the scope 
************************************************************************ */

#include<iostream>
#include <string>
#include<stack>

using namespace std;

class SymbolTable
{
private:
	struct item {
		string identifier;
		char type;
		int scope;
		int offset;
		item* next;
	};
	static const int TableSize = 10;
	item* HashTable[TableSize];

public:
	int count_of_table = -1;	//count of symbol tables
	int scope = -1;	// maintais the count of the current scope

	SymbolTable() {
		for (int i = 0; i < TableSize; i++)
		{
			HashTable[i] = new item;
			HashTable[i]->identifier = "empty";
			HashTable[i]->scope = 0;
			HashTable[i]->next = NULL;
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
	index = hash % TableSize;
	return index;
}
/* *****************************************************************
Inserts the identifier into current symbol table aong with scope and offset
**************************************************************************** */

void Insert(string Identifier,char type, SymbolTable *current_table,int scope_of_table,int offset) 
{
	int index = Hash(Identifier); // Calls the hash function to get the index value in hashtable
	if (current_table->HashTable[index]->identifier == "empty")
	{
		current_table->HashTable[index]->identifier = Identifier;
		current_table->HashTable[index]->scope = scope_of_table +1;
		current_table->HashTable[index]->type = type;
		current_table->HashTable[index]->offset = offset;
	}
	else  // If hash table bucket is not empty create a new bucket 
	{
		item* Ptr = current_table->HashTable[index];
		item* n = new item;
		n->identifier = Identifier;
		n->scope = scope_of_table +1 ;
		n->type = type;
		n->offset = offset;
		n->next = NULL;
		while (Ptr->next != NULL)	//Traverses till the end of list
		{
			Ptr = Ptr->next;
		}
		Ptr->next = n;	//Point the current bucket to the next 
	}
}
/* **********************************************************
Pre-Conditions: Symbol table exists
Post-Conditions: Checks if string is in non local scope and returns a pointer to that object
************************************************************** */
bool Find_All(string id, stack <SymbolTable*>copy_of_scope)
{				
	item* Ptr = new item;
	while (copy_of_scope.size() != 0)
	{
		SymbolTable* obj = copy_of_scope.top();				// gets the object at top in in_scope_stack
		int index = Hash(id);
		Ptr = obj->HashTable[index];
		while (Ptr != NULL)
		{
			if (Ptr->identifier == id)
				return true;
			else
				Ptr = Ptr->next;
		}
		copy_of_scope.pop();							// pops the table at the top of copy_of_scope
	}
	return false;
}

/* **********************************************************
Pre-Conditions: Symbol table exists
Post-Conditions: Prints all symbol tables
************************************************************** */
void PrintSymbolTable(stack <SymbolTable*>copy_of_scope) 
{				
	item* Ptr;
	while (copy_of_scope.size() != 0)
	{
		SymbolTable *obj = copy_of_scope.top();
		for (int i = 0; i < TableSize; i++)
		{
			Ptr = obj->HashTable[i];
			while (Ptr != NULL && Ptr->identifier != "empty")
			{
				cout << "Scope: " << Ptr->scope;
				cout << "   " << Ptr->identifier;
				cout << "   " << Ptr->type;
				cout << "   " << Ptr->offset;
				cout << endl;
				Ptr = Ptr->next;
			}
		}
		copy_of_scope.pop();
		cout << endl;
	}
}
/* **********************************************************
Pre-Conditions: Symbol table exists
Post-Conditions: Checks if string is in current scope and returns a pointer to that object
************************************************************** */
SymbolTable::item* SymbolTable::Find_Local(string id, SymbolTable &current_table)
{
	int i = Hash(id);						// gets the hash value of string
	item* Ptr;
	Ptr = current_table.HashTable[i];
	while (Ptr != NULL)
	{
		if (Ptr->identifier == id)
			return Ptr;
		else
			Ptr = Ptr->next;
	}
	return Ptr;
}
};

#endif 