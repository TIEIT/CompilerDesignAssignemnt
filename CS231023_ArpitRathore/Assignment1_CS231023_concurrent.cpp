#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <thread>
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
    cout << "Usage: ./concurrent <filename>" << endl;
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

  int mid = content.length() / 2;

  while (mid < content.length() && content[mid] != '\n')
  {
    mid++;
  }

  string part1 = content.substr(0, mid);
  string part2 = content.substr(mid);

  vector<Token> tokens1, tokens2;

  thread t1([&]()
            { tokens1 = tokenize(part1); });

  thread t2([&]()
            { tokens2 = tokenize(part2); });

  t1.join();
  t2.join();

  vector<Token> finalTokens;
  finalTokens.insert(finalTokens.end(), tokens1.begin(), tokens1.end());
  finalTokens.insert(finalTokens.end(), tokens2.begin(), tokens2.end());

  cout << "Total Tokens: " << finalTokens.size() << endl;

  return 0;
}