#include "scheme_builtInFunc.h"

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
  Object* minuendObj = popArg<Object*>();
  double minuend = hasTag(minuendObj, TAG_FLOAT) ? getFloatValue(minuendObj)
                                                 : static_cast<double>(getIntValue(minuendObj));

  if (subtrahends.size() == 0) {
    if (hasTag(minuendObj, TAG_FLOAT))
      t_RETURN(newFloat(-getFloatValue(minuendObj)));
    t_RETURN(newInteger(-getIntValue(minuendObj)));
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
    t_RETURN(newFloat(minuend - doubleSubtrahend));
  }
  else {
    auto lambda = [](int a, Object* b) { return a + getIntValue(b); };
    intSubtrahend = std::accumulate(subtrahends.begin(), subtrahends.end(), int(0), lambda);
    t_RETURN(newInteger(static_cast<int>(minuend) - intSubtrahend));
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
      int result = getIntValue(b) * a;
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
Continuation* division()
{
  int nArgs{popArg<int>()};
  return trampolineCall((Continuation*)(multiplication), (Continuation*)(division_second), {nArgs - 1});
}

/**
 *
 * @return
 */
Continuation* equal() {
  int nArgs{popArg<int>()};
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
  int nArgs{popArg<int>()};
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
  int nArgs{popArg<int>()};
  Object* b{popArg<Object*>()};
  Object* a{popArg<Object*>()};

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
  int nArgs{popArg<int>()};
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
  int nArgs{popArg<int>()};
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

Continuation* consFunction() {
  int nArgs{popArg<int>()};
  Object* cdr{popArg<Object*>()};
  Object* car{popArg<Object*>()};
  lastReturnValue = newCons(car, cdr);
  return popFunc();
}

//TODO

} // namespace scm::trampoline
