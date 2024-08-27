import java.util.*;

class Token
{
	String name;
	String value;
	
	public Token(String name, String value)
	{
		this.name = name;
		this.value = value;
	}
}



public class LexicalAnalysis
{
	String text;
	int pointer;
	Vector<Token> TokenList = new Vector<Token>();
	String[] Key = {"begin", "end", "return", "int", "float", "string", "print"};	

	public LexicalAnalysis()
	{
		this.pointer = 0;
	}

	public boolean Analyse()
	{
		int state = 0;
		String value = "";

		for(; this.pointer < this.text.length(); this.pointer++)
		{
			char c = this.text.charAt(this.pointer);
			switch(state)
			{
				case 0:
						if(c == ' ' || c == '\n' || c == '\t')
						{
							state = 0;
							value = "";
						}	
						else if(c >= '0' && c <= '9')
						{
							state = 1;
							value += c;		
						}
						else if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
						{
							state = 5;
							value += c;
						}
						else if(c == '\"')
						{
							state = 7;
						}
						else if(c == '+')
						{
							this.TokenList.addElement(new Token("plus", "+"));
						}
						else if(c == '-')
						{
							this.TokenList.addElement(new Token("minus", "-"));
						}
						else if(c == '*')
						{
							this.TokenList.addElement(new Token("times", "*"));
						}
						else if(c == '/')
						{
							this.TokenList.addElement(new Token("division", "/"));
						}
						else if(c == '=')
						{
							this.TokenList.addElement(new Token("equal", "="));
						}
						else if(c == '(')
						{
							this.TokenList.addElement(new Token("lparenth", "("));
						}
						else if(c == ')')
						{
							this.TokenList.addElement(new Token("rparenth", ")"));
						}
						else if(c == '{')
						{
							this.TokenList.addElement(new Token("lbrace", "{"));
						}
						else if(c == '}')
						{
							this.TokenList.addElement(new Token("rbrace", "}"));
						}
						else if(c == ';')
						{
							this.TokenList.addElement(new Token("semicolon", ";"));
						}
						else if(c == ',')
						{
							this.TokenList.addElement(new Token("comma", ","));
						}
						else
						{
							PrintError();
							return false;
						}


					break;

				case 1:
					if(c > '0' && c <= '9')
					{
						state = 1;
						value += c;
					}
					else if(c == '.')
					{
						state = 2;
						value += c;
					}
					else 
					{
						this.TokenList.addElement(new Token("integer", value));

						state = 0;
						value = "";

						this.pointer--;
					}
					break;

				case 2:
					if(c >= '0' && c <= '9')
					{
						state = 2;
						value += c;
					}	
					else
					{
						this.TokenList.addElement(new Token("float", value));
						
						state = 0;
						value ="";

						this.pointer--;
					}
					break;



				case 5:
					if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_')
					{
						state = 5;
						value += c;
					}
					else
					{
						String name = "id";
						for(String k : this.Key) 
						{
							if(value.equals(k))
							{
								name = "key";
								break;
							}	
						}
							
						this.TokenList.addElement(new Token(name, value));

						state = 0;
						value = "";
						
						this.pointer--;
					}
					break;

				case 7:
					if(c == '\"')
					{
						this.TokenList.addElement(new Token("string", value));

						state = 0;
						value = "";
					}
					else
					{
						state = 7;
						value += c;
					}
			}		
		}

		return true;	
	}

	void LoadText(String text)
	{
		this.text = text;
	}


	private void PrintError()
	{
		System.out.println(this.text.substring(0,this.pointer));
	}
	
	void PrintToken()
	{
		for(Token token : this.TokenList)
		{
			System.out.println("< " + token.name + " : " + token.value + " >");
		}
	}

	Vector<Token> GetToken()
	{
		return this.TokenList;
	}
}
