#pragma once
#include <iostream>
#include "evaluate.h"

namespace scm::trampoline {

/**
 *
 * @param env
 * @param expression
 * @return
 */
Object* evaluateInput(Environment& env, Object* expression) {
  pushArgs({&env, expression});
  return trampoline((Continuation*)(evaluate));
}

/**
 *
 * @return
 */
static Continuation* evaluateArguments_repeat()
{
  // get variables from stack
  auto* env = popArg<Environment*>();
  auto* operation = popArg<Object*>();
  auto* argumentCons = popArg<Object*>();

  int nArgs = popArg<int>();

  // get evaluated object and store on stack for later functions
  pushArg(lastReturnValue);

  // "loop" with next argument or return
  argumentCons = getCdr(argumentCons);
  if (argumentCons != SCM_NIL) {
    Object* nextArg{getCar(argumentCons)};
    // arguments for evaluateArguments_repeat
    pushArgs({env, operation, argumentCons, ++nArgs});
    // call evaluation for next argument
    return tCall((Continuation *) (evaluate), (Continuation *) (evaluateArguments_repeat), {env, nextArg});
  }
  else {
    pushArgs({env, operation, nArgs});
    return popFunc();
  }
};

/**
 *
 * @return
 */
static Continuation* evaluateArguments()
{
  // get variables from stack
  auto* env{popArg<Environment*>()};
  auto* operation{popArg<Object*>()};
  auto* argumentCons{popArg<Object*>()};

  // keep track of how many arguments were evaluated
  int nArgs{0};

  if (argumentCons != SCM_NIL) {
    // push arguments for evaluateArgunents_repeat
    pushArgs({env, operation, argumentCons, ++nArgs});
    // push and call evaluate on current argument
    Object* currentArgument{getCar(argumentCons)};
    return tCall((Continuation *) (evaluate), (Continuation *) (evaluateArguments_repeat), {env, currentArgument});
  }
  else {
    pushArgs({env, operation, nArgs});
    return popFunc();
  }
};

/**
 *
 * @return
 */
static Continuation* evaluate_Part1()
{
  // get arguments from stack
  Environment* env{popArg<Environment*>()};
  Object* obj{popArg<Object*>()};

  // get previously evaluated operation
  Object* evaluatedOperation{lastReturnValue};
  Object* argumentCons{getCdr(obj)};

  switch (evaluatedOperation->tag) {
    case TAG_FUNC_BUILTIN:
      // evaluate arguments first then continue with function evaluation
      return tCall((Continuation*)(evaluateArguments), (Continuation*)(evaluateBuiltinFunction), {env, evaluatedOperation, argumentCons}); //TODO
    case TAG_SYNTAX:
      return tCall((Continuation*)(evaluateSyntax), NULL, {env, evaluatedOperation, argumentCons});
    case TAG_FUNC_USER:
      // evaluate arguments first then continue with function evaluation
      return tCall((Continuation*)(evaluateArguments), (Continuation*)(evaluateUserDefinedFunction), {env, evaluatedOperation, argumentCons});
    default:
      lastReturnValue = evaluatedOperation;
      return popFunc();
  }
}

/**
 *
 * @return
 */
Continuation* evaluate() {
  // get current environment and expression from their stacks
  Environment* env{popArg<Environment*>()};
  Object* obj{popArg<Object*>()};

  scm::Object* evaluatedObj;
  switch (obj->tag) {
    case TAG_INT:
    case TAG_FLOAT:
    case TAG_STRING:
    case TAG_NIL:
    case TAG_FALSE:
    case TAG_TRUE:
    case TAG_FUNC_BUILTIN:
    case scm::TAG_EOF:
      lastReturnValue = obj;
      return popFunc();
    case scm::TAG_SYMBOL: {
      evaluatedObj = getVariable(*env, obj);
      lastReturnValue = evaluatedObj;
      return popFunc();
    }
    case scm::TAG_CONS: {
      Object* operation{getCar(obj)};
      pushArgs({env, obj});
      return tCall((Continuation*)(evaluate), (Continuation*)(evaluate_Part1), {env, operation});
    }
    default:
      std::cout << ("evaluation not yet implemented for " + scm::toString(obj));
  }
}



} // namespace scm