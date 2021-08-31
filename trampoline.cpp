#include "trampoline.h"
#include <stack>

namespace scm::trampoline {

std::stack<ArgumentTypeVariant> argumentStack;

std::stack<Continuation*> functionStack;

Object* lastReturnValue = SCM_NIL;

/**
 *
 * @param startFunction
 * @return
 */
Object* trampoline(Continuation* startFunction) {
  Continuation* nextFunction{startFunction};
  pushFunc(nullptr);
  while (nextFunction != nullptr) {
    nextFunction = (Continuation*)(*nextFunction)();
  }
  return lastReturnValue;
}

/**
 *
 * @param nextFunc
 * @param nextPart
 * @param arguments
 * @return
 */
Continuation* trampolineCall(Continuation* nextFunc, Continuation* nextPart, std::vector<ArgumentTypeVariant> arguments)
{
  pushArgs(std::move(arguments));
  if (nextPart != nullptr) {
    pushFunc(nextPart);
  }
  return nextFunc;
}

/**
 *
 * @param arg
 */
void pushArg(ArgumentTypeVariant arg) {
  argumentStack.push(arg);
}

/**
 *
 * @param arguments
 */
void pushArgs(std::vector<ArgumentTypeVariant> arguments) {
  for (auto i = arguments.rbegin(); i != arguments.rend(); i++) {
    pushArg(*i);
  }
}

/**
 *
 * @param nextFunc
 */
void pushFunc(Continuation* nextFunc) {
  functionStack.push(nextFunc);
}

/**
 *
 * @return
 */
Continuation* popFunc() {
  if (!functionStack.empty()) {
    Continuation* nextFunc{functionStack.top()};
    functionStack.pop();
    return nextFunc;
  }
  return nullptr;
}

} // namespace scm::trampoline
