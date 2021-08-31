#pragma once
#include "scheme_basics.h"
#include "scheme_getter.h"
#include "environment.h"
#include "trampoline.h"

namespace scm::trampoline {

Object* evaluateInput(Environment& env, Object* obj);

Continuation* trampolineEvaluateFirst();


} // namespace scm
