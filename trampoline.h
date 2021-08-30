#pragma once
#include <stack>
#include <variant>
#include "scheme_functions.h"
#include "scheme_getter.h"
#include "environment.h"
#include "memory.h"

namespace scm::trampoline {

using ArgumentTypeVariant = std::variant<Object*, Environment*, Continuation*, std::size_t, int>;

extern Object* lastReturnValue;
extern std::stack<ArgumentTypeVariant> argumentStack;

Object* trampoline(Continuation* startFunction);
Continuation* trampolineCall(Continuation* nextFunc, Continuation* nextPart = NULL, std::vector<ArgumentTypeVariant> arguments = {});

// push functions
void pushArg(ArgumentTypeVariant arg);
void pushArgs(std::vector<ArgumentTypeVariant> arguments);
void pushFunc(Continuation* nextFunc);

// pop functions
Continuation* popFunc();

/**
 *
 * @tparam T
 * @return
 */
template <typename T>
T popArg()
{
  T arg{std::get<T>(argumentStack.top())};
  argumentStack.pop();
  return arg;
}

/**
 *
 * @tparam T
 * @param n
 * @return
 */
template <typename T>
std::vector<T> popArgs(int n)
{
  std::vector<T> values;
  for (int i{0}; i < n; i++) {
    values.push_back(popArg<T>());
  }
  return values;
}

} // namespace scm::trampoline
