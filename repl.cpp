#include "repl.h"

namespace scm {

/**
 * Read-Eval-Print-Loop (repl)
 * Reads input from an user or a file, evaluates the content and returns/ prints the result
 * @param streamPointer
 */
void repl(std::istream* streamPointer, Environment& env) {

  // endless loop until manually exited or an error occurs
  for (;;) {
    try {
      Object* argument = readInput(streamPointer);
      std::cout << tagToString(argument->tag); // TODO REMOVE
      std::cout << "\n--> ";
      // if an argument can't be read, exit the loop
      if (argument == SCM_EOF) {
        return;
      }
      Object* value = trampoline::evaluateInput(env, argument); // TODO
    }
    // catch standard c++ error
    catch (std::exception& e) {
      std::cerr << "C++ ERROR: " << e.what() << '\n';
    }
  }
}

}