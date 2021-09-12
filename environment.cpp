#include "environment.h"
#include "scheme_getter.h"

namespace scm {

/**
 * define a new binding to a key in the environment
 * @param env the environment in which the key should be set
 * @param key the name under which the value should be set
 * @param value the value for the key
 */
void defineKey(Environment& env, Object* key, Object* value) {
  if (hasTag(key, TAG_SYMBOL)) {
    env.bindings[getStringValue(key)] = value;
  } else {
    throw schemeException("object is not a symbol", __FILE__, __LINE__);
  }
}

/**
 * define a new binding to a key in the environment, while the key is a string
 * @param env the environment in which the key should be set
 * @param key the name under which the value should be set
 * @param value the value for the key
 */
void defineKeyString(Environment &env, const std::string& key, Object *value) {
  env.bindings[key] = value;
}

/**
 * set a new variable with a value for all environments
 * @param env the environment in which the key should be set
 * @param key the name under which the value should be set
 * @param value the value for the key
 */
void setKey(Environment& env, Object* key, Object* value) {
  Environment* currentEnvPtr = &env;
  while (currentEnvPtr != nullptr) {
    defineKey(*currentEnvPtr, key, value);
    currentEnvPtr = (*currentEnvPtr).parentEnv;
  }
}

/**
 * return the value of a variable which is set in ether the current environment or in one of its parent environments
 * @param env the environment from which the key should be retrieved
 * @param key the name from which the value should be retrieved
 * @return
 */
Object* getVariable(Environment& env, Object* key) {
  Environment* currentEnvPtr = &env;
  if (hasTag(key, TAG_SYMBOL)) {
    while (currentEnvPtr != nullptr) {
        if (currentEnvPtr->bindings.empty()){
          throw schemeException("no variable was declared in the environment", __FILE__, __LINE__);
        }
        std::string keyStr{getStringValue(key)};
        auto envKey = currentEnvPtr->bindings.find(keyStr);
        if (envKey != currentEnvPtr->bindings.end()) {
          return currentEnvPtr->bindings.at(getStringValue(key));
        } else {
          currentEnvPtr = currentEnvPtr->parentEnv;
        }
      }
  }
  return nullptr;
}

/**
 * Constructor for a environemnt
 * @param env the environment
 */
Environment::Environment(const Environment& env) {
  parentEnv = env.parentEnv;
  bindings = env.bindings;
}

} // namespace scm