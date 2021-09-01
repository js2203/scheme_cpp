#include "scheme_basics.h"
#include "scheme_getter.h"
#include <iostream>

namespace scm {

/**
 *
 * @param scmObj
 * @return
 */
ObjectTag getTag(Object *scmObj) {
  return scmObj->tag;
}

/**
 *
 * @param stringObj
 * @return
 */
std::string getStringValue(Object *stringObj) {
  return std::get<std::string>(stringObj->value);
}

/**
 *
 * @param intObj
 * @return
 */
int getIntValue(Object *intObj) {
  return std::get<int>(intObj->value);
}

/**
 *
 * @param floatObj
 * @return
 */
double getFloatValue(Object *floatObj) {
  return std::get<double>(floatObj->value);
}

/**
 *
 * @param consObj
 * @return
 */
Cons getCons(Object *consObj) {
  return std::get<Cons>(consObj->value);
}

/**
 *
 * @param carObj
 * @return
 */
Object *getCar(Object *carObj) {
  auto cons{getCons(carObj)};
  return cons.car;
}

/**
 *
 * @param cdrObj
 * @return
 */
Object *getCdr(Object *cdrObj) {
  auto cons{getCons(cdrObj)};
  return cons.cdr;
}

/**
 *
 * @param funcObj
 * @return
 */
FunctionTag getBuiltinFuncTag(Object* funcObj) {
  return std::get<Func>(funcObj->value).tag;
}

/**
 *
 * @param funcObj
 * @return
 */
int getBuiltinFuncNArgs(Object* funcObj) {
  return std::get<Func>(funcObj->value).nArgs;
}

/**
 *
 * @param userFuncObj
 * @return
 */
Environment* getUserFunctionParentEnv(Object* userFuncObj) {
  return std::get<UserFunc>(userFuncObj->value).env;
}

/**
 *
 * @param userFuncObj
 * @return
 */
Object* getUserFunctionBodyList(Object* userFuncObj)
{
  return std::get<UserFunc>(userFuncObj->value).bodyList;
}

/**
 *
 * @param userFuncObj
 * @return
 */
Object* getUserFunctionArgList(Object* userFuncObj)
{
  return std::get<UserFunc>(userFuncObj->value).argList;
}

/**
 *
 * @param function
 * @return
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
 *
 * @param syntax
 * @return
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