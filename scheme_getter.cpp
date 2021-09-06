#include "scheme_basics.h"
#include "scheme_getter.h"
#include <iostream>

namespace scm {

/**
 * returns the tag of a scheme object
 * @param scmObj the scheme object
 * @return Tag of the scheme Object
 */
ObjectTag getTag(Object *scmObj) {
  return scmObj->tag;
}

/**
 * returns the value of a string scheme object
 * @param stringObj the scheme object
 * @return the string value
 */
std::string getStringValue(Object *stringObj) {
  return std::get<std::string>(stringObj->value);
}

/**
 * returns the value of a integer scheme object
 * @param intObj the scheme object
 * @return the integer value
 */
int getIntValue(Object *intObj) {
  return std::get<int>(intObj->value);
}

/**
 * returns the value of a float scheme object
 * @param floatObj the scheme object
 * @return the float value
 */
double getFloatValue(Object *floatObj) {
  return std::get<double>(floatObj->value);
}

/**
 * returns the value of a cons scheme object
 * @param consObj the scheme object
 * @return the cons value
 */
Cons getCons(Object *consObj) {
  return std::get<Cons>(consObj->value);
}

/**
 * returns the value of a cons scheme object
 * @param carObj the scheme object
 * @return the car value
 */
Object *getCar(Object *carObj) {
  auto cons{getCons(carObj)};
  return cons.car;
}

/**
 * returns the value of a cons scheme object
 * @param cdrObj the scheme object
 * @return the cdr value
 */
Object *getCdr(Object *cdrObj) {
  auto cons{getCons(cdrObj)};
  return cons.cdr;
}

/**
 * returns the Tag of a function scheme object
 * @param funcObj the scheme object
 * @return the function tag
 */
FunctionTag getBuiltinFuncTag(Object* funcObj) {
  return std::get<Func>(funcObj->value).tag;
}

/**
 * returns the amount of arguments a function takes
 * @param funcObj the scheme object
 * @return the amount of arguments
 */
int getBuiltinFuncNArgs(Object* funcObj) {
  return std::get<Func>(funcObj->value).nArgs;
}

/**
 * returns the parent environment of an user defined function
 * @param userFuncObj the scheme object
 * @return the parent environment
 */
Environment* getUserFunctionParentEnv(Object* userFuncObj) {
  return std::get<UserFunc>(userFuncObj->value).env;
}

/**
 * returns the body of an user defined function
 * @param userFuncObj the scheme object
 * @return the body of the function
 */
Object* getUserFunctionBody(Object* userFuncObj)
{
  return std::get<UserFunc>(userFuncObj->value).body;
}

/**
 * returns the arguments of an user defined function
 * @param userFuncObj the scheme object
 * @return the arguments an user defined function takes
 */
Object* getUserFunctionArgs(Object* userFuncObj)
{
  return std::get<UserFunc>(userFuncObj->value).args;
}

/**
 * returns the corresponding function for a builtin function
 * @param function the scheme object
 * @return the corresponding function for a function tag
 */
Continuation* getBuiltinFunc(Object* function) {
  switch (getBuiltinFuncTag(function)) {
    case FUNC_ADD:
      return trampoline::addition();
    case FUNC_SUB:
      return trampoline::subtraction();
    case FUNC_MUL:
      return trampoline::multiplication();
    case FUNC_DIV:
      return trampoline::division();
    case FUNC_CONS:
      return trampoline::buildCons();
    case FUNC_CAR:
      return trampoline::getCarFunc();
    case FUNC_CDR:
      return trampoline::getCdrFunc();
    case FUNC_EQ:
      return trampoline::equal();
    case FUNC_EQUAL_STRING:
      return trampoline::equalString();
    case FUNC_EQUAL_NUMBER:
      return trampoline::equalNumber();
    case FUNC_GT:
      return trampoline::greaterThan();
    case FUNC_LT:
      return trampoline::lesserThan();
    case FUNC_DISPLAY:
      return trampoline::display();
    case FUNC_LIST:
      return trampoline::buildList();
    case FUNC_FUNCTION_BODY:
      return trampoline::returnFuncBody();
    case FUNC_FUNCTION_ARGS:
      return trampoline::returnFuncArguments();
    case FUNC_IS_STRING:
      return trampoline::isStringFunc();
    case FUNC_IS_NUMBER:
      return trampoline::isNumberFunc();
    case FUNC_IS_CONS:
      return trampoline::isConsFunc();
    case FUNC_IS_FUNC:
      return trampoline::isBuiltinFunc();
    case FUNC_IS_USERFUNC:
      return trampoline::isUserFunc();
    case FUNC_IS_BOOL:
      return trampoline::isBoolFunc();
    default:
      return nullptr;
  }
}

/**
 * returns the corresponding syntax for a builtin function
 * @param syntax the scheme object
 * @return the corresponding syntax for a function tag
 */
Continuation* getBuiltinSyntax(Object* syntax) {
  switch (getBuiltinFuncTag(syntax)) {
    case SYNTAX_DEFINE:
      return trampoline::syntaxDefine();
    case SYNTAX_SET:
      return trampoline::syntaxSet();
    case SYNTAX_QUOTE:
      return trampoline::syntaxQuote();
    case SYNTAX_IF:
      return trampoline::syntaxIf();
    case SYNTAX_BEGIN:
      return trampoline::syntaxBegin();
    case SYNTAX_LAMBDA:
      return trampoline::syntaxLambda();
    default:
      return nullptr;
  }
}

} // namespace scm