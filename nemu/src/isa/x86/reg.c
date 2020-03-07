#include "nemu.h"
#include <stdlib.h>
#include <time.h>

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
  srand(time(0));
  uint32_t sample[8];
  uint32_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
    reg_l(i) = sample[i];
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
}

void isa_reg_display() {
  printf("[EAX] = %04x %02x %02x\n", reg_l(R_EAX)>>16, reg_w(R_AX)>>8, reg_b(R_AL));
  printf("[ECX] = %04x %02x %02x\n", reg_l(R_ECX)>>16, reg_w(R_AX)>>8, reg_b(R_CL));
  printf("[EDX] = %04x %02x %02x\n", reg_l(R_EDX)>>16, reg_w(R_AX)>>8, reg_b(R_DL));
  printf("[EBX] = %04x %02x %02x\n", reg_l(R_EBX)>>16, reg_w(R_AX)>>8, reg_b(R_BL));
  printf("[ESP] = %04x %02x %02x\n", reg_l(R_ESP)>>16, reg_w(R_AX)>>8, reg_b(R_SP) & 0xff);
  printf("[EBP] = %04x %02x %02x\n", reg_l(R_EBP)>>16, reg_w(R_AX)>>8, reg_b(R_BP) & 0xff);
  printf("[ESI] = %04x %02x %02x\n", reg_l(R_ESI)>>16, reg_w(R_AX)>>8, reg_b(R_SI) & 0xff);
  printf("[EDI] = %04x %02x %02x\n", reg_l(R_EDI)>>16, reg_w(R_AX)>>8, reg_b(R_DI) & 0xff);
  printf("[EIP] = %08x\n", cpu.pc);
  printf("[CF] = %d, [ZF] = %d, [SF] = %d, [OF] = %d\n[eflags] = %08x\n", cpu.CF, cpu.ZF, cpu.SF, cpu.OF, cpu.eflags_val);
}

uint32_t isa_reg_str2val(const char *s, bool *success) {
  /* return 0; */
  if (s[2] == 'a' && s[3] == 'x') {
    *success = true;
    return reg_l(R_EAX);
  } else if (s[2] == 'c' && s[3] == 'x') {
    *success = true;
    return reg_l(R_ECX);
  } else if (s[2] == 'b') {
    if (s[3] == 'x') {
      *success = true;
      return reg_l(R_EBX);
    } else if (s[3] == 'p') {
      *success = true;
      return reg_l(R_EBP);
    }
  } else if (s[2] == 'd') {
    if (s[3] == 'x') {
      *success = true;
      return reg_l(R_EDX);
    } else if (s[3] == 'p') {
      *success = true;
      return reg_l(R_EBP);
    }
  } else if (s[2] == 's') {
    if (s[3] == 'i') {
      *success = true;
      return reg_l(R_ESI);
    } else if (s[3] == 'p') {
      *success = true;
      return reg_l(R_ESP);
    }
  }
  *success = false;
  return 0;
}
