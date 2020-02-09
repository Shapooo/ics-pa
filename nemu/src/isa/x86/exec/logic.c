#include "cc.h"
#include "cpu/decode.h"
#include "cpu/exec.h"
#include "rtl/rtl-wrapper.h"
#include "rtl/rtl.h"

make_EHelper(test) {
  /* TODO(); */
  if (id_src->type == OP_TYPE_IMM) {
    rtl_andi(&s0, &id_dest->val, id_src->val);
  } else {
    rtl_and(&s0, &id_src->val, &id_dest->val);
  }

  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(test);
}

make_EHelper(and) {
  /* TODO(); */
  if (id_src->type == OP_TYPE_IMM) {
    rtl_andi(&s0, &id_dest->val, id_src->val);
  } else {
    rtl_and(&s0, &id_src->val, &id_dest->val);
  }

  operand_write(&id_dest, &s0);

  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(and);
}

make_EHelper(xor) {
  /* TODO(); */
  if (id_src->type == OP_TYPE_IMM) {
    rtl_xori(&s0, &id_dest->val, id_src->val);
  } else {
    rtl_xor(&s0, &id_src->val, &id_dest->val);
  }

  operand_write(&id_dest, &s0);

  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(xor);
}

make_EHelper(or) {
  /* TODO(); */
  if (id_src->type == OP_TYPE_IMM) {
    rtl_ori(&s0, &id_dest->val, id_src->val);
  } else {
    rtl_or(&s0, &id_src->val, &id_dest->val);
  }

  operand_write(&id_dest, &s0);

  s1 = 0;
  rtl_set_CF(&s1);
  rtl_set_OF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(or);
}

make_EHelper(sar) {
  /* TODO(); */
  // unnecessary to update CF and OF in NEMU
  if (id_src->type == OP_TYPE_IMM) {
    rtl_sari(&s0, &id_dest->val, id_src->val);
  } else {
    rtl_sar(&s0, &id_dest->val, &id_src->val);
  }

  operand_write(&id_dest, &s0);

  s1 = 0;
  rtl_set_OF(&s1);
  rtl_set_CF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(sar);
}

make_EHelper(shl) {
  /* TODO(); */
  // unnecessary to update CF and OF in NEMU
  if (id_src->type == OP_TYPE_IMM) {
    rtl_shli(&s0, &id_dest->val, id_src->val);
    rtl_shri(&s1, &s0, id_src->val);
  } else {
    rtl_shl(&s0, &id_dest->val, &id_src->val);
    rtl_shr(&s1, &s0, id_src->val);
  }

  operand_write(&id_dest, &s0);

  s1 = (s1 != id_dest->val);
  rtl_set_OF(&s1);
  rtl_set_CF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(shl);
}

make_EHelper(shr) {
  /* TODO(); */
  // unnecessary to update CF and OF in NEMU
  if (id_src->type == OP_TYPE_IMM) {
    rtl_shri(&s0, &id_dest->val, id_src->val);
  } else {
    rtl_shr(&s0, &id_dest->val, &id_src->val);
  }

  operand_write(&id_dest, &s0);

  s1 = 0;
  rtl_set_OF(&s1);
  rtl_set_CF(&s1);
  rtl_update_ZFSF(&s0, id_dest->width);

  print_asm_template2(shr);
}

make_EHelper(setcc) {
  uint32_t cc = decinfo.opcode & 0xf;

  rtl_setcc(&s0, cc);
  operand_write(id_dest, &s0);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

make_EHelper(not) {
  /* TODO(); */
  s0 = id_dest->val;
  operand_write(id_dest, &s0);

  print_asm_template1(not);
}
