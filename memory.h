#pragma once
#include "scheme_basics.h"

namespace scm {

void initConstSchemeObjects();

Object* newInteger(int value);
Object* newFloat(double value);
Object* newString(std::string value);
Object* newSymbol(std::string value);
Object* newCons(Object* car, Object* cdr);
Object* newUserFunction(Object* argList, Object* bodyList, Environment& homeEnv);
Object* newBuiltinFunction(std::string name, int numArgs, FunctionTag funcTag);
Object* newSyntax(std::string name, int numArgs, FunctionTag funcTag);

extern Object* SCM_NIL;
extern Object* SCM_VOID;
extern Object* SCM_EOF;
extern Object* SCM_TRUE;
extern Object* SCM_FALSE;

} //namespace SCM
