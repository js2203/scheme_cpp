#include "init.h"
#include "memory.h"

namespace scm {

/**
 *
 * @param env
 * @param name
 * @param nArgs
 * @param tag
 */
void defineNewSyntax(Environment& env, const std::string& name, int nArgs, FunctionTag tag){
  Object* func{newSyntax(name, nArgs, tag)};
  scm::defineKeyString(env, name, func);
}

/**
 *
 * @param env
 * @param name
 * @param nArgs
 * @param tag
 */
void defineNewBuiltinFunction(Environment& env, const std::string& name, int nArgs, FunctionTag tag){
  Object* func{newBuiltinFunction(name, nArgs, tag)};
  scm::defineKeyString(env, name, func);
}

void initBaseEnvironment(Environment& env) {

  defineNewSyntax(env, "quote", -1, SYNTAX_QUOTE);
  defineNewSyntax(env, "if", -1, SYNTAX_IF);
  defineNewSyntax(env, "define", -1, SYNTAX_DEFINE);
  defineNewSyntax(env, "set!", -1, SYNTAX_SET);
  defineNewSyntax(env, "lambda", -1, SYNTAX_LAMBDA);
  defineNewSyntax(env, "begin", -1, SYNTAX_BEGIN);

  defineNewBuiltinFunction(env, "+", -1, FUNC_ADD);
  defineNewBuiltinFunction(env, "-", -1, FUNC_SUB);
  defineNewBuiltinFunction(env, "*", -1, FUNC_MUL);
  defineNewBuiltinFunction(env, "/", -1, FUNC_DIV);
  defineNewBuiltinFunction(env, "eq?", 2, FUNC_EQ);
  defineNewBuiltinFunction(env, "equal-string?", 2, FUNC_EQUAL_STRING);
  defineNewBuiltinFunction(env, "=", 2, FUNC_EQUAL_NUMBER);
  defineNewBuiltinFunction(env, ">", 2, FUNC_GT);
  defineNewBuiltinFunction(env, "<", 2, FUNC_LT);
  defineNewBuiltinFunction(env, "cons", 2, FUNC_CONS);
  defineNewBuiltinFunction(env, "car", 1, FUNC_CAR);
  defineNewBuiltinFunction(env, "cdr", 1, FUNC_CDR);
  defineNewBuiltinFunction(env, "list", -1, FUNC_LIST);
  defineNewBuiltinFunction(env, "display", -1, FUNC_DISPLAY);
  defineNewBuiltinFunction(env, "function-body", 1, FUNC_FUNCTION_BODY);
  defineNewBuiltinFunction(env, "string?", 1, FUNC_IS_STRING);
  defineNewBuiltinFunction(env, "number?", 1, FUNC_IS_NUMBER);
  defineNewBuiltinFunction(env, "cons?", 1, FUNC_IS_CONS);
  defineNewBuiltinFunction(env, "function?", 1, FUNC_IS_FUNC);
  defineNewBuiltinFunction(env, "user-function?", 1, FUNC_IS_USERFUNC);
  defineNewBuiltinFunction(env, "bool?", 1, FUNC_IS_BOOL);

}

} // namespace scm
