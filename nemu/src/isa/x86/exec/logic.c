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

  operand_write(id_dest, &s0);

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

  operand_write(id_dest, &s0);

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

  operand_write(id_dest, &s0);

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

  operand_write(id_dest, &s0);

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
    rtl_shr(&s1, &s0, &id_src->val);
  }

  operand_write(id_dest, &s0);

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

  operand_write(id_dest, &s0);

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
  rtl_not(&s0, &id_dest->val);
  operand_write(id_dest, &s0);

  print_asm_template1(not);
}

make_EHelper(rol) {
  uint32_t count = id_src->val % (id_dest->width << 3);
  uint32_t mask = 0xffffffff >> ((4 - id_dest->width) << 3);
  s0 = (0xffffffff >> (4 - id_dest->width) << 3) & mask;
  rtl_and(&s0, &id_dest->val, &s0);
  s0 >>= (id_dest->width << 3) - count;
  rtl_shli(&s1, &id_dest->val, id_src->val);
  s1 &= mask;
  rtl_or(&s0, &s0, &s1);

  operand_write(id_dest, &s0);

  s1 = s0 & 0x1;
  rtl_set_CF(&s1);
  if (id_src->val == 1) {
    s0 = (s0 >> ((id_dest->width << 3) - 1)) & 0x1;
    s1 = (s0 != s1);
    rtl_set_OF(&s1);
  }
}

make_EHelper(ror) {
  int count = id_src->val % (id_dest->width << 3);
  s0 = 0xffffffff >> ((4 << 3) - count);
  rtl_and(&s0, &id_dest->val, &s0);
  s0 <<= (id_dest->width) - count;
  rtl_shri(&s1, &id_dest->val, id_src->val);
  rtl_or(&s0, &s0, &s1);

  operand_write(id_dest, &s0);

  s1 = (s0 >> ((id_dest->width << 3) - 1));
  rtl_set_CF(&s1);
  if (id_src->val == 1) {
    s0 = (s0 >> ((id_dest->width << 3) - 2)) & 0x1;
    s1 = (s0 != s1);
    rtl_set_OF(&s1);
  }
}

make_EHelper(rcl) {
  uint32_t count = id_src->val % (id_dest->width << 3);
  uint32_t mask = 0xffffffff >> ((4 - id_dest->width) << 3);
  s0 = (0xffffffff >> (4 - id_dest->width) << 3) & mask;
  rtl_and(&s0, &id_dest->val, &s0);
  s0 >>= (id_dest->width << 3) - count;
  rtl_shli(&s1, &id_dest->val, id_src->val);
  s1 &= mask;
  rtl_or(&s0, &s0, &s1);

  uint32_t tmpcf;
  rtl_get_CF(&tmpcf);
  s1 = s0 & 0x1;
  s0 &= 0xffffffff << 1;
  s0 += tmpcf;

  operand_write(id_dest, &s0);

  rtl_set_CF(&s1);
  if (id_src->val == 1) {
    s0 = (s0 >> ((id_dest->width << 3) - 1)) & 0x1;
    s1 = (s0 != s1);
    rtl_set_OF(&s1);
  }
}

make_EHelper(rcr) {
  int count = id_src->val % (id_dest->width << 3);
  s0 = 0xffffffff >> ((4 << 3) - count);
  rtl_and(&s0, &id_dest->val, &s0);
  s0 <<= (id_dest->width) - count;
  rtl_shri(&s1, &id_dest->val, id_src->val);
  rtl_or(&s0, &s0, &s1);

  uint32_t tmpcf;
  rtl_get_CF(&tmpcf);
  s1 = (s0 >> ((id_dest->width << 3) - 1));
  s0 &= 0xffffffff >> (((4 - id_dest->width) << 3) + 1);
  s0 += tmpcf << ((id_dest->width << 3) - 1);

  operand_write(id_dest, &s0);

  rtl_set_CF(&s1);
  if (id_src->val == 1) {
    s0 = (s0 >> ((id_dest->width << 3) - 2)) & 0x1;
    s1 = (s0 != s1);
    rtl_set_OF(&s1);
  }
}
