/**
 * should be done?
 */

#include "parser.h"
#include "memory.h"
#include <regex>

namespace scm {

/**
 *
 * @param arguments
 * @return
 */
bool isValid(const std::vector<std::string>& arguments) {
  long parentheses[2]= { };

  // check for the amount of open and closed parentheses
  for (auto& argument : arguments) {
    if (argument == "(") {
      parentheses[0] += 1;
    } else if (argument == ")") {
      parentheses[1] += 1;
    }
  }

  // if the amount is equal, the line is valid
  if (parentheses[0] == parentheses[1]){
    return true;
  }
  return false;
}

/**
 *
 * @param currentLine
 * @return
 */
std::vector<std::string> splitCurrentLine(std::string currentLine)
{
  std::vector<std::string> elements;

  // regex to check for strings, symbols, numbers and variables
  std::regex Parser(R"(\"[^\"]*\"|[#<>=\-\w\d\.\?\!]+|[\'\+\/\*\%\=\(\)])");
  for (std::sregex_iterator i = std::sregex_iterator(currentLine.begin(), currentLine.end(), Parser);
       i != std::sregex_iterator();
       i++) {
    elements.push_back(std::smatch(*i).str());
  }
  return elements;
}

Object* interpretList(std::vector<std::string>::iterator& current)
{
  Object *car, *cdr;

  // the end of a list is ')'
  if (*current == ")") {
    return SCM_NIL;
  }
  // car = current element, cdr = remaining elements
  car = interpretInput(current);
  cdr = interpretList(++current);
  return newCons(car, cdr);
}

/**
 *
 * @param current
 * @return
 */
Object* interpretInput(std::vector<std::string>::iterator& current) {
  // integer
  if (std::regex_match(*current, std::regex(R"(^\-?[0-9]+$)"))) {
    return newInteger(std::stoi(*current));
  }
    // float
  else if (std::regex_match(*current, std::regex(R"(^\-?[0-9]*\.[0-9]+$)"))) {
    return newFloat(stof(*current));
  }
  // string
  else if (std::regex_match(*current, std::regex(R"(^\".*\"$)"))) {
    return newString((*current).substr(1, (*current).length() - 2));
  }
  // true
  else if (*current == std::string("#t")) {
    return SCM_TRUE;
  }
  // false
  else if (*current == std::string("#f")) {
    return SCM_FALSE;
  }
  // List
  else if (*current == "(") {
    return interpretList(++current);
  }
  // QUOTE
  else if (*current == "'") {
    Object* quoteContents{interpretInput(++current)};
    Object* cdr = (quoteContents == SCM_NIL) ? SCM_NIL : newCons(quoteContents, SCM_NIL);
    return newCons(newSymbol("quote"), cdr);
  }
  // EOF
  else if (*current == "exit!") {
    return SCM_EOF;
  }
  // Symbol
  else if (std::regex_match(*current, std::regex(R"(^[^\s^\d^.^\-^']\S*$|^\-$)"))) {
    return newSymbol(*current);
  }
  else {
    return SCM_EOF;
  }
}

/**
 *
 * @param streamPointer
 * @return
 */
Object* readInput(std::istream *streamPointer) {

  // define a storage for the arguments and the line
  std::vector<std::string> arguments;
  std::string currentLine;

  // read a single argument or until all parenthesis are closed
  do {
    if (!std::getline(*streamPointer, currentLine)) {
      return SCM_EOF;
    }

    // exclude every comment in scheme
    currentLine = currentLine.substr(0, currentLine.find(';'));

    // split the line in every single argument
    std::vector<std::string> currentLineElements = splitCurrentLine(currentLine);

    // save the arguments in a vector
    arguments.insert(arguments.end(),
                     std::make_move_iterator(currentLineElements.begin()),
                     std::make_move_iterator(currentLineElements.end()));

  } while (!isValid(arguments) || arguments.empty());

  // make the arguments iterable and create the correct scheme object
  std::vector<std::string>::iterator iter{arguments.begin()};
  Object* correctObject{interpretInput(iter)};

  return correctObject;
}

} // end namespace
