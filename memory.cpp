#include "memory.h"

namespace scm {

Object* SCM_NIL{new Object};
Object* SCM_VOID{new Object};
Object* SCM_EOF{new Object};
Object* SCM_TRUE{new Object};
Object* SCM_FALSE{new Object};

/**
 * initializes all constant Objects
 */
void initConstSchemeObjects() {
  SCM_NIL->tag = TAG_NIL;
  SCM_VOID->tag = TAG_VOID;
  SCM_EOF->tag = TAG_EOF;
  SCM_TRUE->tag = TAG_TRUE;
  SCM_FALSE->tag = TAG_FALSE;
}

/**
 * built a new user defined function object
 * @param arguments an object with all arguments
 * @param body an object with expressions to be evaluated
 * @param env the environment of the function
 * @return a pointer to the function object
 */
Object* newUserFunction(Object* arguments, Object* body, Environment& env) {
  Object* userDefFuncObj{new Object};
  userDefFuncObj->tag = TAG_FUNC_USER;
  userDefFuncObj->value = UserFunc{arguments, body, &env};
  return userDefFuncObj;
}

/**
 * built a new built in function object
 * @param arguments an object with all arguments
 * @param body an object with expressions to be evaluated
 * @param env the environment of the function
 * @return a pointer to the function object
 */
Object* newBuiltinFunction(const std::string& name, int numArgs, FunctionTag tag) {
  Object* builtInFuncObj{new Object};
  builtInFuncObj->tag = TAG_FUNC_BUILTIN;
  builtInFuncObj->value = Func{"primitive:" + name, numArgs, tag};
  return builtInFuncObj;
};

/**
 * built a syntax object
 * @param name the name of the syntax
 * @param numArgs the number of arguments required
 * @param tag the tag of the syntax
 * @return a pointer to the syntax object
 */
Object* newSyntax(const std::string& name, int numArgs, FunctionTag tag) {
  Object* synObj{new Object()};
  synObj->tag = TAG_SYNTAX;
  synObj->value = Func{"syntax:" + name, numArgs, tag};
  return synObj;
}

/**
 * built a new integer object
 * @param value the value of the integer
 * @return a pointer to the integer object
 */
Object* newInteger(int value) {
  Object* newInt{new Object};
  newInt->tag = TAG_INT;
  newInt->value = value;
  return newInt;
}

/**
 * built a new float object
 * @param value the value of the float
 * @return a pointer to the float object
 */
Object* newFloat(double value) {
  Object* newFloat{new Object};
  newFloat->tag = TAG_FLOAT;
  newFloat->value = value;
  return newFloat;
}

/**
 * built a new string object
 * @param value the value of the string
 * @return a pointer to the string object
 */
Object* newString(std::string value) {
  Object* newString{new Object};
  newString->tag = TAG_STRING;
  newString->value = value;
  return newString;
}

/**
 * built a new symbol object
 * @param value the value of the symbol
 * @return a pointer to the symbol object
 */
Object* newSymbol(std::string value) {
  Object* newSymbol{new Object};
  newSymbol->tag = TAG_SYMBOL;
  newSymbol->value = value;
  return newSymbol;
}

/**
 * built a new cons object
 * @param car the first element of the cons
 * @param cdr the last element of the cons
 * @return a pointer to the cons object
 */
Object* newCons(Object* car, Object* cdr) {
  Object* newCons{new Object};
  newCons->tag = TAG_CONS;
  newCons->value = Cons{car, cdr};
  return newCons;
}

}
