#pragma once
#include <map>
#include "scheme_basics.h"

namespace scm{

class Environment {
 private:
  std::map<std::string, Object*> bindings;
  Environment* parentEnv;

 public:
  explicit Environment(Environment* parent = nullptr) : parentEnv(parent){};
  Environment(const Environment& obj);
  ~Environment() = default;
  friend void setKey(Environment& env, Object* key, Object* value);
  friend void defineKey(Environment& env, Object* key, Object* value);
  friend void defineKeyString(Environment& env, const std::string& key, Object* value);
  friend Object* getVariable(Environment& env, Object* key);
};

void defineKey(Environment& env, Object* key, Object* value);
void defineKeyString(Environment& env, const std::string& key, Object* value);
void setKey(Environment& env, Object* key, Object* value);
Object* getVariable(Environment& env, Object* key);

} // namespace scm
