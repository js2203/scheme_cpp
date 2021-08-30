#pragma once
#include "environment.h"
#include "scheme_getter.h"
#include <variant>
#include <string>
#include <vector>
#include <stack>

namespace scm {

struct Object;

enum TypeTag {
  TAG_INT = 1,
  TAG_FLOAT,
  TAG_STRING,
  TAG_SYMBOL,
  TAG_CONS,
  TAG_NIL,
  TAG_TRUE,
  TAG_FALSE,
  TAG_FUNC_BUILTIN,
  TAG_FUNC_USER,
  TAG_SYNTAX,
  TAG_VOID,
  TAG_EOF,
};

enum FunctionTag {
  SYNTAX_QUOTE,
  SYNTAX_LAMBDA,
  SYNTAX_DEFINE,
  SYNTAX_IF,
  SYNTAX_SET,
  SYNTAX_BEGIN,
  SYNTAX_HELP,

  FUNC_ADD,
  FUNC_SUB,
  FUNC_MULT,
  FUNC_DIV,
  FUNC_CONS,
  FUNC_CAR,
  FUNC_CDR,
  FUNC_EQ,
  FUNC_EQUAL_NUMBER,
  FUNC_EQUAL_STRING,
  FUNC_GT,
  FUNC_LT,
  FUNC_LIST,
  FUNC_DISPLAY,
  FUNC_FUNCTION_BODY,
  FUNC_FUNCTION_ARGLIST,
  FUNC_IS_STRING,
  FUNC_IS_NUMBER,
  FUNC_IS_CONS,
  FUNC_IS_FUNC,
  FUNC_IS_USERFUNC,
  FUNC_IS_BOOL,
};

struct Cons {
  Object* car;
  Object* cdr;
};

struct Func {
  std::string name;
  int nArgs;
  FunctionTag tag;
};

struct UserFunc {
  Object* argList;
  Object* bodyList;
  Environment* env;
};

struct Object {
  TypeTag tag;
  std::variant<int, double, std::string, Cons, Func, UserFunc> value;
};

using VoidPtrFunc = void*();
using Continuation = VoidPtrFunc*();
using ObjectVec = std::vector<Object*>;
using ObjectStack = std::stack<Object*>;
using FunctionStack = std::stack<Continuation*>;

bool hasTag(Object* obj, TypeTag tag);
bool isFloat(Object* obj);
std::string tagToString(TypeTag tag);
std::string toString(scm::Object* obj);
static std::string consToString(scm::Object* cons, std::string& str);

}
