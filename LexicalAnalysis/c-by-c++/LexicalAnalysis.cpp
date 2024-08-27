#include <cstdio>
#include<map>
#include<vector>
#include<cstring>
#include<string>
#include<string.h>
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<fstream>
#include<sstream>

using namespace std;

struct Token
{
	string name;
	string value;
};

class LexicalAnalysis
{
private:
	string text;
	int pointer = 0;
	vector<struct Token> Token;
	string Key[32] = {"auto", "short", "int", "long", "double", "char", "struct", "union", "enum", "typedef", "const", "unsigned", "signed", "extern", "register", "static", "volatile", "void", "if", "else", "switch", "case", "for", "do", "while", "goto", "continue", "break", "default", "sizeof", "return"};
	string Macro[5] = {"include", "define", "ifdef", "ifndef", "endif"};

	bool Analyse(string& error)
	{
		struct Token token;
		for(; this->pointer < this->text.length(); )
		{
			char c = text[this->pointer];
			
			if(c == ' ' || c == '\n' || c == '\t')
			{
				this->pointer++;
				continue;
			}

			if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
			{
				string value;
				if(isId(value))
				{
					if(isKey(value))
					{
						token.name = "key";
						token.value = value;
						this->Token.push_back(token);
					}
					else
					{
						token.name = "id";
						token.value = value;
						this->Token.push_back(token);
					}
				}
				else
				{
					error = value;
					return false;
				}
			}
			else if(c == '0')
			{
				string value = "0";
				this->pointer++;

				if(this->text[this->pointer] == 'x' || this->text[this->pointer] == 'X')
				{
					value += this->text[this->pointer++];

					value += getNumber();
					
					token.name = "hex";
					token.value = value;

					this->Token.push_back(token);
				}
				else 
				{
					value += getNumber();
					
					if(this->text[this->pointer] == '.' && (this->text[this->pointer + 1] >= '0' || this->text[this->pointer + 1] <= '9'))
					{
						value += this->text[this->pointer++];
						value += getNumber();

						token.name = "float";
						token.value = value;

						this->Token.push_back(token);
					}	
					else
					{
						token.name = "integer";
						token.value = value;
						this->Token.push_back(token);
					}
				}

			}
			else if(c >= '0' && c <= '9')
			{
				string value = getNumber();
				
				if(this->text[this->pointer] == '.' && (this->text[this->pointer + 1] >= '0' || this->text[this->pointer + 1] <= '9'))
				{
					value += this->text[this->pointer++];
					
					value += getNumber();

					token.name = "float";
					token.value = value;

					this->Token.push_back(token);
				}
				else
				{
					token.name = "integer";
					token.value = value;
					this->Token.push_back(token);
				}
			}
			else if(c == '\"')
			{
				string value = "";
				
				c = this->text[++this->pointer];
				while(c != '\"')
				{
					value += c;
					c = this->text[++this->pointer];
				}

				this->pointer++;
				token.name = "string";
				token.value = value;

				this->Token.push_back(token);
			}
			else if(c == '#')
			{
				string value = "";
				
				c = this->text[++this->pointer];
				while((c >= 'a') && (c <= 'z'))
				{
					value += c;
					if(this->pointer  == this->text.length())
					{
						break;
					}
					c = this->text[++this->pointer];
				}

				if(isMacro(value))
				{
					token.name = "macro";
					token.value = '#' + value;

					this->Token.push_back(token);
				}
				else
				{
					error = '#' + value;
					return false;
				}
			}
			else if(c == '/')
			{
				string value = "";

				c = this->text[++this->pointer];

				if(c == '/')
				{
					c = this->text[++this->pointer];
					while(this->pointer != this->text.length() && c != '\n')
					{
						value += c;
						c = this->text[++this->pointer];
					}

					token.name = "comment";
					token.value = value;

					this->Token.push_back(token);
				}
				else if(c == '=')
				{
					value += c;

					token.name = "divisionequal";
					token.value = value;
					this->Token.push_back(token);
				}
				else
				{
					value = "/";
					token.name = "slash";
					token.value = value;

					this->Token.push_back(token);
				}
			}
			else if(c == '=')
			{
				string value ="=";

				c = this->text[++this->pointer];

				if(c == '=')
				{
					value += '=';
					
					token.name = "dequal";
					token.value = value;
					
					this->pointer++;
				}
				else
				{
					token.name = "equal";
					token.value =value;
				}

				this->Token.push_back(token);
			}
			else if(c == '!')
			{
				string value = "!";

				c= this->text[++this->pointer];

				if(c == '=')
				{
					value += c;
					
					token.name = "nequal";
					token.value = value;

					this->pointer++;
				}
				else
				{
					token.name = "exclamatory";
					token.value = value;
				}

				this->Token.push_back(token);
			}
			else if(c == '<')
			{
				string value = "<";

				c = this->text[++this->pointer];

				if( c == '=')
				{
					value += c;

					token.name = "lequal";
					token.value = value;

					this->pointer++;
				}
				else if(c == '<')
				{
					value += c;

					c = this->text[++this->pointer];

					if(c == '=')
					{
						value += c;
						token.name = "lshiftequal";
						token.value = value;

						this->pointer++;
					}
					else
					{
						token.name = "lshift";
						token.value = value;
					}
				}
				else
				{
					token.name = "langle";
					token.value = value;
				}

				this->Token.push_back(token);
			}
			else if(c == '>')
			{
				string value = ">";

				c = this->text[++this->pointer];

				if( c == '=')
				{
					value += c;

					token.name = "gequal";
					token.value = value;

					this->pointer++;
				}
				else if(c == '>')
				{
					value += c;

					c = this->text[++this->pointer];

					if(c == '=')
					{
						value += c;
						token.name = "rshiftequal";
						token.value = value;

						this->pointer++;
					}
					else
					{
						token.name = "rshift";
						token.value = value;
					}
				}
				else
				{
					token.name = "rangle";
					token.value = value;
				}

				this->Token.push_back(token);
			}		
			else if(c == '+')
			{
				string value = "+";

				c = this->text[++this->pointer];

				if(c == '+')
				{
					value += c;

					token.name = "dplus";
					token.value = value;

					this->pointer++;
				}
				else if(c == '=')
				{
					value += c;

					token.name = "plusequal";
					token.value = value;

					this->pointer++;
				}
				else
				{
					token.name = "plus";
					token.value = value;
				}

				this->Token.push_back(token);
			}
			else if(c == '-')
			{
				string value = "-";

				c = this->text[++this->pointer];

				if(c == '-')
				{
					value += c;

					token.name = "dminus";
					token.value = value;

					this->pointer++;
				}
				else if(c == '=')
				{
					value += c;

					token.name = "minusequal";
					token.value = value;

					this->pointer++;
				}
				else
				{
					token.name = "minus";
					token.value = value;
				}

				this->Token.push_back(token);
			}
			else if(c == '*')
			{
				string value = "*";
				
				c = this->text[++this->pointer];

				if(c == '=')
				{
					value += c;

					token.name = "timesequal";
					token.value = value;

					this->pointer++;
				}
				else
				{
					token.name = "star";
					token.value = value;
				}

				this->Token.push_back(token);
			}
			else if(c == '\'')
			{
				/*if(this->pointer + 2 >= this->text.length())
				{
					error = c;
					this->pointer++;
					while(this->pointer < this->text.length())
					{
						error += this->text[this->pointer];
						this->pointer++;
					}	
					return false;
				}*/
				string value = "";
				c = this->text[++this->pointer];

				if(c == '\\')
				{
					value += c;

					c = this->text[++this->pointer];

					value += c;

					c = this->text[++this->pointer];

					if(c == '\'')
					{
						token.name = "char";
						token.value = value;
					}
					else
					{
						error = value + c;
						return false;
					}
				}
				else
				{
					value += c;

					c = this->text[++this->pointer];
					
					if(c == '\'')
					{
						token.name = "char";
						token.value = value;
					}
					else
					{
						error = value + c;
						return false;
					}
				}

				this->pointer++;

				/*
				if(this->text[this->pointer + 1] == '\'')
				{
					value += c;

					token.name = "char";
					token.value = value;

					this->pointer += 2;
				}
				else if(c == '\\')
				{
					value += c;

					c = this->text[++this->pointer];

					value += c;

					c = this->text[++this->pointer];

					if(c != '\'')
					{
						error = '\'' + value + c;
						return false;
					}

					this->pointer++;
				}
				else
				{
					error = '\'' + c  + this->text[this->pointer];
					return false;	
				}
				*/

				this->Token.push_back(token);
			}
			else if(c == '{')
			{
				string value = "";
				value += c;

				token.name = "lbrace";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}
			else if(c == '}')
			{
				string value = "";
				value += c;

				token.name = "rbrace";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}
			else if(c == '[')
			{
				string value = "";
				value += c;

				token.name = "lbracket";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}
			else if(c == ']')
			{
				string value = "";
				value += c;

				token.name = "rbracket";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}
			else if(c == '(')
			{
				string value = "";
				value += c;

				token.name = "lparenth";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}
			else if(c == ')')
			{
				string value = "";
				value += c;

				token.name = "rparenth";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}
			else if(c == ';')
			{
				string value = "";
				value += c;

				token.name = "semicolon";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}
			else if(c == ':')
			{
				string value = "";
				value += c;

				token.name = "colon";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}

			else if(c == '.')
			{
				string value = "";
				value += c;

				token.name = "dot";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}
			else if(c == '?')
			{
				string value = "";
				value += c;

				token.name = "question";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}
			else if(c == '\\')
			{
				string value = "";
				value += c;

				token.name = "escape";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}
			else if(c == '&')
			{
				string value = "";
				value += c;
				
				c = this->text[++this->pointer];
				if(c == '&')
				{
					value += c;

					token.name = "dand";
					token.value = value;
					
					this->pointer++;	
				}
				else
				{
					token.name = "and";
					token.value = value;
				}
				this->Token.push_back(token);
			}
			else if(c == '|')
			{
				string value = "";
				value += c;
				
				c = this->text[++this->pointer];
				if(c == '|')
				{
					value += c;

					token.name = "dor";
					token.value = value;
					
					this->pointer++;	
				}
				else
				{
					token.name = "or";
					token.value = value;
				}
				this->Token.push_back(token);
			}
			else if(c == ',')
			{
				string value = "";
				value += c;

				token.name = "comma";
				token.value = value;

				this->Token.push_back(token);
				this->pointer++;
			}

			else
			{
				error = "";
				char c = this->text[this->pointer];
				for(int i = 0; i + this->pointer < this->text.length() && i < 10; c = this->text[++i + this->pointer])
				{
					error += c;
				}
				return false;
			}
		}	
		
		return true;
	}

