#include "cpu/exec.h"

make_EHelper(lidt) {
  /* TODO(); */
  s0 = id_dest->addr;
  rtl_lm(&s1, &s0, 2);
  cpu.idtr.limit = s1;
  s0 += 2;
  rtl_lm(&s1, &s0, 4);
  cpu.idtr.base = s1;

  print_asm_template1(lidt);
}

make_EHelper(mov_r2cr) {
  TODO();

  print_asm("movl %%%s,%%cr%d", reg_name(id_src->reg, 4), id_dest->reg);
}

make_EHelper(mov_cr2r) {
  TODO();

  print_asm("movl %%cr%d,%%%s", id_src->reg, reg_name(id_dest->reg, 4));

  difftest_skip_ref();
}

make_EHelper(int) {
  /* TODO(); */
  extern void raise_intr(uint32_t, vaddr_t);
  raise_intr(id_dest->val, decinfo.seq_pc);

  print_asm("int %s", id_dest->str);

  difftest_skip_dut(1, 2);
}

make_EHelper(iret) {
  /* TODO(); */
  rtl_pop(&decinfo.seq_pc);
  rtl_pop(&cpu.cs);
  rtl_pop(&cpu.eflags_val);

  print_asm("iret");
}

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

make_EHelper(in) {
  /* TODO(); */
  if (id_dest->width == 4) {
    s0 = pio_read_l((ioaddr_t)id_src->val);
  } else if (id_dest->width == 2) {
    s0 = pio_read_w((ioaddr_t)id_src->val);
  } else {
    s0 = pio_read_b((ioaddr_t)id_src->val);
  }

  operand_write(id_dest, &s0);

  print_asm_template2(in);
}

make_EHelper(out) {
  /* TODO(); */
  if (id_dest->width == 4) {
    pio_write_l((ioaddr_t)id_dest->val, id_src->val);
  } else if (id_dest->width == 2) {
    pio_write_w((ioaddr_t)id_dest->val, id_src->val);
  } else {
    pio_write_b((ioaddr_t)id_dest->val, id_src->val);
  }

  print_asm_template2(out);
}
