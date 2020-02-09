#include "cpu/exec.h"

make_EHelper(mov) {
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push) {
  /* TODO(); */
  rtl_push(&id_dest->val);
  print_asm_template1(push);
}

make_EHelper(pop) {
  /* TODO(); */
  rtl_pop(&id_dest->val);
  print_asm_template1(pop);
}

make_EHelper(pusha) {
  /* TODO(); */
  rtl_lr(&s0, R_ESP, id_dest->width);
  rtl_lr(&s1, R_EAX, id_dest->width);
  rtl_push(&s1);
  rtl_lr(&s1, R_ECX, id_dest->width);
  rtl_push(&s1);
  rtl_lr(&s1, R_EDX, id_dest->width);
  rtl_push(&s1);
  rtl_lr(&s1, R_EBX, id_dest->width);
  rtl_push(&s1);
  rtl_push(&s0);
  rtl_lr(&s1, R_EBP, id_dest->width);
  rtl_push(&s1);
  rtl_lr(&s1, R_ESI, id_dest->width);
  rtl_push(&s1);
  rtl_lr(&s1, R_EDI, id_dest->width);
  rtl_push(&s1);

  print_asm("pusha");
}

make_EHelper(popa) {
  /* TODO(); */
  rtl_pop(&s0);
  rtl_sr(R_EDI, &s0, id_dest->width);
  rtl_pop(&s0);
  rtl_sr(R_ESI, &s0, id_dest->width);
  rtl_pop(&s0);
  rtl_sr(R_EBP, &s0, id_dest->width);
  rtl_pop(&s0);
  rtl_pop(&s0);
  rtl_sr(R_EBX, &s0, id_dest->width);
  rtl_pop(&s0);
  rtl_sr(R_EDX, &s0, id_dest->width);
  rtl_pop(&s0);
  rtl_sr(R_ECX, &s0, id_dest->width);
  rtl_pop(&s0);
  rtl_sr(R_EAX, &s0, id_dest->width);

  print_asm("popa");
}

make_EHelper(leave) {
  TODO();

  print_asm("leave");
}

make_EHelper(cltd) {
  if (decinfo.isa.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl) {
  if (decinfo.isa.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }

  print_asm(decinfo.isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(&s0, &id_src->val, id_src->width);
  operand_write(id_dest, &s0);
  print_asm_template2(movsx);
}

make_EHelper(movzx) {
  id_dest->width = decinfo.isa.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea) {
  operand_write(id_dest, &id_src->addr);
  print_asm_template2(lea);
}
