#include "scheme_basics.h"
#include <iostream>
#include <fstream>
#include "memory.h"
#include "repl.h"
#include "environment.h"

int main(int argc, char** argv) {
  scm::initSingletons();
  scm::Environment baseEnv{};
  // scm::setupEnvironment(topLevelEnv); TODO

  // define input stream from file
  std::ifstream inputStream;
  std::cout << "scheme started!\n";

  switch(argc) {
    case 1: {
      // init stream from cin
      scm::repl(&std::cin, baseEnv);
      break;
    }
    case 2: {
      // read from file
      inputStream.open(argv[1]);
      if (!inputStream)
        return 1;
      scm::repl(&inputStream, baseEnv);
      break;
    }
    default:
      std::cout << "Too many arguments!";
      return 1;
  }


  return 0;
}
