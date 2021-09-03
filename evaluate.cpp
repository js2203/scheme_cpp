#include <iostream>
#include "evaluate.h"
#include <tuple>

namespace scm::trampoline {

/**
 *
 * @return
 */
std::tuple<Environment*, Object*> popEnvObj() {
  Environment* env{popArg<Environment*>()};
  Object* obj{popArg<Object*>()};
  return  {env, obj};
}
/**
 *
 * @return
 */
std::tuple<Environment*, Object*, Object*> popEnvObjObj() {
  Environment* env{popArg<Environment*>()};
  Object* obj{popArg<Object*>()};
  Object* secondObj{popArg<Object*>()};
  return  {env, obj, secondObj};
}

/**
 *
 * @param env
 * @param expression
 * @return
 */
Object* evaluateInput(Environment& env, Object* expression) {
  pushArgs({&env, expression});
  return trampoline((Continuation*)(trampolineEvaluateFirst));
}

/**
 *
 * @return
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
 *
 * @return
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
 *
 * @return
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
 *
 * @return
 */
static Continuation* evaluateUserDefinedFunction() {
  auto [env, function] = popEnvObj();
  int nArgs{popArg<int>()};

  Object* functionArguments{getUserFunctionArgList(function)};
  Object* functionBody{getUserFunctionBodyList(function)};
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
    return trampolineCall((Continuation *)(syntaxBegin), nullptr, {funcEnv, functionBody});
  }
  else {
    return trampolineCall((Continuation *)(trampolineEvaluateFirst), nullptr, {funcEnv, functionBody});
  }
}

/**
 *
 * @return
 */
static Continuation* evaluateSyntax() {
  auto [env, syntax, arguments] = popEnvObjObj();

  pushArgs({env, arguments});
  return getBuiltinSyntax(syntax);
}

/**
 *
 * @return
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
 *
 * @return
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
      std::cout << ("evaluation not yet implemented for " + scm::toString(obj));
  }
  return nullptr;
}

} // namespace scm