	bool isId(string& value)
	{
		value = this->text[this->pointer];
		this->pointer++;
		char c = this->text[this->pointer]; 
		for(;(c >= 'a'&& c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; c = this->text[++this->pointer])
		{
			value += c;	
		}	
		
		return true;
	}

	bool isKey(string value)
	{
		for(int i = 0; i < 32; i++)
		{
			if(this->Key[i] == value)
			{
				return true;
			}
		}
		return false;
	}

	bool isMacro(string value)
	{
		for(int i = 0; i < 5; i++)
		{
			if(this->Macro[i] == value)
			{
				return true;
			}
		}
		return false;
	}



	string getNumber()
	{
		string number = "";
		char c = this->text[this->pointer];
		for(; c >= '0' && c <= '9'; c = this->text[++this->pointer])
		{
			number += c;
		}

		return number;
	}

public:
	LexicalAnalysis(string text)
	{
		string error;
		this->text = text;
		if(!Analyse(error))
		{
			cout<<"error: "<<error<<endl;
			for(int i = 0; i < this->pointer; i++)
			{
				cout << text[i];
			}
			cout << endl;
		}
	}

	vector<struct Token> getToken()
	{
		return this->Token;
	}

	void PrintToken()
	{
		for(int i = 0; i < this->Token.size(); i++)
		{
			cout<<"{ "<<this->Token[i].name<<" : "<<this->Token[i].value<<" }"<<endl;
		}
	}

	void WriteToken(string name)
	{
		ofstream out(name);	
		for(int i = 0; i < this->Token.size(); i++)
		{
			out << "{ " << this->Token[i].name << " : " << this->Token[i].value << " }" <<endl;
		}	
		out.close();		
	}

};

int main()
{
	ifstream in;
	stringstream buf;
	in.open("LexicalAnalysis.cpp");
	
	buf << in.rdbuf();

	string data = buf.str();

	in.close();
	
	//data = "string Key[32] = {\"auto\", \"short\", \"int\", \"long\", \"double\", \"char\", \"struct\", \"union\", \"enum\", \"typedef\", \"const\", \"unsigned\", \"signed\", \"extern\", \"register\", \"static\", \"volatile\", \"void\", \"if\", \"else\", \"switch\", \"case\", \"for\", \"do\", \"while\", \"goto\", \"continue\", \"break\", \"default\", \"sizeof\", \"return\"};";	
	LexicalAnalysis l(data);
	l.PrintToken();
	l.WriteToken("token");
}

