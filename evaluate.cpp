#include <iostream>
#include "evaluate.h"
#include <tuple>

namespace scm::trampoline {

/**
 * pops the first environment and the first object on the current stack
 * @return a tuple with an environment and a scheme object
 */
std::tuple<Environment*, Object*> popEnvObj() {
  Environment* env{popArg<Environment*>()};
  Object* obj{popArg<Object*>()};
  return  {env, obj};
}
/**
 * pops the first environment and the first two objects on the current stack
 * @return a tuple with an environment and two scheme object
 */
std::tuple<Environment*, Object*, Object*> popEnvObjObj() {
  Environment* env{popArg<Environment*>()};
  Object* obj{popArg<Object*>()};
  Object* secondObj{popArg<Object*>()};
  return  {env, obj, secondObj};
}

/**
 * starting function for the trampoline
 * @param env the top level environment
 * @param code the code to be evaluated
 * @return
 */
Object* evaluateInput(Environment& env, Object* code) {
  pushArgs({&env, code});
  return trampoline((Continuation*)(trampolineEvaluateFirst));
}

/**
 * retrieves arguments from the stack and evaluates them until all arguments are evaluated
 * @return the next function for the trampoline
 */
static Continuation* evaluateArgumentsSecond() {
  auto [env, operation, argumentCons] = popEnvObjObj();

  int nArgs = popArg<int>();

  pushArg(lastReturnValue);

  argumentCons = getCdr(argumentCons);
  if (argumentCons != SCM_NIL) {
    Object* nextArg{getCar(argumentCons)};
    pushArgs({env, operation, argumentCons, ++nArgs});
    return trampolineCall((Continuation*)(trampolineEvaluateFirst), (Continuation*)(evaluateArgumentsSecond), {env, nextArg});
  }
  else {
    pushArgs({env, operation, nArgs});
    return popFunc();
  }
}

/**
 * retrieves parameters from the stack and evaluates a con object passed to a function
 * @return the next function for the trampoline
 */
static Continuation* evaluateArgumentsFirst() {
  auto [env, operation, argumentCons] = popEnvObjObj();

  int nArgs{0};

  if (argumentCons != SCM_NIL) {
    pushArgs({env, operation, argumentCons, ++nArgs});
    Object* currentArgument{getCar(argumentCons)};
    return trampolineCall((Continuation*)(trampolineEvaluateFirst), (Continuation*)(evaluateArgumentsSecond), {env, currentArgument});
  }
  else {
    pushArgs({env, operation, nArgs});
    return popFunc();
  }
}

/**
 * retrieves arguments from the stack and evaluates a built in function
 * @return the next function for the trampoline
 */
static Continuation* evaluateBuiltinFunction() {
  auto [env, function] = popEnvObj();
  int nArgs{popArg<int>()};

  if (nArgs != getBuiltinFuncNArgs(function) && getBuiltinFuncNArgs(function) != -1) {
    throw schemeException("incorrect amount of arguments", __FILE__, __LINE__);
  }

  pushArg(nArgs);
  return getBuiltinFunc(function);
}

/**
 * retrieves arguments from the stack and evaluates a user defined function
 * @return the next function for the trampoline
 */
static Continuation* evaluateUserDefinedFunction() {
  auto [env, function] = popEnvObj();
  int nArgs{popArg<int>()};

  Object* functionArguments{getUserFunctionArgs(function)};
  Object* functionBody{getUserFunctionBody(function)};
  Environment* funcEnv{new Environment(getUserFunctionParentEnv(function))};

  if (nArgs == 0 && functionArguments != SCM_NIL) {
    throw schemeException("incorrect amount of arguments for function", __FILE__, __LINE__);
  }

  if (nArgs > 0) {
    ObjectVec evaluatedArguments{popArgs<Object*>(nArgs)};

    for(;;) {
      if (functionArguments == SCM_NIL) {
        break;
      }
      else if (nArgs == 0) {
        throw schemeException("incorrect amount of arguments for function", __FILE__, __LINE__);
      }

      Object* argName{getCar(functionArguments)};
      Object* argValue{evaluatedArguments[--nArgs]};
      scm::defineKey(*funcEnv, argName, argValue);
      functionArguments = getCdr(functionArguments);
    }
  }

  if (hasTag(getCar(functionBody), TAG_CONS)) {
    return trampolineCall((Continuation*)(syntaxBegin), nullptr, {funcEnv, functionBody});
  }
  else {
    return trampolineCall((Continuation*)(trampolineEvaluateFirst), nullptr, {funcEnv, functionBody});
  }
}

/**
 * retrieves arguments from the stack and evaluates a syntax
 * @return the next function for the trampoline
 */
static Continuation* evaluateSyntax() {
  auto [env, syntax, arguments] = popEnvObjObj();

  pushArgs({env, arguments});
  return getBuiltinSyntax(syntax);
}

/**
 * retrieves arguments from the stack and starts the evaluation of a function or syntax
 * @return the next function for the trampoline
 */
static Continuation* trampolineEvaluateSecond() {
  auto [env, obj] = popEnvObj();

  Object* evaluatedOperation{lastReturnValue};
  Object* argumentCons{getCdr(obj)};

  switch (evaluatedOperation->tag) {
    case TAG_FUNC_BUILTIN:
      return trampolineCall((Continuation*)(evaluateArgumentsFirst), (Continuation*)(evaluateBuiltinFunction), {env, evaluatedOperation, argumentCons});
    case TAG_SYNTAX:
      return trampolineCall((Continuation*)(evaluateSyntax), nullptr, {env, evaluatedOperation, argumentCons});
    case TAG_FUNC_USER:
      return trampolineCall((Continuation*)(evaluateArgumentsFirst), (Continuation*)(evaluateUserDefinedFunction),{env, evaluatedOperation, argumentCons});
    default:
      lastReturnValue = evaluatedOperation;
      return popFunc();
  }
}

/**
 * retrieves arguments from the stack and evaluates an object
 * @return the next function for the trampoline
 */
Continuation* trampolineEvaluateFirst() {
  auto [env, obj] = popEnvObj();

  scm::Object* evaluatedObj;
  switch (obj->tag) {
    case TAG_INT:
    case TAG_FLOAT:
    case TAG_STRING:
    case TAG_NIL:
    case TAG_TRUE:
    case TAG_FALSE:
    case TAG_FUNC_BUILTIN:
    case TAG_FUNC_USER:
    case TAG_SYNTAX:
    case TAG_VOID:
    case TAG_EOF:
      lastReturnValue = obj;
      return popFunc();
    case TAG_SYMBOL: {
      evaluatedObj = getVariable(*env, obj);
      if (!evaluatedObj) {
        throw schemeException("no symbol was found", __FILE__, __LINE__);
      }
      lastReturnValue = evaluatedObj;
      return popFunc();
    }
    case scm::TAG_CONS: {
      Object* operation{getCar(obj)};
      pushArgs({env, obj});
      return trampolineCall((Continuation*)(trampolineEvaluateFirst), (Continuation*)(trampolineEvaluateSecond), {env, operation});
    }
    default:
      throw schemeException("evaluation not yet implemented", __FILE__, __LINE__);
  }
  return nullptr;
}

} // namespace scm