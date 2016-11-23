#include <iostream>
#include <string>
#include <functional>
#include <sstream>
#include <utility>
#include <regex>

template <typename T>
using UnaryFunction = std::function<T(T)>;

template <typename T>
using BinaryFunction = std::function<T(T,T)>;

template <typename T> T to_num(const std::string &str);
template <> int to_num(const std::string &str){return stoi(str);}
template <> long to_num(const std::string &str){return stol(str);}
template <> long long to_num(const std::string &str){return stoll(str);}
template <> unsigned long to_num(const std::string &str){return stoul(str);}
template <> unsigned long long to_num(const std::string &str){return stoull(str);}
template <> float to_num(const std::string &str){return stof(str);}
template <> double to_num(const std::string &str){return stod(str);}
template <> long double to_num(const std::string &str){return stold(str);}

class Token
{
public:
  virtual std::string to_string(){return "Token";}
};

class Identifier : public Token
{
public:
  Identifier(std::string &&str) : Id(str) {}
  virtual std::string to_string(){return "Identifier";}
  std::string Id;
};

template <typename T>
class Number: public Token
{
public:
  Number(const std::string &str)
  {
    number = to_num<T>(str);
  }
  virtual std::string to_string(){return "Number";}
  T number;
};

class Operator : public Token
{
public:
  Operator(const std::string &str)
  {opt = str[0];}
  virtual std::string to_string(){return "Operator";}
  char opt;
};

class OpenBracket : public Token
{
  virtual std::string to_string(){return "OpenBracket";}
};

class CloseBracket : public Token
{
  virtual std::string to_string(){return "CloseBracket";}
};


const std::array<std::function<std::unique_ptr<Token>(std::string &&str)>,5> tokenGenerators
{
  [](std::string &&str){return std::unique_ptr<Token>(new Identifier(std::move(str)));},
  [](std::string &&str){return std::unique_ptr<Token>(new Number<double>(str));},
  [](std::string &&str){return std::unique_ptr<Token>(new Operator(str));},
  [](std::string &&str){return std::unique_ptr<Token>(new OpenBracket());},
  [](std::string &&str){return std::unique_ptr<Token>(new CloseBracket());}
};

const std::regex token_regex (
  "[[:space:]]*" //Ignore all whitespace at beginning
  "|"   // or
  "([[:alpha:]]+)" // 1: Identifier
  "|"   // or
  "((?:[0-9]*[.])?[0-9]+)" // 2: Number
  "|"   // or
  "([\\-\\+\\*\\/\\^\\<\\>\\=\\,\\!\\%\\:\\?\\&\\|])" // 3: Operator
  "|"   // or
  "([\\{\\{\\(])" // 4: open bracket
  "|"   // or
  "([\\}\\)\\]])" // 5: Close bracket
);

std::vector<std::unique_ptr<Token>> Tokenize(const std::string &s)
{
  std::vector<std::unique_ptr<Token>> tokens;

  auto words_begin =
    std::sregex_iterator(s.begin(), s.end(), token_regex);
  auto words_end = std::sregex_iterator();
  for (std::sregex_iterator rit = words_begin; rit != words_end; ++rit) {
    for (size_t i = 0; i < rit->size(); i++) {
      if(rit->length(i+1) != 0) {
        std::cout << rit->str(i+1) << std::endl;
        tokens.push_back(tokenGenerators[i](rit->str(i+1)));
        break;
      }
    }
  }
  return tokens;
}

std::vector<std::unique_ptr<Instructions> > parser

int main(int argc, char*argv[])
{
  std::vector<std::unique_ptr<Token> > tokens(Tokenize("abs(1+2)"));
  for(auto &&token: tokens)
  {
    std::cout << token->to_string() << std::endl;
  }
}
