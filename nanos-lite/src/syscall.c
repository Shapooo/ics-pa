#include "common.h"
#include "syscall.h"
#include <stdint.h>
inline static uintptr_t sys_exit(_Context *c) {
  uintptr_t exit_status = c->GPR2;
  _halt(exit_status);
  return 0;
}

inline static uintptr_t sys_yield(_Context *c) {
  _yield();
  return 0;
}

#define STDOUT 1
#define STDERR 2

inline static uintptr_t sys_write(_Context *c) {
  uintptr_t fd = c->GPR2;
  char *buf = (char *)c->GPR3;
  uintptr_t count = c->GPR4;
  if (fd == STDOUT || fd == STDERR) {
    for (int i = 0; i < count; ++i) {
      _putc(*buf++);
    }
  }
  return count;
}

_Context *do_syscall(_Context *c) {
  /* uintptr_t a[4]; */
  /* a[0] = c->GPR1; */
  /* a[1] = c->GPR2; */
  /* a[2] = c->GPR3; */
  /* a[3] = c->GPR4; */
  uintptr_t syscall_id = c->GPR1;

  switch (syscall_id) {
  case (SYS_exit): {
    c->GPRx = sys_exit(c);
    break;
  }
  case (SYS_yield): {
    c->GPRx = sys_yield(c);
    break;
  }
  case (SYS_write): {
    c->GPRx = sys_write(c);
    break;
  }
  default:
    panic("Unhandled syscall ID = %d", syscall_id);
  }

  return c;
}
