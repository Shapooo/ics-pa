#include "common.h"
#include "syscall.h"
#include <stdint.h>
#include "proc.h"

void naive_uload(PCB *pcb, const char *filename);

inline static uintptr_t sys_exit(_Context *c) {
  /* uintptr_t exit_status = c->GPR2; */
  /* _halt(exit_status); */
  naive_uload(NULL, "/bin/init");
  return 0;
}

inline static uintptr_t sys_yield(_Context *c) {
  _yield();
  return 0;
}

extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

inline static uintptr_t sys_open(_Context *c) {
  const char *path = (const char *)c->GPR2;
  int flags = (int)c->GPR3;
  int mode = (int)c->GPR4;
  return fs_open(path, flags, mode);
}

inline static uintptr_t sys_read(_Context *c) {
  int fd = (int)c->GPR2;
  void *buf = (void *)c->GPR3;
  size_t count = (size_t)c->GPR4;
  return fs_read(fd, buf, count);
}

inline static uintptr_t sys_write(_Context *c) {
  uintptr_t fd = c->GPR2;
  char *buf = (char *)c->GPR3;
  size_t count = c->GPR4;
  return fs_write(fd, buf, count);
}

inline static uintptr_t sys_close(_Context *c) {
  int fd = (int)c->GPR2;
  return fs_close(fd);
}

inline static uintptr_t sys_lseek(_Context *c) {
  int fd = (int)c->GPR2;
  size_t offset = (size_t)c->GPR3;
  int whence = (int)c->GPR4;
  return fs_lseek(fd, offset, whence);
}

inline static uintptr_t sys_execve(_Context *c) {
  const char *fname = (const char *)c->GPR2;
  naive_uload(NULL, fname);
  return 0;
}

_Context *do_syscall(_Context *c) {
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
  case (SYS_open): {
    c->GPRx = sys_open(c);
    break;
  }
  case (SYS_read): {
    c->GPRx = sys_read(c);
    break;
  }
  case (SYS_write): {
    c->GPRx = sys_write(c);
    break;
  }
  case (SYS_close): {
    c->GPRx = sys_close(c);
    break;
  }
  case (SYS_lseek): {
    c->GPRx = sys_lseek(c);
    break;
  }
  case (SYS_brk): {
    c->GPRx = 0;
    break;
  }
  case (SYS_execve): {
    c->GPRx = sys_execve(c);
    break;
  }
  default:
    panic("Unhandled syscall ID = %d", syscall_id);
  }

  return c;
}
