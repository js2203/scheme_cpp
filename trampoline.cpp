#include "trampoline.h"
#include <stack>
#include <utility>
#include <variant>

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
  pushFunc(NULL);
  while (nextFunction != NULL) {
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
  pushArgs(arguments);
  if (nextPart != NULL) {
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
  for (auto i = arguments.rbegin(); i != arguments.rend(); i++) { //TODO TEST THIS
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
}

} // namespace scm::trampoline
