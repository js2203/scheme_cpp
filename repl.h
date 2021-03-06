#pragma once
#include <iostream>
#include "environment.h"
#include "scheme_basics.h"
#include "parser.h"
#include "memory.h"
#include "evaluate.h"

namespace scm {

void repl(std::istream* streamPtr, Environment& env);

}// namespace scm