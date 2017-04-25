
//	Author 			: Mostafa Bassem Heiba
// 	Description 		: Advanced Encryption ALgorithm (AES) 
//  	Date			: 29/11/2015

#include <iostream>
#include <fstream>
#include <string>
using namespace std;


/*States*/
typedef enum
{
	START,
	INASSIGN,
	INCOMMENT,
	INNUM,
	INID,
	DONE
}StateType;

void Scan()
{
	string preventReplica = "";
	/*Open Input File*/
	ifstream Scanner_In;
	Scanner_In.open("tiny_sample_code.txt");
	/*Open Output File*/
	ofstream Scanner_Out;
	Scanner_Out.open("scanner_output.txt");
	/*Position used to determine current position in the file*/
	int Position = 0;
	/*Line is the current line being parsed*/
	string Line;
	/*Token is the current symbol being processed */
	string Token = "";
	StateType State;
	/*Reading Input File*/
	while (!Scanner_In.eof())
	{
		if (Position == Line.length())
		{
			/*Start new line & return carriage*/
			getline(Scanner_In, Line);
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
		switch (c)
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
		case '<':
		case '>':
		case ';':
		{
			Scanner_Out << (char)c << endl;
			preventReplica[0] = (char)c; // useless can be removed
			break;
		}
		case '=':
		{
			if (preventReplica != ":=")
			{
				if (Line[Position - 2] == ':')
				{
					Scanner_Out << ":=" << endl;
				}
				else if (Line[Position - 2] != ':')
				{
					Scanner_Out << "=" << endl;
				}
			}
			break;
		}
		case ':':
		{
			if (Line[Position] == '=')
			{
				Scanner_Out << ":=" << endl;
				preventReplica = ":=";
			}
			break;
		}
		default:
			break;
		}
		if (Token != "")
		{
			/* Case a digit */
			if (isdigit(Token[0]))
			{
				Scanner_Out << "number" << endl;
				preventReplica = "number";
			}
			/* Case Reserved word */
			else if ((Token == "if") || (Token == "then") || (Token == "else") ||
				(Token == "end") || (Token == "repeat") || (Token == "until") ||
				(Token == "read") || (Token == "write"))
			{
				Scanner_Out << Token << endl;
				preventReplica = Token;
			}
			/* Case Identifier */
			else
			{
				Scanner_Out << "identifier" << endl;
				preventReplica = "identifier";
			}
			/* Free Token line */
			Token = "";
		}
		

	}
}

/*Input to Parser (Output of Scanner)*/
ifstream Parser_Input;
/*Output of Parsing Process*/
ofstream Output_Parser;
static string CurrentToken;

/*Get Next Token*/
string getToken()
{
	string Retval;
	if (Parser_Input.eof())
	{
		Retval = "EOF";
	}
	else
	{
		getline(Parser_Input, Retval);
	}
	return Retval;
}
/*Prototype of Functions used in Parsing*/
static void exp();
static void match(string Token);
static void stmt_sequence();
static void statment();
static void if_stmt();
static void simple_exp();
static void term();
static void factor();
static void read_stmt();
static void repeat_stmt();
static void assign_stmt();
static void write_stmt();



static void match(string Token)
{
	if (CurrentToken == "EOF")
	{
		return;
	}
	else if (CurrentToken == Token)
	{
		CurrentToken = getToken();
	}
	else
	{
		Output_Parser << "Unexpected Error Occured ! " << CurrentToken << endl;
		exit(1); /* Unsuccessful termination*/
	}
}
static void stmt_sequence()
{
	statment();
	while (CurrentToken == ";")
	{
		match(";");
		statment();
	}
	//Output_Parser << "stmt_sequence parsed " << endl;
}
static void statment()
{
	if (CurrentToken == "if")
	{
		if_stmt();
	}
	else if (CurrentToken == "repeat")
	{
		repeat_stmt();
	}
	else if (CurrentToken == "identifier")
	{
		assign_stmt();
	}
	else if (CurrentToken == "read")
	{
		read_stmt();
	}
	else if (CurrentToken == "write")
	{
		write_stmt();
	}
}
static void if_stmt()
{
	match("if");
	exp();
	match("then");
	stmt_sequence();
	if (CurrentToken == "else")
	{
		match("else");
		stmt_sequence();
	}
	match("end");
	Output_Parser << "If Statment Found" << endl;
}
static void repeat_stmt()
{
	match("repeat");
	stmt_sequence();
	match("until");
	exp();
	Output_Parser << "Repeat Statement Found" << endl;

}
static void assign_stmt()
{
	match("identifier");
	match(":=");
	exp();
	Output_Parser << "Assign Statement Found" << endl;

}
static void read_stmt()
{
	match("read");
	match("identifier");
	Output_Parser << "Read Statement Found" << endl;
}
static void write_stmt()
{
	match("write");
	exp();
	Output_Parser << "Write Statement Found" << endl;

}
static void exp()
{
	simple_exp();
	if (CurrentToken == "<" || CurrentToken == "=")
	{
		match(CurrentToken);
		simple_exp();
	}
	//Output_Parser << "exp parsed " << endl;
}
static void simple_exp()
{
	term();
	while (CurrentToken == "+" || CurrentToken == "-")
	{
		match(CurrentToken);
		term();
	}
	Output_Parser << "Simple Expression Found" << endl;
}
static void term()
{
	factor();
	while (CurrentToken == "*")
	{
		match("*");
		factor();
	}
	Output_Parser << "Term Found" << endl;
}
static void factor()
{
	if (CurrentToken == "(")
	{
		match("(");
		exp();
		match(")");
	}
	else if (CurrentToken == "number")
	{
		match("number");
	}
	else if (CurrentToken == "identifier")
	{
		match("identifier");
	}
	Output_Parser << "Factor Found" << endl;
}
void Parse()
{
	Output_Parser.open("parser_output.txt");
	CurrentToken = getToken();
	stmt_sequence();
	Output_Parser << "Program Found" << endl;
	Output_Parser.close();
}

int main()
{
	Scan();
	Parser_Input.open("scanner_output.txt");
	Parse();
	return 0;
}
