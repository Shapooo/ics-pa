#include "common.h"

extern _Context* do_syscall(_Context *c);
static _Context* do_event(_Event e, _Context* c) {
  _Context * ret;
  switch (e.event) {
  case (_EVENT_YIELD): {
    printf("Yield test...\n");
    ret = NULL;
    break;
  }
  case (_EVENT_SYSCALL): {
    ret = do_syscall(c);
    break;
  }
    default: panic("Unhandled event ID = %d", e.event);
  }

  return ret;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  _cte_init(do_event);
}
