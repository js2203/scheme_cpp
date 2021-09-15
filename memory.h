#pragma once
#include "scheme_basics.h"

namespace scm {

void initConstSchemeObjects();

Object* newInteger(int value);
Object* newFloat(double value);
Object* newString(std::string value);
Object* newSymbol(std::string value);
Object* newCons(Object* car, Object* cdr);
Object* newUserFunction(Object* arguments, Object* body, Environment& env);
Object* newBuiltinFunction(const std::string& name, int numArgs, FunctionTag tag);
Object* newSyntax(const std::string& name, int numArgs, FunctionTag tag);

extern Object* SCM_NIL;
extern Object* SCM_VOID;
extern Object* SCM_EOF;
extern Object* SCM_TRUE;
extern Object* SCM_FALSE;

} //namespace SCM
