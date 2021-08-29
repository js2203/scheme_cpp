#pragma once
#include "scheme_functions.h"

namespace scm {

TypeTag getTag(Object* obj);
std::string getStringValue(Object* obj);
int getIntValue(Object* obj);
double getFloatValue(Object* obj);
Cons getCons(Object* obj);
Object* getCar(Object* obj);
Object* getCdr(Object* obj);
// FunctionTag getBuiltinFuncTag(Object* obj);
std::string getBuiltinFuncName(Object* obj);
int getBuiltinFuncNArgs(Object* obj);
std::string getBuiltinFuncHelpText(Object* obj);
Object* getUserFunctionBodyList(Object* obj);
Object* getUserFunctionArgList(Object* obj);
std::string getBuiltinFuncHelpText(Object* obj);
// Environment* getUserFunctionParentEnv(Object* obj);

} // namespace scm