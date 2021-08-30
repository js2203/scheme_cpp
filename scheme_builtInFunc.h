#pragma once
#include "evaluate.h"
#include "memory.h"
#include "scheme_functions.h"
#include "scheme_getter.h"
#include "trampoline.h"
#include <algorithm>
#include <string>
#include <variant>
#include <iostream>
#include <numeric>

namespace scm::trampoline {

Continuation* addition();
Continuation* subtraction();
Continuation* multiplication();
Continuation* division();
Continuation* equal();
Continuation* equalString();
Continuation* equalNumber();
Continuation* greaterThan();
Continuation* lesserThan();
Continuation* buildCons();
Continuation* getCarFunc();
Continuation* getCdrFunc();
Continuation* buildList();
Continuation* display();
Continuation* returnFuncBody();
Continuation* returnFuncArguments();
Continuation* isStringFunc();
Continuation* isNumberFunc();
Continuation* isConsFunc();
Continuation* isBuiltinFunc();
Continuation* isUserFunc();
Continuation* isBoolFunc();

} // namespace scm::trampoline