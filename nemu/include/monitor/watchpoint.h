#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char *expr;
  uint32_t oldVal;
} WP;
void init_wp_pool();
WP* new_wp();
void free_wp(WP*);
WP* get_wp(int);
#endif
