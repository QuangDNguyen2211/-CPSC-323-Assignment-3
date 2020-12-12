/*
	Members: Vinh Tran
			 Quang Nguyen

	Course: CPSC 323 - 04

	Professor: Anthony Le

	Assignment 1: Lexical Analyzer
*/

#include "Tools.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>
using namespace std;

const string WHITESPACE = " \t\n\r";

int getBoundedInt(string prompt, int lowerBound, int upperBound)
{
	ostringstream errorMessage;
	errorMessage
		<< "Range error: Input must be in the range ["
		<< lowerBound
		<< ","
		<< upperBound
		<< "].";
	return getBoundedIntWithErrorMessage(prompt, lowerBound, upperBound, errorMessage.str());
}

int getBoundedIntWithErrorMessage(string prompt, int lowerBound, int upperBound, string errorMessage)
{
	do
	{
		int userInput = getInt(prompt);
		if (userInput >= lowerBound && userInput <= upperBound)
		{
			return userInput;
		}
		cout << errorMessage << endl;
	} while (true);
}

int getInt(string prompt)
{
	do
	{
		istringstream inputLine(getLine(prompt));
		int userInput;
		inputLine >> userInput;
		if (inputLine.fail())
		{
			cout << "Input format error: please give an integer." << endl;
			continue;
		}
		char junk;
		inputLine >> junk;
		if (!inputLine.fail())
		{
			cout << "Input format error: please give ONLY an integer.\n" << endl;
			continue;
		}
		return userInput;
	} while (true);
}

string getLine(string prompt)
{
	do
	{
		cout << prompt;
		string userInput;
		getline(cin, userInput);
		if (!cin.fail())
		{
			return userInput;
		}
		cout << "Input error. Please try again." << endl;
		cin.clear();
		flushInput();
	} while (true);
}

bool isEmpty(string s)
{
	return string::npos == s.find_first_not_of(WHITESPACE);
}

void flushInput(void)
{
	cin.ignore(999, '\n');
}