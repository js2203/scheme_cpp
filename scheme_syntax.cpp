#include "scheme_syntax.h"

namespace scm::trampoline {

/**
 *
 * @return
 */
static Continuation* syntaxDefine_second() {
  Environment* env{popArg<Environment*>()};
  Object* symbol{popArg<Object*>()};
  // get evaluated value of definition
  Object* value{lastReturnValue};

  scm::defineKey(*env, symbol, value);
  lastReturnValue = SCM_VOID;
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* syntaxDefine() {
  Environment* env{popArg<Environment*>()};
  Object* argumentCons{popArg<Object*>()};

  Object *symbol, *value;

  // get key of definition
  symbol = getCar(argumentCons);

  // shorthand lambda definition!
  // if symbol is a cons, treat it like a lambda declaration
  // (define (funcname var1 var2 ...) (body))
  if (hasTag(symbol, TAG_CONS)) {
    // push arguments required for next part
    pushArgs({env, getCar(symbol)});
    // call lambda then continue with next part
    return trampolineCall((Continuation*) (syntaxLambda), (Continuation*)(syntaxDefine_second), {env, newCons(getCdr(symbol), getCdr(argumentCons))});
  }
    // in the case of a single symbol, we just define the variable
  else {
    value = getCdr(argumentCons);
    // push arguments required for next part
    pushArgs({env, symbol});

    // call evaluate then continue with next part
    return trampolineCall((Continuation*)(trampolineEvaluateFirst), (Continuation*)(syntaxDefine_second), {env, getCar(value)});
  }
}

/**
 *
 * @return
 */
static Continuation* syntaxSet_second() {
  Environment* env{popArg<Environment*>()};
  Object* symbol{popArg<Object*>()};
  Object* value{lastReturnValue};

  scm::setKey(*env, symbol, value);
  lastReturnValue = value; //TODO unnecessary?
  return popFunc();
}

/**
 *
 * @return
 */
Continuation* syntaxSet() {
  Environment* env{popArg<Environment*>()};
  Object* argumentCons{popArg<Object*>()};

  Object *symbol, *expression;

  symbol = getCar(argumentCons);
  argumentCons = getCdr(argumentCons);
  expression = getCar(argumentCons);

  // push arguments required for next part
  pushArgs({env, symbol});

  // call evaluate then continue with next part
  return trampolineCall((Continuation*)(trampolineEvaluateFirst), (Continuation*)(syntaxSet_second), {env, expression});
}

/**
 *
 * @return
 */
Continuation* syntaxQuote() {
  Environment* env{popArg<Environment*>()};
  Object* argumentCons{popArg<Object*>()};
  Object* quoted = (hasTag(argumentCons, TAG_CONS)) ? getCar(argumentCons) : argumentCons;
  lastReturnValue = quoted;
  return popFunc();
}

/**
 *
 * @return
 */
static Continuation* syntaxIf_second() {
  Environment* env{popArg<Environment*>()};
  Object* trueExpression{popArg<Object*>()};
  Object* falseExpression{popArg<Object*>()};

  Object* evaluatedCondition{lastReturnValue};

  Object* conditionAsBool;

  switch (evaluatedCondition->tag) {
    case scm::TAG_INT: {
      conditionAsBool = (scm::getIntValue(evaluatedCondition) != 0) ? SCM_TRUE : SCM_FALSE;
      break;
    }
    case scm::TAG_FLOAT: {
      conditionAsBool = (scm::getFloatValue(evaluatedCondition) != 0) ? SCM_TRUE : SCM_FALSE;
      break;
    }
    case scm::TAG_STRING: {
      conditionAsBool =
          (scm::getStringValue(evaluatedCondition) == std::string{}) ? SCM_TRUE : SCM_FALSE;
      break;
    }
    case scm::TAG_TRUE:
    case scm::TAG_FUNC_BUILTIN:
    case scm::TAG_FUNC_USER:
    case scm::TAG_SYNTAX: {
      conditionAsBool = SCM_TRUE;
      break;
    }
    case scm::TAG_NIL:
    case scm::TAG_FALSE: {
      conditionAsBool = SCM_FALSE;
      break;
    }
    default: {
      break;
    }
  }
  Object* expression{(conditionAsBool == SCM_TRUE) ? trueExpression : falseExpression};
  return trampolineCall((Continuation*)(trampolineEvaluateFirst), nullptr, {env, expression});
}

/**
 *
 * @return
 */
Continuation* syntaxIf() {
  Environment* env{popArg<Environment*>()};
  Object* argumentCons{popArg<Object*>()};
  Object *condition, *trueExpression, *falseExpression;

  // get all required Objects
  condition = getCar(argumentCons);
  argumentCons = getCdr(argumentCons);
  trueExpression = getCar(argumentCons);
  argumentCons = getCdr(argumentCons);
  falseExpression = getCar(argumentCons);
  // push arguments required for next part
  pushArgs({env, trueExpression, falseExpression});

  // call evaluate then continue with next part
  return trampolineCall((Continuation*)(trampolineEvaluateFirst), (Continuation*)(syntaxIf_second), {env, condition});
}

/**
 *
 * @return
 */
static Continuation* syntaxBegin_second()
{
  Environment* env{popArg<Environment*>()};
  Object* argumentCons{popArg<Object*>()};

  Object* currentExpression = getCar(argumentCons);
  argumentCons = getCdr(argumentCons);

  // check if we're finished
  if (argumentCons == SCM_NIL) {
    // evaluate last expression in begin block and return
    return trampolineCall((Continuation*)(trampolineEvaluateFirst), nullptr, {env, currentExpression});
  }
  else {
    // push arguments for next part
    pushArgs({env, argumentCons});
    return trampolineCall((Continuation*)(trampolineEvaluateFirst), (Continuation*)(syntaxBegin_second), {env, currentExpression});
  }
}

/**
 *
 * @return
 */
Continuation* syntaxBegin() {
  Environment* env{popArg<Environment*>()};
  Object* argumentCons{popArg<Object*>()};
  // because of this check we need to split the function
  // SCM_NIL is required as check for the end of cons objects
  if (argumentCons == SCM_NIL) {
    lastReturnValue = SCM_VOID;
    return popFunc();
  }
  else {
    return trampolineCall((Continuation*)(syntaxBegin_second), nullptr, {env, argumentCons});
  }
}

/**
 *
 * @return
 */
Continuation* syntaxLambda()
{
  // get arguments from stack
  Environment* env{popArg<Environment*>()};
  Object* argumentCons{popArg<Object*>()};

  // try to get argument list and body list from arguments
  Object* argList = getCar(argumentCons);
  Object* bodyList = getCdr(argumentCons);

  lastReturnValue = newUserFunction(argList, bodyList, *env);
  return popFunc();
}

} // namespace scm::trampoline