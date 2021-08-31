#pragma once
#include "evaluate.h"
#include "memory.h"
#include "scheme_basics.h"
#include "scheme_getter.h"
#include "trampoline.h"
#include <algorithm>
#include <string>
#include <variant>
#include <iostream>
#include <numeric>

namespace scm::trampoline {

Continuation* syntaxDefine();
Continuation* syntaxSet();
Continuation* syntaxQuote();
Continuation* syntaxIf();
Continuation* syntaxBegin();
Continuation* syntaxLambda();

} // namespace scm::trampoline