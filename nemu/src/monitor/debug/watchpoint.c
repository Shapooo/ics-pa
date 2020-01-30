#include <stdlib.h>
#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp()
{
  if (free_) {
    WP* tmp = head;
    head = free_;
    free_ = free_->next;
    head->next = tmp;
    return head;
  } else {
    printf("No more idle watchpoint.\n");
    assert(0);
  }
}

void free_wp(WP *wp)
{
  /* assert(wp <= wp[NR_WP]); */
  /* assert(wp >= wp); */
  WP *tmp = head;
  while (tmp && tmp->next != wp) {
    tmp = tmp->next;
  }
  assert(tmp);
  free(tmp->expr);
  tmp->next = wp->next;
  wp->next = free_->next;
  free_->next = wp;
}

inline WP* get_wp(int x)
{
  assert(x >= 0);
  assert(x <= NR_WP);
  return wp_pool + x;
}
