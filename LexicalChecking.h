/*
	Members: Vinh Tran
			 Quang Nguyen
	Course: CPSC 323 - 04
	Professor: Anthony Le
	Assignment 3: Syntax Analyzer
*/

#include <iostream>
#include <string>
#include "Tools.h"

using namespace std;

/*
	Q = { 0 (ENTRY), 1 (IDENTIFIER), 2 (INTEGER), 3 (REAL) }
	q0 = 0 (ENTRY)
*/
enum FINITE_STATE_MACHINE {
	ENTRY = 0,
	IDENTIFIER = 1,
	INTEGER = 2,
	REAL = 3,
};

/*
		  |			    LETTER			        DIGIT				OTHER
	N:	  | Input:  [a...z][A...Z][$]		   [0...9]				 [.]
__________|______________________________________________________________
-->   0   |               1                       2                   0
	  1   |				  1					      1					  0
	  2   |				  0					      2					  3
	  3   |				  0					      3					  0
*/
int state[4][3] = { {IDENTIFIER, INTEGER, ENTRY},
					{IDENTIFIER, IDENTIFIER, ENTRY},
					{ENTRY, INTEGER, REAL},
					{ENTRY, REAL , ENTRY} };

string checkState(int state) {
	if (state == IDENTIFIER) {
		return "IDENTIFIER";
	}
	else if (state == INTEGER) {
		return "INTEGER";
	}
	else {
		return "REAL";
	}
}

// Check the token of a character if it is an OPERATOR
bool checkOperator(string word) {
	if (word == "*" || word == "+" || word == "-" || word == "=" || word == "/" || word == ">" || word == "<" || word == "%") {
		return true;
	}
	return false;
}

// Check the token of a character if it is a SEPARATOR
bool checkSeparator(string word) {
	if (word == "'" || word == "(" || word == ")" || word == "{" || word == "}" || word == "[" || word == "]" || word == "," ||
		word == "." || word == ":" || word == ";") {
		return true;
	}
	return false;
}

// Check the token of a word if it is an IDENTIFIER, INTEGER, REAL, or KEYWORD
string lexer(string word)
{
	// Check if 'word' is a KEYWORD
	if (word == "int" || word == "float" || word == "bool" || word == "true" || word == "false" || word == "if" || word == "else" || word == "then" ||
		word == "endif" || word == "while" || word == "do" || word == "for" || word == "input" || word == "output" || word == "and" || word == "or" ||
		word == "not" || word == "whileend" || word == "double") {
		return "KEYWORD";
	}
	// Check if 'word' is an IDENTIFIER, INTEGER, REAL
	else {

		// Initialize the entry state for FSM
		int currentState = 0;

		for (int c = 0; c < word.length(); c++) {
			int col;

			if (word[c] == '.') {
				col = 2;
			}
			else if (isdigit(word[c])) {
				col = 1;
			}
			else {
				col = 0;
			}
			currentState = state[currentState][col];
		}
		return checkState(currentState);
	}
}


// Check each letter in a string
LinkedList<string> checkWord(string line, int& blockComment)
{
	string word;
	string testChar;
	LinkedList<string> list;

	for (int c = 0; c < line.length(); c++) {
		testChar.clear();
		testChar = line[c];

		if (testChar == "!")
			blockComment++;
		// If the program see ! and the blockComment variable is not == to 2
		// it will skip that char
		if (blockComment == 1 || blockComment == 2) {
			if (blockComment == 2) {
				blockComment = 0;
			}
			continue;
		}

		// This is to check if the word is not empty and the first char of the word
		// is not a letter but testWord is then it will do a lexical
		// checking the word
		else if (!isEmpty(word) && !isalpha(word[0]) && isalpha(testChar[0])) {
			list.push_back(lexer(word), word);
			word.clear();
		}
		else if (testChar == " " || testChar == "\t") {
			if (!word.empty()) {
				list.push_back(lexer(word), word);
				word.clear();
			}
			continue;
		}
		// Check if 'testWord' is an OPERATOR
		else if (checkOperator(testChar)) {
			if (!word.empty()) {
				list.push_back(lexer(word), word);
				word.clear();

				if (testChar == "<" || testChar == ">" || testChar == "=") {
					if (c < line.length() - 1) {
						string temp;
						temp = line[c + 1];
						if (temp == "=") {
							testChar += temp;
							c += 2;
						}
					}
				}

				list.push_back("OPERATOR", testChar);
			}
			else {
				if (testChar == "<" || testChar == ">" || testChar == "=") {
					if (c < line.length() - 1) {
						string temp;
						temp = line[c + 1];
						if (temp == "=") {
							testChar += temp;
							c += 1;
						}
					}
				}

				list.push_back("OPERATOR", testChar);
			}
			continue;
		}
		// Check if 'testWord' is a SEPARATOR
		else if (checkSeparator(testChar)) {
			if (!word.empty()) {
				if (testChar == ".") {
					// The program will loop through the word to see if there are any digit on it
					for (int i = 0; i < word.length(); i++)
					{
						// If it found one that is not, it will do the lexical checking for that word
						// before pushing "." in the list
						if (!isdigit(word[i]))
						{
							list.push_back(lexer(word), word);
							word.clear();

							list.push_back("SEPARATOR", testChar);
							break;
						}
						// If it reaches the end and no non-digit char is found then it will push "." into word
						if (i == word.length() - 1)
						{
							word += line[c];
							// If it is at the end of the line then do a lexical checking for the word
							if (c == line.length() - 1)
							{
								list.push_back(lexer(word), word);
								word.clear();
							}
							break;
						}
					}
				}
				// This is for if the testWord is not "."
				else
				{
					list.push_back(lexer(word), word);
					word.clear();

					list.push_back("SEPARATOR", testChar);
				}
			}
			else {
				list.push_back("SEPARATOR", testChar);
			}
			continue;
		}

		// Combine a single checked character of 'testWord' into 'word'
		word += line[c];

		// Check the last character of a string
		if (c == line.length() - 1) {
			list.push_back(lexer(word), word);
			word.clear();
		}

	}

	return list;
}