/*
	Members: Vinh Tran
			 Quang Nguyen
	Course: CPSC 323 - 04
	Professor: Anthony Le
	Assignment 3: Syntax Analyzer
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include "Tools.h"
using namespace std;

// Functions' Prototype
void statement(LinkedList<string>&, ofstream& writeFile);
void assign(LinkedList<string>&, ofstream& writeFile);
void declarative(LinkedList<string>&, ofstream& writeFile);
void if_loop(LinkedList<string>&, ofstream& writeFile);
void while_loop(LinkedList<string>&, ofstream& writeFile);
void conditionall(LinkedList<string>&, ofstream& writeFile);
bool relop(LinkedList<string>&, ofstream& writeFile);
void expression(LinkedList<string>&, ofstream& writeFile);
bool term(LinkedList<string>&, ofstream& writeFile);
void expression_prime(LinkedList<string>&, ofstream& writeFile);
bool factor(LinkedList<string>&, ofstream& writeFile);
void term_prime(LinkedList<string>&, ofstream& writeFile);
void print(LinkedList<string> data, ofstream& writeFile, string cases, string on_off);
void print_error(ofstream& writeFile, string cases);

// Start analyzing the syntax based on user's input or a input file
void syntax_analyzer(LinkedList<string> data, ofstream& writeFile) {
	statement(data, writeFile);
}

// Analyze the declarative statement or the assignment statement
void statement(LinkedList<string>& data, ofstream& writeFile) {
	while (!data.isEmpty()) {
		// Case 1: Analyze the assignment statement
		if (data.showToken() == "IDENTIFIER") {
			assign(data, writeFile);
		}
		// Case 2: Analyze the declarative statement
		else if (data.showToken() == "KEYWORD") {
			if (data.showLexeme() == "int" || data.showLexeme() == "float" || data.showLexeme() == "bool") {
				declarative(data, writeFile);
			}
			else if (data.showLexeme() == "if") {
				if_loop(data, writeFile);
			}
			else if (data.showLexeme() == "while") {
				while_loop(data, writeFile);
			}
			else if (data.showLexeme() == "else" || data.showLexeme() == "endif" || data.showLexeme() == "whileend") {
				break;
			}
			else {
				print_error(writeFile, "error keyword");
			}
		}
		// Case 3: Display the error
		else {
			print_error(writeFile, "error");
			exit(EXIT_FAILURE);
		}
	}
}

void assign(LinkedList<string>& data, ofstream& writeFile) {
	print(data, writeFile, "assign", "on");
	data.pop_front();
	if (data.isEmpty() || data.showLexeme() != "=") {
		print_error(writeFile, "error =");
		exit(EXIT_FAILURE);
	}
	else {
		print(data, writeFile, "", "on");
		data.pop_front();
		expression(data, writeFile);
		if (data.isEmpty() || data.showLexeme() != ";") {
			print_error(writeFile, "error ;");
			exit(EXIT_FAILURE);
		}
		print(data, writeFile, ";", "on");
		data.pop_front();
	}
}

void declarative(LinkedList<string>& data, ofstream& writeFile) {
	print(data, writeFile, "declarative", "on");
	data.pop_front();
	if (data.isEmpty() || data.showToken() != "IDENTIFIER") {
		print_error(writeFile, "error id");
		exit(EXIT_FAILURE);
	}
	else {
		print(data, writeFile, "id", "on");
		data.pop_front();
		if (data.isEmpty() || data.showLexeme() != "=" && data.showLexeme() != ";") {
			print_error(writeFile, "error ; or =");
			exit(EXIT_FAILURE);
		}
		else if (data.showLexeme() == "=") {
			print(data, writeFile, "assign", "off");
			print(data, writeFile, "", "on");
			data.pop_front();
			expression(data, writeFile);
		}
		if (data.isEmpty() || data.showLexeme() != ";") {
			print_error(writeFile, "error ;");
			exit(EXIT_FAILURE);
		}
		print(data, writeFile, ";", "on");
		data.pop_front();
	}
}

void if_loop(LinkedList<string>& data, ofstream& writeFile) {
	print(data, writeFile, "if", "on");
	data.pop_front();

	conditionall(data, writeFile);

	if (data.isEmpty() || data.showLexeme() != "then") {
		print_error(writeFile, "error keyword");
		exit(EXIT_FAILURE);
	}
	else {
		print(data, writeFile, "if", "on");
		data.pop_front();
		if (data.isEmpty()) {
			print_error(writeFile, "error id");
			exit(EXIT_FAILURE);
		}
	}

	statement(data, writeFile);

	if (data.isEmpty() || data.showLexeme() != "else") {
		print_error(writeFile, "error keyword");
		exit(EXIT_FAILURE);
	}
	else {
		print(data, writeFile, "if", "on");
		data.pop_front();
		if (data.isEmpty()) {
			print_error(writeFile, "error id");
			exit(EXIT_FAILURE);
		}
	}

	statement(data, writeFile);

	if (data.isEmpty() || data.showLexeme() != "endif") {
		print_error(writeFile, "error keyword");
		exit(EXIT_FAILURE);
	}
	else {
		print(data, writeFile, "if", "on");
		data.pop_front();
	}
}

void while_loop(LinkedList<string>& data, ofstream& writeFile) {
	print(data, writeFile, "while", "on");
	data.pop_front();

	conditionall(data, writeFile);

	if (data.isEmpty() || data.showLexeme() != "do") {
		print_error(writeFile, "error keyword");
		exit(EXIT_FAILURE);
	}
	else {
		print(data, writeFile, "while", "on");
		data.pop_front();
		if (data.isEmpty()) {
			print_error(writeFile, "error id");
			exit(EXIT_FAILURE);
		}
	}

	statement(data, writeFile);

	if (data.isEmpty() || data.showLexeme() != "whileend") {
		print_error(writeFile, "error keyword");
		exit(EXIT_FAILURE);
	}
	else {
		print(data, writeFile, "while", "on");
		data.pop_front();
	}
}

void conditionall(LinkedList<string>& data, ofstream& writeFile) {
	expression(data, writeFile);
	if (relop(data, writeFile))
		expression(data, writeFile);
}

bool relop(LinkedList<string>& data, ofstream& writeFile) {
	if (data.isEmpty()) {
		print_error(writeFile, "error relop");
		exit(EXIT_FAILURE);
	}
	else if (data.showLexeme() == "<" || data.showLexeme() == "<=" || data.showLexeme() == "<>" || data.showLexeme() == "==" || data.showLexeme() == ">=" || data.showLexeme() == ">") {
		print(data, writeFile, "", "on");
		data.pop_front();
		return true;
	}
	return false;
}

// Analyze the expression in the statement
void expression(LinkedList<string>& data, ofstream& writeFile) {
	if (data.isEmpty()) {
		print_error(writeFile, "error id");
		exit(EXIT_FAILURE);
	}
	if (data.showLexeme() != "(")
		print(data, writeFile, "expression", "on");
	if (term(data, writeFile)) {
		if (data.isEmpty()) {
			return;
		}
		expression_prime(data, writeFile);
	}
	else {
		print_error(writeFile, "error factor");
		exit(EXIT_FAILURE);
	}
}

// Analyze the term in the statment
bool term(LinkedList<string>& data, ofstream& writeFile) {
	if (factor(data, writeFile)) {
		data.pop_front();
		term_prime(data, writeFile);
		return true;
	}
	return false;
}

// Analyze the expression using left recursion in the statement
void expression_prime(LinkedList<string>& data, ofstream& writeFile) {
	if (data.isEmpty()) {
		return;
	}
	else if (data.showLexeme() == "+" || data.showLexeme() == "-") {
		print(data, writeFile, "expression prime", "on");
		data.pop_front();
		print(data, writeFile, "", "on");
		if (term(data, writeFile)) {
			expression_prime(data, writeFile);
		}
		else {
			print_error(writeFile, "error id");
			exit(EXIT_FAILURE);
		}
	}
}

// Analyze the factor in the statement
bool factor(LinkedList<string>& data, ofstream& writeFile) {
	if (data.isEmpty()) {
		return false;
	}
	else if (data.showLexeme() == "(") {
		print(data, writeFile, "", "on");
		data.pop_front();
		expression(data, writeFile);
		if (data.showLexeme() != ")") {
			print_error(writeFile, "error )");
			return false;
		}
		print(data, writeFile, "", "on");
		return true;
	}
	else if (data.showToken() == "IDENTIFIER") {
		print(data, writeFile, "identifier", "off");
		return true;
	}
	else if (data.showToken() == "INTEGER" || data.showToken() == "REAL") {
		print(data, writeFile, "num", "off");
		return true;
	}
	else if (data.showToken() == "KEYWORD" && (data.showLexeme() == "true" || data.showLexeme() == "false")) {
		print(data, writeFile, "bool", "off");
		return true;
	}
	return false;
}

// Analye the term using left recursion in the statement
void term_prime(LinkedList<string>& data, ofstream& writeFile) {
	if (data.isEmpty()) {
		return;
	}
	else if (data.showLexeme() == "*" || data.showLexeme() == "/") {
		print(data, writeFile, "term prime", "on");
		data.pop_front();
		if (data.isEmpty()) {
			return;
		}
		print(data, writeFile, "", "on");
		if (factor(data, writeFile)) {
			data.pop_front();
			term_prime(data, writeFile);
		}
	}
}

// Analyze the error of the source code and display on the screen and write to the file
void print_error(ofstream& writeFile, string cases) {
	string temp;

	if (cases == "error") {
		temp = "EXPECT AN IDENTIFIER OR KEYWORD";
	}
	else if (cases == "error keyword") {
		temp = "WRONG KEYWORD";
	}
	else if (cases == "error =") {
		temp = "= NEEDED";
	}
	else if (cases == "error ;") {
		temp = "; NEEDED";
	}
	else if (cases == "error )") {
		temp = ") NEEDED";
	}
	else if (cases == "error id") {
		temp = "IDENTIFIER NEEDED";
	}
	else if (cases == "error relop") {
		temp = "RELOP NEEDED";
	}
	else if (cases == "error ; or =") {
		temp = "; or = NEEDED";
	}
	else if (cases == "error factor") {
		temp = "IDENTIFIER OR INTEGER OR FLOAT NEEDED";
	}

	// Display on the screen
	cout << "SYNTAX ERROR!! " << temp << endl;
	// Write into the destination file
	writeFile << "SYNTAX ERROR!! " << temp << endl;
	return;
}

// Analyze the appropriate rules of the source code and display on the screen and write to the file
void print(LinkedList<string> data, ofstream& writeFile, string cases, string on_off) {
	cout << left;
	if (on_off == "on") {
		// Display on the screen
		cout << "Token: " << setw(20) << data.showToken() << setw(11) << "|" << "Lexeme: " << setw(20) << data.showLexeme() << endl;
		// Write into the destination file
		writeFile << "Token: " << setw(20) << data.showToken() << setw(11) << "|" << "Lexeme: " << setw(20) << data.showLexeme() << endl;
	}

	if (cases == "assign") {
		// Display on the screen
		cout << "   <Statement> -> <Assign>" << endl
			 << "   <Assign> -> <Identifier> = <Expression>;" << endl;
		// Write into the destination file
		writeFile << "   <Statement> -> <Assign>" << endl
			      << "   <Assign> -> <Identifier> = <Expression>;" << endl;
	}
	else if (cases == "declarative") {
		// Display on the screen
		cout << "   <Statement> -> <Declarative>" << endl
			 << "   <Declarative> -> <Type> <ID>" << endl
			 << "   <Type> -> int | float | bool" << endl
			 << "   <ID> -> id" << endl;
		// Write into the destination file
		writeFile << "   <Statement> -> <Declarative>" << endl
			      << "   <Declarative> -> <Type> <ID>" << endl
			      << "   <Type> -> int | float | bool" << endl
			      << "   <ID> -> id" << endl;
	}
	else if (cases == "if") {
		// Display on the screen
		cout << "   <Statement> -> if <Conditional> then <Statement> else <Statement> endif" << endl
			 << "   <Conditional> -> <Expression> <Relop> <Expression> | <Expression>" << endl;
		// Write into the destination file
		writeFile << "   <Statement> -> if <Conditional> then <Statement> else <Statement> endif" << endl
			      << "   <Conditional> -> <Expression> <Relop> <Expression> | <Expression>" << endl;
	}
	else if (cases == "while") {
		// Display on the screen
		cout << "   <Statement> -> while <Conditional> do <Statement> whileend" << endl
			 << "   <Conditional> -> <Expression> <Relop> <Expression> | <Expression>" << endl;
		// Write into the destination file
		writeFile << "   <Statement> -> while <Conditional> do <Statement> whileend" << endl
			      << "   <Conditional> -> <Expression> <Relop> <Expression> | <Expression>" << endl;
	}
	else if (cases == "id") {
		// Display on the screen
		cout << "   <ID> -> id" << endl;
		// Write into the destination file
		writeFile << "   <ID> -> id" << endl;
	}
	else if (cases == "expression") {
		// Display on the screen
		cout << "   <Expression> -> <Term> <ExpressionPrime>" << endl;
		// Write into the destination file
		writeFile << "   <Expression> -> <Term> <ExpressionPrime>" << endl;
	}
	else if (cases == "expression prime") {
		// Display on the screen
		cout << "   <TermPrime> -> Epsilon" << endl
			 << "   <ExpressionPrime> -> " << data.showLexeme() << " <Term> <ExpressionPrime>" << endl;
		// Write into the destination file
		writeFile << "   <TermPrime> -> Epsilon" << endl
			      << "   <ExpressionPrime> -> " << data.showLexeme() << " <Term> <ExpressionPrime>" << endl;
	}
	else if (cases == "term prime") {
		// Display on the screen
		cout << "   <TermPrime> -> " << data.showLexeme() << " <Factor> <TermPrime>" << endl;
		// Write into the destination file
		writeFile << "   <TermPrime> -> " << data.showLexeme() << " <Factor> <TermPrime>" << endl;
	}
	else if (cases == "identifier") {
		// Display on the screen
		cout << "   <Term> -> <Factor> <TermPrime>" << endl
			 << "   <Factor> -> ( <Expression> ) | <ID> | <Num> | <BoolType>" << endl
			 << "   <ID> -> id" << endl;
		// Write into the destination file
		writeFile << "   <Term> -> <Factor> <TermPrime>" << endl
			      << "   <Factor> -> ( <Expression> ) | <ID> | <Num> | <BoolType>" << endl
			      << "   <ID> -> id" << endl;
	}
	else if (cases == "num") {
		// Display on the screen
		cout << "   <Term> -> <Factor> <TermPrime>" << endl
			 << "   <Factor> -> ( <Expression> ) | <ID> | <Num> | <BoolType>" << endl
			 << "   <Num> -> int | float" << endl
			 << "   <BoolType> -> true | false" << endl;
		// Write into the destination file
		writeFile << "   <Term> -> <Factor> <TermPrime>" << endl
			      << "   <Factor> -> ( <Expression> ) | <ID> | <Num> | <BoolType>" << endl
			      << "   <Num> -> int | float" << endl
			      << "   <BoolType> -> true | false" << endl;
	}
	else if (cases == "bool") {
		// Display on the screen
		cout << "   <Term> -> <Factor> <TermPrime>" << endl
			 << "   <Factor> -> ( <Expression> ) | <ID> | <Num> | <BoolType>" << endl
			 << "   <Num> -> int | float" << endl
			 << "   <BoolType> -> true | false" << endl;
		// Write into the destination file
		writeFile << "   <Term> -> <Factor> <TermPrime>" << endl
			      << "   <Factor> -> ( <Expression> ) | <ID> | <Num> | <BoolType>" << endl
			      << "   <Num> -> int | float" << endl
			      << "   <BoolType> -> true | false" << endl;
	}
	else if (cases == ";") {
		// Display on the screen
		cout << "   <TermPrime> -> Epsilon" << endl
			 << "   <ExpressionPrime> -> Epsilon" << endl;
		// Write into the destination file
		writeFile << "   <TermPrime> -> Epsilon" << endl
			      << "   <ExpressionPrime> -> Epsilon" << endl;
	}
}

