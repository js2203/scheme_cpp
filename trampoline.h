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
 *
 * @tparam T
 * @return
 */
template <typename T>
T popArg() {
  if (argumentStack.empty()) {
    throw schemeException("stack is empty", __FILE__, __LINE__);
  }
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
std::vector<T> popArgs(int n) {
  if (argumentStack.size() < n) {
    throw schemeException("stack has to few arguments", __FILE__, __LINE__);
  }
  std::vector<T> values;
  for (int i{0}; i < n; i++) {
    values.push_back(popArg<T>());
  }
  return values;
}

} // namespace scm::trampoline
