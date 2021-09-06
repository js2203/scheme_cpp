#include "scheme_basics.h"
#include "scheme_getter.h"

namespace scm {

/**
 * checks whether a scheme object has a certain tag
 * @param obj the scheme object
 * @param tag the tag to check for
 * @return true if the object has the tag, else false
 */
bool hasTag(Object* obj, ObjectTag tag) {
  return (getTag(obj) == tag);
}

/**
 * checks whether a scheme object has the float tag
 * @param obj the scheme object
 * @return true if the object has a float tag, else false
 */
bool isFloat(Object* obj) {
  return hasTag(obj, TAG_FLOAT);
}

/**
 * checks whether a scheme object has the string tag
 * @param obj the scheme object
 * @return true if the object has a string tag, else false
 */
bool isString(Object* obj) {
  return hasTag(obj, TAG_STRING);
}

/**
 * checks whether a scheme object has the integer or float tag
 * @param obj the scheme object
 * @return true if the object has a integer or float tag, else false
 */
bool isNumber(Object* obj) {
    return (hasTag(obj, TAG_INT) || hasTag(obj, TAG_FLOAT));
}

/**
 * checks whether a scheme object has the a tag from a list of tags
 * @param obj obj the scheme object
 * @param validTags the allowed tags
 * @return true if the object has a tag from the list, else false
 */
bool isSameType(Object* obj, std::vector<ObjectTag> validTags) {
  auto lambda = [obj](ObjectTag tag) {
    return hasTag(obj, tag);
  };
  return std::any_of(validTags.begin(), validTags.end(), lambda);
}

/**
 * converts a scheme cons object to a string
 * @param cons the scheme object
 * @param str the string in which the values of the cons object are saved
 * @return a string with the cons values
 */
static std::string consToString(scm::Object* cons, std::string& str) {
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

/**
 * converts a scheme object into a printable string
 * @param obj the scheme object
 * @return a string with the value of the scheme object
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
      return "unable to print: " + std::to_string(obj->tag);
  }
}
/**
 * converts a scheme object tag into a printable string
 * @param tag the scheme object tag
 * @return a string with the tag of the scheme object
 */
std::string tagToString(ObjectTag tag) {
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
      return "null";
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
  }
}

}