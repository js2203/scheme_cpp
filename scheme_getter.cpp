#include "scheme_functions.h"
#include "scheme_getter.h"

namespace scm {

/**
 *
 * @param obj
 * @return
 */
TypeTag getTag(Object *obj) {
  return obj->tag;
}

/**
 *
 * @param obj
 * @return
 */
std::string getStringValue(Object *obj) {
  if (!hasTag(obj, TAG_STRING) && !hasTag(obj, TAG_SYMBOL)) {
    // schemeThrow("object has no string value that could be gotten");
  }
  return std::get<std::string>(obj->value);
}

/**
 *
 * @param obj
 * @return
 */
int getIntValue(Object *obj) {
  if (!hasTag(obj, TAG_INT)) {
    // schemeThrow("object has no integer value that could be gotten");
  }
  return std::get<int>(obj->value);
}

/**
 *
 * @param obj
 * @return
 */
double getFloatValue(Object *obj) {
  if (!hasTag(obj, TAG_FLOAT)) {
    // schemeThrow("object has no float value that could be gotten");
  }
  return std::get<double>(obj->value);
}

/**
 *
 * @param obj
 * @return
 */
Cons getCons(Object *obj) {
  if (!hasTag(obj, TAG_CONS)) {
    // schemeThrow("tried to get consvalue from non-cons object: " + toString(obj));
  }
  return std::get<Cons>(obj->value);
}

/**
 *
 * @param obj
 * @return
 */
Object *getCar(Object *obj) {
  auto cons{getCons(obj)};
  return cons.car;
}

/**
 *
 * @param obj
 * @return
 */
Object *getCdr(Object *obj) {
  auto cons{getCons(obj)};
  return cons.cdr;
}

} // namespace scm