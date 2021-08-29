#include "memory.h"
#include "scheme_functions.h"

namespace scm {

Object* SCM_NIL{new Object};
Object* SCM_VOID{new Object};
Object* SCM_EOF{new Object};
Object* SCM_TRUE{new Object};
Object* SCM_FALSE{new Object};

/**
 * initializes all constant Objects
 */
void initSingletons() {
  SCM_NIL->tag = TAG_NIL;
  SCM_VOID->tag = TAG_VOID;
  SCM_EOF->tag = TAG_EOF;
  SCM_TRUE->tag = TAG_TRUE;
  SCM_FALSE->tag = TAG_FALSE;
}


/**
 *
 * @param value
 * @return
 */
Object* newInteger(int value) {
  Object* newInt{new Object};
  newInt->tag = TAG_INT;
  newInt->value = value;
  return newInt;
}

/**
 *
 * @param value
 * @return
 */
Object* newFloat(double value) {
  Object* newFloat{new Object};
  newFloat->tag = TAG_FLOAT;
  newFloat->value = value;
  return newFloat;
}

/**
 *
 * @param value
 * @return
 */
Object* newString(std::string value) {
  Object* newString{new Object};
  newString->tag = TAG_STRING;
  newString->value = value;
  return newString;
}

/**
 *
 * @param value
 * @return
 */
Object* newSymbol(std::string value) {
  Object* newSymbol{new Object};
  newSymbol->tag = TAG_SYMBOL;
  newSymbol->value = value;
  return newSymbol;
}

/**
 *
 * @param car
 * @param cdr
 * @return
 */
Object* newCons(Object* car, Object* cdr) {
  Object* newCons{new Object};
  newCons->tag = TAG_CONS;
  newCons->value = Cons{car, cdr};
  return newCons;
}

}