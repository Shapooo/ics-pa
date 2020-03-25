#include "rtl/rtl.h"

void raise_intr(uint32_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  rtl_push(&cpu.eflags_val);
  cpu.IF = 0;
  cpu.TF = 0;
  rtl_push(&cpu.cs);
  rtl_push(&cpu.pc);
#define IDT_ITEM_SIZE 8         /* 64 bits */
  uint32_t hi = vaddr_read(cpu.idtr.base + NO * IDT_ITEM_SIZE + 6, 2);
  uint32_t lo = vaddr_read(cpu.idtr.base + NO * IDT_ITEM_SIZE, 2);
  rtl_j((hi << 16) | lo);
}

bool isa_query_intr(void) {
  return false;
}
