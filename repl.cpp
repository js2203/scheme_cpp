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

      // if an argument can't be read, exit the loop
      if (argument == SCM_EOF) {
        return;
      }
      Object* value = trampoline::evaluateInput(env, argument);
      if (value != scm::SCM_VOID) {
        std::cout << "--> " << scm::toString(value) << std::endl;
      }
    }
    // catch the error if something went wrong in the scheme evaluation
    catch (scm::schemeException& ex) {
      std::cerr << ex.what() << '\n';
    }
      // catch the error if something went wrong in cpp in general
    catch (std::exception& ex) {
      std::cerr << ex.what() << '\n';
    }
  }
}

}