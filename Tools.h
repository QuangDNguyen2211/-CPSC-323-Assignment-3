/*
	Members: Vinh Tran
			 Quang Nguyen

	Course: CPSC 323 - 04

	Professor: Anthony Le

	Assignment 1: Lexical Analyzer
*/

#include <string>
#include <fstream>
using namespace std;

extern const string WHITESPACE;

int		getInt(string prompt);
string	getLine(string prompt);
void flushInput(void);
bool isEmpty(string s);
int		getBoundedInt(string prompt, int lowerBound, int upperBound);
int getBoundedIntWithErrorMessage(string prompt, int lowerBound, int upperBound, string errorMessage);




