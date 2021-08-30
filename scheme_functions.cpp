#include "scheme_functions.h"

namespace scm {

/**
 *
 * @param obj
 * @param tag
 * @return
 */
bool hasTag(Object* obj, TypeTag tag) {
  return (scm::getTag(obj) == tag);
}

/**
 *
 * @param obj
 * @return
 */
bool isFloat(Object* obj) {
  return hasTag(obj, TAG_FLOAT);
}

/**
 *
 * @param cons
 * @param str
 * @return
 */
static std::string consToString(scm::Object* cons, std::string& str)
{
  scm::Object *car, *cdr;
  car = getCar(cons);
  cdr = getCdr(cons);
  str += toString(car) + " ";
  if (hasTag(cdr, TAG_CONS)) {
    return consToString(cdr, str);
  }
  else if (cdr->tag == TAG_NIL) {
    return str + ")";
  }
  else {
    return str + ". " + toString(cdr) + ')';
  }
}

bool hasTag(Object* obj, TypeTag tag)
{
  return (getTag(obj) == tag);
}

/**
 *
 * @param obj
 * @return
 */
std::string toString(Object* obj) {
  std::string consStart;
  switch (obj->tag) {
    case TAG_INT:
      return std::to_string(std::get<int>(obj->value));
    case TAG_FLOAT:
      return std::to_string(std::get<double>(obj->value));
    case TAG_STRING:
      return '"' + std::get<std::string>(obj->value) + '"';
    case TAG_SYMBOL:
      return std::get<std::string>(obj->value);
    case TAG_NIL:
      return "()";
    case TAG_FALSE:
      return "#f";
    case TAG_TRUE:
      return "#t";
    case TAG_CONS:
      consStart = "( ";
      return consToString(obj, consStart);
    case TAG_EOF:
      return "finished";
    case TAG_VOID:
      return "";
    default:
      return "{{TO STRING NOT YET IMPLEMENTED FOR TAG " + std::to_string(obj->tag) + "}}";
  }
}
/**
 *
 * @param tag
 * @return
 */
std::string tagToString(TypeTag tag) {
  switch (tag) {
    case TAG_INT:
      return "integer";
    case TAG_FLOAT:
      return "float";
    case TAG_STRING:
      return "string";
    case TAG_SYMBOL:
      return "symbol";
    case TAG_CONS:
      return "cons";
    case TAG_NIL:
      return "cons";
    case TAG_TRUE:
      return "true";
    case TAG_FALSE:
      return "false";
    case TAG_FUNC_BUILTIN:
      return "builtin function";
    case TAG_FUNC_USER:
      return "user function";
    case TAG_SYNTAX:
      return "syntax";
    case TAG_VOID:
      return "void";
    default:
      return "unrecognized tag";
  };
}

}