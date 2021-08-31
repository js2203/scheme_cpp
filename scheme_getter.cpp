#include "scheme_basics.h"
#include "scheme_getter.h"
#include <iostream>

namespace scm {

/**
 *
 * @param obj
 * @return
 */
ObjectTag getTag(Object *obj) {
  return obj->tag;
}

/**
 *
 * @param obj
 * @return
 */
std::string getStringValue(Object *obj) {
  if (hasTag(obj, TAG_STRING) && hasTag(obj, TAG_SYMBOL)) {
    return std::get<std::string>(obj->value);
  }
}

/**
 *
 * @param obj
 * @return
 */
int getIntValue(Object *obj) {
  if (hasTag(obj, TAG_INT)) {
    return std::get<int>(obj->value);
  }
}

/**
 *
 * @param obj
 * @return
 */
double getFloatValue(Object *obj) {
  if (hasTag(obj, TAG_FLOAT)) {
    return std::get<double>(obj->value);
  }
}

/**
 *
 * @param obj
 * @return
 */
Cons getCons(Object *obj) {
  if (hasTag(obj, TAG_CONS)) {
    return std::get<Cons>(obj->value);
  }
}

/**
 *
 * @param obj
 * @return
 */
Object *getCar(Object *obj) {
  auto cons{getCons(obj)};
  return cons.car;
}

/**
 *
 * @param obj
 * @return
 */
Object *getCdr(Object *obj) {
  auto cons{getCons(obj)};
  return cons.cdr;
}

/**
 *
 * @param obj
 * @return
 */
FunctionTag getBuiltinFuncTag(Object* obj) {
  return std::get<Func>(obj->value).tag;
}

/**
 *
 * @param obj
 * @return
 */
Environment* getUserFunctionParentEnv(Object* obj) {
  return std::get<UserFunc>(obj->value).env;
}

/**
 *
 * @param obj
 * @return
 */
Object* getUserFunctionBodyList(Object* obj)
{
  return std::get<UserFunc>(obj->value).bodyList;
}

/**
 *
 * @param obj
 * @return
 */
Object* getUserFunctionArgList(Object* obj)
{
  return std::get<UserFunc>(obj->value).argList;
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
      return trampoline::equal();
    case FUNC_CAR:
      return trampoline::equalNumber();
    case FUNC_CDR:
      return trampoline::equalString();
    case FUNC_EQ:
      return trampoline::greaterThan();
    case FUNC_EQUAL_STRING:
      return trampoline::lesserThan();
    case FUNC_EQUAL_NUMBER:
      return trampoline::buildCons();
    case FUNC_GT:
      return trampoline::getCarFunc();
    case FUNC_LT:
      return trampoline::getCdrFunc();
    case FUNC_DISPLAY:
      return trampoline::buildList();
    case FUNC_LIST:
      return trampoline::display();
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