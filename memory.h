#pragma once
#include "scheme_functions.h"

namespace scm {

extern Object* SCM_NIL;
extern Object* SCM_VOID;
extern Object* SCM_EOF;
extern Object* SCM_TRUE;
extern Object* SCM_FALSE;

Object* newInteger(int value);
Object* newFloat(double value);
Object* newString(std::string value);
Object* newEnv();
Object* newSymbol(std::string value);
Object* newCons(Object* car, Object* cdr);

void initSingletons();

}
