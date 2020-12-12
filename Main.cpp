/*
	Members: Vinh Tran
			 Quang Nguyen
	Course: CPSC 323 - 04
	Professor: Anthony Le
	Assignment 2: Syntax Analyzer
*/

#include "LinkedList.h"
#include "LexicalChecking.h"
#include "Tools.h"
#include "SyntaxAnalyzer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

int main() {
	// Variable for reading user's input for selecting the mode of the syntax analyzer
	int choice;
	ofstream writeFile("output.txt");
	writeFile << left;
	// Design the menu driven for the program
		// Mode 1: Let user input a partial code and do the syntax analyzer
		// Mode 2: Do the syntax analyzer from a source file
	cout << "\t=================================\n"
		<< "\t   WELCOME TO SYNTAX ANALYZER\n"
		<< "\t=================================\n"
		<< "\t           MAIN MENU\n"
		<< "   1. Manual\n"
		<< "   2. Read from a file\n"
		<< "   3. Exit\n\n";

	// Get and check user's input
	// If user inputted '1', the program will be in Mode 1
	// If user inputted '2', the program will be in Mode 2
	// If user inputted '3', the program will be terminal
	// If user's input is bad, ask them to input again
	choice = getBoundedInt("Select your choice [Integer Only]: ", 1, 3);

	// Variable for counting and checking a single or block comment
	int blockComment = 0;

	// Define the instance named "list" for the object "LinkedList"
	// "list" will contain two data which are tokens and lexemes
	LinkedList<string> list;
	LinkedList<string> temp;

	switch (choice) {
	case 1:
	{
		string line;
		cout << "\nInput your code: ";
		// Get user's input
		getline(cin, line);
		// Call function "checkWord()" to do the lexical analyzer
		// Then store the data (tokens and lexemes) into "list" if any
		list = checkWord(line, blockComment);
		cout << endl;
		if (!list.isEmpty()) {
			// Call function "syntax_analyzer()" to do the syntax analyzer
			syntax_analyzer(list, writeFile);
		}
		else {
			cout << "YOU INPUTTED A COMMENT NOT THE PARTIAL SOURCE CODE.\n";
			break;
		}
		cout << "\nSYNTAX ANALYZER COMPLETED.\n";
		cout << "\nSyntax Analyzers saved into a file \"output.txt\" as default.\n";

	} break;
	case 2:
	{
		string fileName;
		while (true) {
			cout << "\nInput the file's name: ";
			// Get user's input
			getline(cin, fileName);
			cout << endl;
			// Open the file
			fstream myFile(fileName);
			if (myFile.is_open())
			{
				// Read line by line from a file
				// Then call function "checkWord()" to do the lexical analyzer of each line
				// Store the data (tokens and lexemes) into "list" if any
				string readFile;
				while (getline(myFile, readFile)) {
					temp.clear();
					temp = temp + checkWord(readFile, blockComment);
					list = list + temp;
				}
				if (!list.isEmpty()) {
					// Call function "syntax_analyzer()" to do the syntax analyzer of each line
					syntax_analyzer(list, writeFile);
				}
				cout << "\nSYNTAX ANALYZER COMPLETED.\n";
				cout << "\nSyntax Analyzers saved into a file \"output.txt\" as default.\n";
			}
			else {
				// Show the error and ask user's input again
				cout << "Could not open \"" << fileName << "\" file or file does not exist.\n";
				continue;
			}
			// Close the file
			myFile.close();
			break;
		}
	} break;
	default:
	{
		// The program will be terminal when entered '3'
		cout << "The program was terminated.\n";
		return 0;
	}
	}
	return 0;
}
