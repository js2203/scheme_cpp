#pragma once
#include <variant>
#include <string>
#include <vector>

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

struct Cons {
  Object* car;
  Object* cdr;
};

struct Func {
  std::string name;
  int nArgs;
  //FunctionTag funcTag;
  std::string helpText;
};

struct UserFunc {
  Object* argList;
  Object* bodyList;
  //Environment* env;
};

struct Object {
  TypeTag tag;
  std::variant<int, double, std::string, Cons> value;
};

using VoidPtrFunc = void*();
using Continuation = VoidPtrFunc*();

// Forward Declarations
bool hasTag(Object* obj, TypeTag tag);
bool isString(Object* obj);
bool isNumeric(Object* obj);
bool isFloatingPoint(Object* obj);
bool isOneOf(Object* obj, std::vector<TypeTag> validTypes);
std::string tagToString(TypeTag tag);
std::string toString(scm::Object* obj);
static std::string consToString(scm::Object* cons, std::string& str);
std::string prettifyUserFunction(Object* func);

}
