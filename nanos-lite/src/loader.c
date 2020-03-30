#include "proc.h"
#include <elf.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
  /* TODO(); */
  uintptr_t text_start;

  Elf32_Ehdr elfHeader;
  ramdisk_read(&elfHeader, 0, sizeof(Elf32_Ehdr));
  text_start = elfHeader.e_entry;
  Elf32_Off e_phoff = elfHeader.e_phoff;
  Elf32_Half e_phnum = elfHeader.e_phnum;
  Elf32_Phdr tmpPhdr;
  for (int i = 0; i < e_phnum; ++i) {
    ramdisk_read(&tmpPhdr, e_phoff, sizeof(Elf32_Phdr));
    if (tmpPhdr.p_type == PT_LOAD) {
      ramdisk_write((void *)tmpPhdr.p_vaddr, tmpPhdr.p_offset,
                    tmpPhdr.p_filesz);
      if (tmpPhdr.p_memsz > tmpPhdr.p_filesz) {
        memset((void *)tmpPhdr.p_vaddr + tmpPhdr.p_filesz, 0,
               tmpPhdr.p_memsz - tmpPhdr.p_filesz);
      }

      if (tmpPhdr.p_flags == (PF_R | PF_X)) {
        text_start = tmpPhdr.p_vaddr;
      }
    }
    e_phoff += sizeof(Elf32_Phdr);
  }

  return text_start;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = 0x%x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
