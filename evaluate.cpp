#pragma once
#include <iostream>
#include "evaluate.h"
#include "environment.h"

namespace scm::trampoline {

/**
 *
 * @param env
 * @param expression
 * @return
 */
Object* evaluateInput(Environment& env, Object* expression) {
  pushArgs({&env, expression});
  return trampoline((Continuation *) (trampolineEvaluateFirst));
}

/**
 *
 * @return
 */
static Continuation* evaluateArgumentsSecond()
{
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
    // arguments for evaluateArgumentsSecond
    pushArgs({env, operation, argumentCons, ++nArgs});
    // call evaluation for next argument
    return trampolineCall((Continuation *) (trampolineEvaluateFirst),
                          (Continuation *) (evaluateArgumentsSecond),
                          {env, nextArg});
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
static Continuation* evaluateArgumentsFirst()
{
  auto* env{popArg<Environment*>()};
  auto* operation{popArg<Object*>()};
  auto* argumentCons{popArg<Object*>()};

  // keep track of how many arguments were evaluated
  int nArgs{0};

  if (argumentCons != SCM_NIL) {
    // push arguments for evaluateArgunents_repeat
    pushArgs({env, operation, argumentCons, ++nArgs});
    Object* currentArgument{getCar(argumentCons)};
    return trampolineCall((Continuation *) (trampolineEvaluateFirst),
                          (Continuation *) (evaluateArgumentsSecond),
                          {env, currentArgument});
  }
  else {
    pushArgs({env, operation, nArgs});
    return popFunc();
  }
}

/**
 *
 * @return
 */
static Continuation* evaluateBuiltinFunction()
{
  Environment* env{popArg<Environment*>()};
  Object* function{popArg<Object*>()};
  int nArgs{popArg<int>()};

  pushArg(nArgs);
  return getBuiltinFunc(function);
}

/**
 *
 * @return
 */
static Continuation* evaluateUserDefinedFunction()
{
  Environment* env{popArg<Environment*>()};
  Object* function{popArg<Object*>()};
  int nArgs{popArg<int>()};

  // get arguments and list of expressions
  Object* functionArguments{getUserFunctionArgList(function)};
  Object* functionBody{getUserFunctionBodyList(function)};
  Environment* funcEnv{new Environment(getUserFunctionParentEnv(function))};

  if (nArgs > 0) {
    ObjectVec evaluatedArguments{popArgs<Object*>(nArgs)};

    // define all function arguments in the function environment
    for(;;) {
      if (functionArguments == SCM_NIL) {
        break;
      }

      Object* argName{getCar(functionArguments)};
      Object* argValue{evaluatedArguments[--nArgs]};
      scm::defineKey(*funcEnv, argName, argValue);
      functionArguments = getCdr(functionArguments);
    }
  }

  // body may be a single expression or multiple!
  if (hasTag(getCar(functionBody), TAG_CONS)) {
    return tCall((Continuation *)(beginSyntax), {funcEnv, functionBody}); //TODO
  }
  else {
    return trampolineCall((Continuation *)(trampolineEvaluateFirst), nullptr, {funcEnv, functionBody});
  }
}

/**
 *
 * @return
 */
static Continuation* evaluateSyntax()
{
  Environment* env{popArg<Environment*>()};
  Object* syntax{popArg<Object*>()};
  Object* arguments{popArg<Object*>()};

  pushArgs({env, arguments});
  return getBuiltinSyntax(syntax);
}

/**
 *
 * @return
 */
static Continuation* trampolineEvaluateSecond()
{
  // get arguments from stack
  Environment* env{popArg<Environment*>()};
  Object* obj{popArg<Object*>()};

  // get previously evaluated operation
  Object* evaluatedOperation{lastReturnValue};
  Object* argumentCons{getCdr(obj)};

  switch (evaluatedOperation->tag) {
    case TAG_FUNC_BUILTIN:
      // trampolineEvaluateFirst arguments first then continue with function evaluation
      return trampolineCall((Continuation *) (evaluateArgumentsFirst),
                            (Continuation *) (evaluateBuiltinFunction),
                            {env, evaluatedOperation, argumentCons});
    case TAG_SYNTAX:
      return trampolineCall((Continuation *) (evaluateSyntax), NULL, {env, evaluatedOperation, argumentCons});
    case TAG_FUNC_USER:
      // trampolineEvaluateFirst arguments first then continue with function evaluation
      return trampolineCall((Continuation *) (evaluateArgumentsFirst),
                            (Continuation *) (evaluateUserDefinedFunction),
                            {env, evaluatedOperation, argumentCons});
    default:
      lastReturnValue = evaluatedOperation;
      return popFunc();
  }
}

/**
 *
 * @return
 */
Continuation* trampolineEvaluateFirst() {
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
      return trampolineCall((Continuation *) (trampolineEvaluateFirst),
                            (Continuation *) (trampolineEvaluateSecond),
                            {env, operation});
    }
    default:
      std::cout << ("evaluation not yet implemented for " + scm::toString(obj));
  }
}



} // namespace scm