//	Author 			: Mostafa Bassem Heiba
// 	Description 		: Advanced Encryption ALgorithm (AES) 
//  	Date			: 4/11/2015

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


/******States******/
typedef enum
{
	START,
	INASSIGN,
	INCOMMENT,
	INNUM,
	INID,
	DONE
}StateType;


int main()
{
	/*Open Input File*/
	ifstream in;
	in.open("tiny_sample_code.txt");
	/*Open Output File*/
	ofstream out;
	out.open("scanner_output.txt");
	/*Position used to determine current position in the file*/
	int Position = 0;
	/*Line is the current line being parsed*/
	string Line;
	/*Token is the current symbol being processed */
	string Token = "";
	StateType State;
	/*Reading Input File*/
	while (!in.eof())
	{
		if (Position == Line.length())
		{
			/*Start new line & return carriage*/
			getline(in, Line);
			Position = 0;
		}
		State = START;
		while (State != DONE)
		{
			switch (State)
			{

			case START:
			{
				int c = Line[Position];
				if ((c == ' ') || (c == '\t') || (c == '\n')) { Position++; State = START; }
				/*about to begin a comment*/
				else if (c == '{') { State = INCOMMENT; Position++; }
				/*about to begin a assigning*/
				else if (c == ':') { State = INASSIGN; Position++; }
				/*about to type a numebr*/
				else if (isdigit(c)) { State = INNUM; Position++; }
				/*about to type an ID*/
				else if (isalpha(c)) { State = INID; Position++; }
				else { Position++; State = DONE; }
				break;
			}
			case INCOMMENT:
			{
				/*Incremenet Position untill you reach the end of the comment statement*/
				Position++;
				if (Line[Position] == '}') { State = START; }
				break;

			}
			case INNUM:
			{
				int c = Line[Position];
				if (isdigit(c)) { Token += Line[Position - 1]; Position++; }
				/*Not a digit go to DONE state*/
				else {
					Token += Line[Position - 1];
					State = DONE;
				}
				break;
			}
			case INID:
			{
				int c = Line[Position];
				if (isalpha(c)) { Token += Line[Position - 1]; Position++; }
				/*Not an Alpha goto DONE state*/
				else {
					Token += Line[Position - 1];
					State = DONE;
				}
				break;

			}
			case INASSIGN:
			{
				int c = Line[Position];
				if (c == '=') State = DONE;
				/*Not "=" goto DONE state*/
				else {
					State = DONE;
				}
				break;

			}
			}

		}

		/*It's Done Here*/
		int c = Line[Position - 1];
		/*Check for Special Symbol Here*/
		if ((c == '+') || (c == '-') || (c == '/') || (c == '(') || (c == ')')
			|| ((c == '=') && Line[Position - 2] != ':') || (c == '*') || (c == '<') || (c == ';'))
		{
			out << (char)c << ": special symbol" << endl;
		}
		/*Check for Special Symbol of the assign */
		else if ((c == ':') && Line[Position] == '=')
			out << ":=" << ": special symbol" << endl;
		else if (Token != "") {
			/*Check for Number Here*/
			if (isdigit(Token[0])) out << Token << ": number" << endl;
			/*Check for Reserved Word Here*/
			else if (((Token == "if") || (Token == "then") || (Token == "else") ||
				(Token == "end") || (Token == "repeat")
				|| (Token == "until") || (Token == "read") || (Token == "write")))
			{
				out << Token << ": reserved word" << endl;
			}
			/*If it's not one of the previous then it must be an identifier*/
			else out << Token << ": identifier" << endl;
			Token = "";
		}

	}

	return 0;
}
