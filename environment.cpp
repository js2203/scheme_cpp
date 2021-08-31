#include "environment.h"
#include "scheme_getter.h"

namespace scm {

/**
 * define a new binding to a key in the environment
 * @param env
 * @param key
 * @param value
 */
void defineKey(Environment& env, Object* key, Object* value) {
  if (hasTag(key, TAG_SYMBOL)) {
    env.bindings[getStringValue(key)] = value;
  }
}

/**
 *
 * @param env
 * @param key
 * @param value
 */
void setKey(Environment& env, Object* key, Object* value) {
  Environment* currentEnvPtr = &env;
  // define variable in every env until no parent env can be found
  while (currentEnvPtr != nullptr) {
    defineKey(*currentEnvPtr, key, value);
    currentEnvPtr = (*currentEnvPtr).parentEnv;
  }
}

/**
 *
 * @param env
 * @param key
 * @return
 */
Object* getVariable(Environment& env, Object* key) {
  Environment* currentEnvPtr = &env;
  if (hasTag(key, TAG_SYMBOL)) {
    while (currentEnvPtr != nullptr) {
        if (currentEnvPtr->bindings.empty()){
          throw schemeException("no variable was declared in the environment", __FILE__, __LINE__);
        }
        auto found = currentEnvPtr->bindings.find(getStringValue(key));
        if (found != currentEnvPtr->bindings.end()) {
          return currentEnvPtr->bindings.at(getStringValue(key));
        } else {
          currentEnvPtr = currentEnvPtr->parentEnv;
        }
      }
  }
  return nullptr;
}

/**
 *
 * @param env
 */
Environment::Environment(const Environment& env) {
  parentEnv = env.parentEnv;
  bindings = env.bindings;
}

} // namespace scm