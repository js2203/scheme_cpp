#include "scheme_builtInFunc.h"
#include <iostream>

namespace scm::trampoline {

/**
 *
 * @return
 */
Continuation* addition() {
  int nArgs{popArg<int>()};

  auto arguments = popArgs<Object*>(nArgs);

  if (std::any_of(arguments.begin(), arguments.end(), isFloat)) {
    auto lambda = [](double a, Object* b) {
      if (hasTag(b, TAG_FLOAT)) {
        return getFloatValue(b) + a;
      }
      else {
        return static_cast<double>(getIntValue(b) + a);
      }
    };
    lastReturnValue = newFloat(std::accumulate(arguments.begin(), arguments.end(), 0, lambda));
    return popFunc();
  }
  else {
    auto lambda = [](int a, Object* b) {
      int result = getIntValue(b) + a;
      return result;
    };
    lastReturnValue = newInteger(std::accumulate(arguments.begin(), arguments.end(), 0, lambda));
    return popFunc();
  }
}

/**
 *
 * @return
 */
Continuation* subtraction() {
  int nArgs{popArg<int>()};
  auto subtrahends = popArgs<Object*>(nArgs - 1);
  int intSubtrahend{};
  double doubleSubtrahend;
  auto* minuendObj = popArg<Object*>();

  double minuend = hasTag(minuendObj, TAG_FLOAT) ? getFloatValue(minuendObj) : static_cast<double>(getIntValue(minuendObj));

  if (subtrahends.empty()) {
    if (hasTag(minuendObj, TAG_FLOAT)) {
      lastReturnValue = newFloat(-getFloatValue(minuendObj));
      return popFunc();
    }
    lastReturnValue = newInteger(-getIntValue(minuendObj));
    return popFunc();
  }
  else if (hasTag(minuendObj, TAG_FLOAT) ||
      std::any_of(subtrahends.begin(), subtrahends.end(), isFloat)) {
    auto lambda = [](double a, Object* b) {
      if (hasTag(b, TAG_INT)) {
        return a + static_cast<double>(getIntValue(b));
      }
      return a + getFloatValue(b);
    };
    doubleSubtrahend = std::accumulate(subtrahends.begin(), subtrahends.end(), double(0.0), lambda);
    lastReturnValue = newFloat(minuend - doubleSubtrahend);
    return popFunc();
  }
  else {
    auto lambda = [](int a, Object* b) { return a + getIntValue(b); };
    intSubtrahend = std::accumulate(subtrahends.begin(), subtrahends.end(), int(0), lambda);
    lastReturnValue = newInteger(static_cast<int>(minuend) - intSubtrahend);
    return popFunc();
  }
}

/**
 *
 * @return
 */
Continuation* multiplication() {
  int nArgs{popArg<int>()};
  ObjectVec arguments{popArgs<Object*>(nArgs)};
  if (std::any_of(arguments.begin(), arguments.end(), isFloat)) {
    auto lambda = [](double a, Object* b) {
      if (hasTag(b, TAG_INT)) {
        return static_cast<double>(getIntValue(b)) * a;
      }
      return a * getFloatValue(b);
    };
    lastReturnValue = newFloat(std::accumulate(arguments.begin(), arguments.end(), double(1), lambda));
    return popFunc();
  }
  else {
    auto lambda = [](int a, Object* b) {
      return a * getIntValue(b);
    };
    lastReturnValue = newInteger(std::accumulate(arguments.begin(), arguments.end(), int{1}, lambda));
    return popFunc();
  }
}

/**
 *
 * @return
 */
Continuation* division_second() {
  Object* divisor{lastReturnValue};
  Object* dividend{popArg<Object*>()};

  if (isFloat(dividend) && isFloat(divisor)) {
    lastReturnValue = newFloat(getFloatValue(dividend) / getFloatValue(divisor));
    return popFunc();
  }
  else if (isFloat(dividend)) {
    lastReturnValue = newFloat(getFloatValue(dividend) / static_cast<double>(getIntValue(divisor)));
    return popFunc();
  }
  else if (isFloat(divisor)) {
    lastReturnValue = newFloat(static_cast<double>(getIntValue(dividend)) / getFloatValue(divisor));
    return popFunc();
  }
  else {
    lastReturnValue = newFloat(static_cast<double>(getIntValue(dividend)) / static_cast<double>(getIntValue(divisor)));
    return popFunc();
  }
}

/**
 *
 * @return
 */
Continuation* division() {
  int nArgs{popArg<int>()};
  return trampolineCall((Continuation*)(multiplication), (Continuation*)(division_second), {nArgs - 1});
}

/**
 *
 * @return
 */
Continuation* equal() {
  popArg<int>();
  Object* b{popArg<Object*>()};
  Object* a{popArg<Object*>()};
  lastReturnValue = (a == b) ? SCM_TRUE : SCM_FALSE;
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* equalString() {
  popArg<int>();
  Object* b{popArg<Object*>()};
  Object* a{popArg<Object*>()};
  lastReturnValue = (scm::getStringValue(a) == scm::getStringValue(b)) ? SCM_TRUE : SCM_FALSE;
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* equalNumber() {
  popArg<int>();
  Object* a{popArg<Object*>()};
  Object* b{popArg<Object*>()};

  if (!isNumber(a) || !isNumber(b)){
    throw schemeException("incorrect type at '='", __FILE__, __LINE__);
  }
  if (isFloat(a) && isFloat(b)) {
    lastReturnValue = (scm::getFloatValue(a) == scm::getFloatValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
  else if (isFloat(a)) {
    lastReturnValue = (scm::getFloatValue(a) == scm::getIntValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
  else if (isFloat(b)) {
    lastReturnValue = (scm::getIntValue(a) == scm::getFloatValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
  else {
    lastReturnValue = (scm::getIntValue(a) == scm::getIntValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
}

/**
 *
 * @return
 */
Continuation* greaterThan() {
  popArg<int>();
  Object* b{popArg<Object*>()};
  Object* a{popArg<Object*>()};

  if (isFloat(a) && isFloat(b)) {
    lastReturnValue = (scm::getFloatValue(a) > scm::getFloatValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
  else if (isFloat(a)) {
    lastReturnValue = (scm::getFloatValue(a) > scm::getIntValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
  else if (isFloat(b)) {
    lastReturnValue = (scm::getIntValue(a) > scm::getFloatValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
  else {
    lastReturnValue = (scm::getIntValue(a) > scm::getIntValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
}

/**
 *
 * @return
 */
Continuation* lesserThan() {
  popArg<int>();
  Object* b{popArg<Object*>()};
  Object* a{popArg<Object*>()};

  if (isFloat(a) && isFloat(b)) {
    lastReturnValue = (scm::getFloatValue(a) < scm::getFloatValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
  else if (isFloat(a)) {
    lastReturnValue = (scm::getFloatValue(a) < scm::getIntValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
  else if (isFloat(b)) {
    lastReturnValue = (scm::getIntValue(a) < scm::getFloatValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
  else {
    lastReturnValue = (scm::getIntValue(a) < scm::getIntValue(b)) ? SCM_TRUE : SCM_FALSE;
    return popFunc();
  }
}

/**
 *
 * @return
 */
Continuation* buildCons() {
  popArg<int>();
  Object* cdr{popArg<Object*>()};
  Object* car{popArg<Object*>()};
  lastReturnValue = newCons(car, cdr);
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* getCarFunc() {
  popArg<int>();
  Object* cons{popArg<Object*>()};
  if (hasTag(cons, TAG_CONS)) {
    lastReturnValue = getCar(cons);
    return popFunc();
  }
  return nullptr;
}

/**
 *
 * @return
 */
Continuation* getCdrFunc() {
  popArg<int>();
  Object* cons{popArg<Object*>()};
  if (hasTag(cons, TAG_CONS)) {
    lastReturnValue = getCdr(cons);
    return popFunc();
  }
  return nullptr;
}

/**
 *
 * @return
 */
Continuation* buildList() {
  int nArgs{popArg<int>()};
  Object* rest = SCM_NIL;
  while (nArgs--) {
    Object* currentArgument{popArg<Object*>()};
    rest = newCons(currentArgument, rest);
  }
  lastReturnValue = rest;
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* display() {
  int nArgs{popArg<int>()};
  ObjectVec arguments{popArgs<Object*>(nArgs)};
  for (auto argument{arguments.rbegin()}; argument != arguments.rend(); argument++) {
    std::cout << toString(*argument) << " ";
  }
  std::cout << '\n';
  lastReturnValue = SCM_VOID;
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* returnFuncBody() {
  popArg<int>();
  Object* obj{popArg<Object*>()};
  if (hasTag(obj, TAG_FUNC_USER)) {
    lastReturnValue = getUserFunctionBodyList(obj);
    return popFunc();
  }
  return nullptr;
}

/**
 *
 * @return
 */
Continuation* returnFuncArguments() {
  popArg<int>();
  Object* obj{popArg<Object*>()};
  if (hasTag(obj, TAG_FUNC_USER)) {
    lastReturnValue = getUserFunctionArgList(obj);
    return popFunc();
  }
  return nullptr;
}

/**
 *
 * @return
 */
Continuation* isStringFunc() {
  popArg<int>();
  Object* obj{popArg<Object*>()};
  lastReturnValue = (isString(obj)) ? SCM_TRUE : SCM_FALSE;
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* isNumberFunc() {
  popArg<int>();
  Object* obj{popArg<Object*>()};
  lastReturnValue = (isNumber(obj)) ? SCM_TRUE : SCM_FALSE;
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* isConsFunc() {
  popArg<int>();
  Object* obj{popArg<Object*>()};
  lastReturnValue = (hasTag(obj, TAG_CONS)) ? SCM_TRUE : SCM_FALSE;
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* isBuiltinFunc() {
  popArg<int>();
  Object* obj{popArg<Object*>()};
  lastReturnValue = (hasTag(obj, TAG_FUNC_BUILTIN)) ? SCM_TRUE : SCM_FALSE;
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* isUserFunc() {
  popArg<int>();
  Object* obj{popArg<Object*>()};
  lastReturnValue = (hasTag(obj, TAG_FUNC_USER)) ? SCM_TRUE : SCM_FALSE;
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* isBoolFunc() {
  popArg<int>();
  Object* obj{popArg<Object*>()};
  lastReturnValue = (scm::isSameType(obj, {TAG_TRUE, TAG_FALSE})) ? SCM_TRUE : SCM_FALSE;
  return popFunc();
}

} // namespace scm::trampoline
