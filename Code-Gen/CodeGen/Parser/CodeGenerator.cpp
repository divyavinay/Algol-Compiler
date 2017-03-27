#include "stdafx.h"
#include<iostream> 
#include <fstream>
#include <string>

using namespace std;

class CodeGeneratorClass
{

public:
	ofstream intermediate_code_file_obj;				// fstream object to write to file
	ofstream data_section_file_obj;
	int if_label_counter = 0;
	int while_label_counter = 0;
	int dataLabel_counter = 0;
	string label;
/***************************************************************
Constructor creates and opens an intermediate code file
****************************************************************/
	CodeGeneratorClass()
	{
		intermediate_code_file_obj.open("IntermediateFile.txt");
		data_section_file_obj.open("dataSection.txt");
	}

/***************************************************************
	Writes necessary lines to intermediate file
****************************************************************/
	void WriteProlog()
	{
		intermediate_code_file_obj << "	.data" << endl
			<< "ProgStart: .asciiz \"Program Start\\n\"" << endl
			<< "ProgEnd: .asciiz	\"Program End\"" << endl
			<< "	.globl main" << endl
			<<"	.code"<<endl
			<< "main:" << endl
			<< "mov $fp,$sp" << endl
			<< "la $a0,ProgStart" << endl << "li $v0,4"<<endl
			<< "syscall" << endl;

		data_section_file_obj << ".data" << endl;
	}

	void WritePostLog()
	{
		intermediate_code_file_obj << "la $a0,ProgEnd" << endl
			<< "li $v0,4" << endl
			<< "syscall" << endl
			<< "li $v0,10" << endl
			<< "syscall" << endl;
	}
	/***************************************************************
	Writes generated labels to a seperate file which is later appended
	****************************************************************/
	void CodeGeneDataSection(string data_label)
	{
		data_section_file_obj << data_label << endl;
	}

	/***************************************************************
	Writes generated code to intermediate file
	****************************************************************/
	void CodeGen(string instruction, string source_reg1,string source_reg2, string value)
	{
		intermediate_code_file_obj << instruction << " " << source_reg1 << " "<<source_reg2<<" " << value << endl;
	}
	
	void close_file()
	{
		intermediate_code_file_obj.close();
	}
	/***************************************************************
	Generates label based on lexeme
	****************************************************************/
	string GenLabel(string lexeme)
	{
		if (lexeme == "IF")
		{
			if_label_counter++;
			label = lexeme + "_Func_" + to_string(if_label_counter);
		}
		else if (lexeme == "THEN")
		{
			label = lexeme + "_Func_" + to_string(if_label_counter);
		}
		else if (lexeme == "TopWhileLabel")
		{
			while_label_counter++;
			label = lexeme + "_Func_" + to_string(while_label_counter);
		}
		else if (lexeme == "EndWhileLable")
		{
			label = lexeme + "_Func_" + to_string(while_label_counter);
		}
		else
		{
			dataLabel_counter++;
			label = "Label" + to_string(dataLabel_counter);
		}
		return label;
	}
	/***************************************************************
	Generates appropraite comments
	****************************************************************/
	void GeneComments(string comment)
	{
		intermediate_code_file_obj << "#" << comment << endl;
	}
	/***************************************************************
	Copies the .data section from data file into intermediate file at the end of parse
	****************************************************************/
	void copyDataSectionToFile()
	{
		ifstream data_file;
		string content = "";
		data_file.open("dataSection.txt");
		while(getline(data_file, content))
			intermediate_code_file_obj << content<<endl;
		intermediate_code_file_obj << ".code" << endl;
	}
	
};
