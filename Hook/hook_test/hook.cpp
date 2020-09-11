#include <iostream>
#include "subhook.h"

subhook::Hook foo_hook;
subhook::Hook foo_hook_tr;

typedef void (*foo_func)(int x);
extern "C" void foo() {
  puts("foo() called");
}
void my_foo(int x) {
  // ScopedHookRemove removes the specified hook and automatically re-installs
  // it when the object goes out of scope (thanks to C++ destructors).
  subhook::ScopedHookRemove remove(&foo_hook);

  std::cout << "foo(" << x << ") called" << std::endl;
  foo(x + 1);
}

void my_foo_tr(int x) {
  std::cout << "foo(" << x << ") called" << std::endl;

  // Call the original function via trampoline.
  ((foo_func)foo_hook_tr.GetTrampoline())(x + 1);
}

int main() {
  foo_hook.Install((void *)foo, (void *)my_foo);
  foo_hook_tr.Install((void *)foo, (void *)my_foo_tr);
}
