#pragma once
#include <iostream>
#include <vector>
#include "scheme_basics.h"
#include "memory.h"

namespace scm {

Object* readInput(std::istream* streamPointer);
Object* interpretInput(std::vector<std::string>::iterator& current);

}
