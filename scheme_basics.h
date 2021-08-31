#pragma once
#include <variant>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <exception>
#include <sstream>

namespace scm {

struct Object;
class Environment;

enum ObjectTag {
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
  FUNC_ADD,
  FUNC_SUB,
  FUNC_MUL,
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
  FUNC_FUNCTION_ARGS,
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
  ObjectTag tag;
  std::variant<int, double, std::string, Cons, Func, UserFunc> value;
};

/**
 *
 */
class schemeException : public std::runtime_error {
  const char* file;
  const char* errorMsg;
  int line;

 public:
  schemeException(const char* msg,
              const char* file_,
              int line_) : std::runtime_error(msg), file (file_), line (line_) {
    errorMsg = msg;
  }

  const char* get_file() const { return file; }
  int get_line() const { return line; }
  const char * what () const noexcept override { return errorMsg; }

};

using VoidPtrFunc = void*();
using Continuation = VoidPtrFunc*();
using ObjectVec = std::vector<Object*>;

bool hasTag(Object* obj, ObjectTag tag);
bool isFloat(Object* obj);
bool isString(Object* obj);
bool isNumber(Object* obj);
bool isSameType(Object* obj, std::vector<ObjectTag> validTypes);

std::string tagToString(ObjectTag tag);
std::string toString(scm::Object* obj);
static std::string consToString(scm::Object* cons, std::string& str);

}
