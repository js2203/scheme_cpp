#include "scheme_getter.h"
#include <iostream>

namespace scm {

/**
 *
 * @param obj
 * @return
 */
TypeTag getTag(Object *obj) {
  return obj->tag;
}

/**
 *
 * @param obj
 * @return
 */
std::string getStringValue(Object *obj) {
  if (!hasTag(obj, TAG_STRING) && !hasTag(obj, TAG_SYMBOL)) {
    // schemeThrow("object has no string value that could be gotten");
  }
  return std::get<std::string>(obj->value);
}

/**
 *
 * @param obj
 * @return
 */
int getIntValue(Object *obj) {
  if (!hasTag(obj, TAG_INT)) {
    // schemeThrow("object has no integer value that could be gotten");
  }
  return std::get<int>(obj->value);
}

/**
 *
 * @param obj
 * @return
 */
double getFloatValue(Object *obj) {
  if (!hasTag(obj, TAG_FLOAT)) {
    // schemeThrow("object has no float value that could be gotten");
  }
  return std::get<double>(obj->value);
}

/**
 *
 * @param obj
 * @return
 */
Cons getCons(Object *obj) {
  if (!hasTag(obj, TAG_CONS)) {
    // schemeThrow("tried to get consvalue from non-cons object: " + toString(obj));
  }
  return std::get<Cons>(obj->value);
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
 * @param function
 * @return
 */
Continuation* getBuiltinFunc(Object* function) { //TODO
  switch (getBuiltinFuncTag(function)) {
    case FUNC_ADD:
      return trampoline::addition();
    case FUNC_SUB:
      return trampoline::subtraction();
    case FUNC_MULT:
      return trampoline::multiplication();
    case FUNC_DIV:
      return trampoline::division();
    case FUNC_CONS:
      return trampoline::equal();
    case FUNC_CAR:
      return trampoline::equalNumber();
    case FUNC_CDR:
      return trampoline::equalString();
    default:
      return nullptr;
  }
}


} // namespace scm