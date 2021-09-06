#pragma once
#include <stack>
#include <variant>
#include "scheme_basics.h"
#include "scheme_getter.h"
#include "environment.h"
#include "memory.h"

namespace scm::trampoline {

using ArgumentTypeVariant = std::variant<Object*, Environment*, Continuation*, std::size_t, int>;

extern Object* lastReturnValue;
extern std::stack<ArgumentTypeVariant> argumentStack;

Object* trampoline(Continuation* startFunction);
Continuation* trampolineCall(Continuation* nextFunc, Continuation* nextPart = nullptr, std::vector<ArgumentTypeVariant> arguments = {});

void pushArg(ArgumentTypeVariant arg);
void pushArgs(std::vector<ArgumentTypeVariant> arguments);
void pushFunc(Continuation* nextFunc);

Continuation* popFunc();

/**
 * pops and returns the top argument on the argument stack
 * @tparam Type the Type of argument to pop
 * @return the argument
 */
template <typename Type>
Type popArg() {
  if (argumentStack.empty()) {
    throw schemeException("stack is empty", __FILE__, __LINE__);
  }
  Type arg{std::get<Type>(argumentStack.top())};
  argumentStack.pop();
  return arg;
}

/**
 * pops and returns the top N amount of arguments on the argument stack
 * @tparam Type the Type of argument to pop
 * @param n the number of arguments to pop
 * @return the arguments
 */
template <typename Type>
std::vector<Type> popArgs(int n) {
  if (argumentStack.size() < n) {
    throw schemeException("stack has to few arguments", __FILE__, __LINE__);
  }
  std::vector<Type> values;
  for (int i{0}; i < n; i++) {
    values.push_back(popArg<Type>());
  }
  return values;
}

} // namespace scm::trampoline
