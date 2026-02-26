#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
using namespace std;

enum TokenType
{
  IDENTIFIER,
  NUMBER,
  OPERATOR
};

struct Token
{
  TokenType type;
  string value;
};

vector<Token> tokenize(const string &input)
{
  vector<Token> tokens;
  int i = 0;

  while (i < input.length())
  {

    if (isspace(input[i]))
    {
      i++;
      continue;
    }

    if (isalpha(input[i]))
    {
      string identifier;
      while (i < input.length() && (isalnum(input[i]) || input[i] == '_'))
      {
        identifier += input[i++];
      }
      tokens.push_back({IDENTIFIER, identifier});
    }

    else if (isdigit(input[i]))
    {
      string number;
      while (i < input.length() && isdigit(input[i]))
      {
        number += input[i++];
      }
      tokens.push_back({NUMBER, number});
    }

    else if (input[i] == '+' || input[i] == '-' || input[i] == '*' ||
             input[i] == '/' || input[i] == '=' || input[i] == ';')
    {

      string op(1, input[i]);
      tokens.push_back({OPERATOR, op});
      i++;
    }

    else
    {
      i++;
    }
  }

  return tokens;
}

int main(int argc, char *argv[])
{

  if (argc < 2)
  {
    cout << "Usage: ./lexer <filename>" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  if (!file)
  {
    cout << "File not found!" << endl;
    return 1;
  }

  string content((istreambuf_iterator<char>(file)),
                 istreambuf_iterator<char>());

  vector<Token> tokens = tokenize(content);

  for (const auto &token : tokens)
  {
    if (token.type == IDENTIFIER)
      cout << "IDENTIFIER: " << token.value << endl;
    else if (token.type == NUMBER)
      cout << "NUMBER: " << token.value << endl;
    else if (token.type == OPERATOR)
      cout << "OPERATOR: " << token.value << endl;
  }

  return 0;
}