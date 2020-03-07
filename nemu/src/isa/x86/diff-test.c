#include "nemu.h"
#include "monitor/diff-test.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  /* return false; */
  uint32_t *refRegP = (uint32_t*)ref_r;
  uint32_t *locRegP = (uint32_t*)&cpu;
  int i = 0;
  for (; i < (DIFFTEST_REG_SIZE / sizeof(uint32_t)); ++i) {
    if (refRegP[i] != locRegP[i]) {
      printf("reg[%d] was not matched\n", i);
      return false;
    }
  }
  /* if ((refRegP[i] & 0x8c1) != (locRegP[i] & 0x8c1)) { */
  /*   return false; */
  /* } */
  return true;

}

void isa_difftest_attach(void) {
}
