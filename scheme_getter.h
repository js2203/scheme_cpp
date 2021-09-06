#pragma once
#include "scheme_basics.h"
#include "scheme_builtInFunc.h"
#include "scheme_syntax.h"
#include "memory.h"
#include "environment.h"

namespace scm {

ObjectTag getTag(Object* obj);
std::string getStringValue(Object* obj);
int getIntValue(Object* obj);
double getFloatValue(Object* obj);
Cons getCons(Object* obj);
Object* getCar(Object* obj);
Object* getCdr(Object* obj);

Continuation* getBuiltinFunc(Object* function);
Continuation* getBuiltinSyntax(Object* Syntax);
Object* getUserFunctionBody(Object* obj);
Object* getUserFunctionArgs(Object* obj);
int getBuiltinFuncNArgs(Object* obj);
Environment* getUserFunctionParentEnv(Object* obj);

} // namespace scm