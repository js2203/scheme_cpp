#pragma once
#include <map>
#include "scheme_functions.h"

namespace scm{

class Environment {
 private:
  std::map<std::string, Object*> bindings;
  Environment* parentEnv;

 public:
  Environment(Environment* parent = NULL) : parentEnv(parent){};
  Environment(const Environment& obj);
  ~Environment() = default;
  friend void setKey(Environment& env, Object* key, Object* value);
  friend void defineKey(Environment& env, Object* key, Object* value);
  friend Object* getVariable(Environment& env, Object* key);
};

void defineKey(Environment& env, Object* key, Object* value);
void setKey(Environment& env, Object* key, Object* value);
void printEnv(Environment& env);
Object* getVariable(Environment& env, Object* key);

} // namespace scm
