#ifndef CALL_C_FUNCTION_HOOK_H_
#define CALL_C_FUNCTION_HOOK_H_

#include <string>

class CallCFunctionHookInterface {
 public:
   virtual void Call(const std::string& name, const std::string& param, const std::string& result) = 0;
};

#endif  // CALL_C_FUNCTION_HOOK_H_
