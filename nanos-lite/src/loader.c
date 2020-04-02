#include "proc.h"
#include <elf.h>
#include "fs.h"

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
extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int fs_close(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) {
  uintptr_t prog_entry;
  int fd = fs_open(filename, 0, 0);
  Elf_Ehdr elfHeader;
  fs_read(fd, (void *)&elfHeader, sizeof(Elf_Ehdr));
  prog_entry = elfHeader.e_entry;
  uintptr_t phoff = elfHeader.e_phoff;
  Elf_Phdr tmpPhdr;
  for (int i = 0; i < elfHeader.e_phnum; ++i) {
    fs_lseek(fd, phoff, SEEK_SET);
    fs_read(fd, &tmpPhdr, sizeof(Elf_Phdr));
    if (tmpPhdr.p_type == PT_LOAD) {
      fs_lseek(fd, tmpPhdr.p_offset, SEEK_SET);
      fs_read(fd, (void *)tmpPhdr.p_vaddr, tmpPhdr.p_filesz);
      if (tmpPhdr.p_memsz > tmpPhdr.p_filesz) {
        memset((void *)tmpPhdr.p_vaddr + tmpPhdr.p_filesz, 0,
               tmpPhdr.p_memsz - tmpPhdr.p_filesz);
      }
    }
    phoff += sizeof(Elf_Phdr);
  }
  fs_close(fd);
  return prog_entry;
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
