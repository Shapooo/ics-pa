#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);
static int cmd_si(char *args);
static int cmd_info(char *args);
static int cmd_p(char *args);
static int cmd_x(char *args);
static int cmd_w(char *args);
static int cmd_d(char *args);
static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Single step execution", cmd_si},
  { "info", "Print status of the program", cmd_info},
  { "p", "Evaluate the expression", cmd_p},
  { "x", "Scan the blocks of memory", cmd_x},
  { "w", "Set a watchpoint that returns value of a varing expression", cmd_w},
  { "d", "Delete a watchpoint", cmd_d}
  /* TODO: Add more commands */
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_si(char *args)
{
  char *stepStr = strtok(NULL, " ");
  if (stepStr && *stepStr) {
    int len = strlen(stepStr), stepNum = 0;
    for (int i = 0; i < len; ++i) {
      if (stepStr[i] > '9' || stepStr[i] < 0) {
        printf("Pls input a int after si\n");
        break;
      }
      stepNum *= 10;
      stepNum += stepStr[i] - '0';
    }
    cpu_exec(stepNum);
  } else {
    printf("Pls input a int after si\n");
  }
  return 0;
}

static int cmd_info(char *args)
{
  return 0;
}

static int cmd_p(char *args)
{
  bool succ;
  int ret = expr(args, &succ);
  if (succ) {
    printf("%d\n", ret);
  } else {
    printf("Unproper expression\n");
  }
  return 0;
}

static int cmd_x(char *args)
{
  return 0;
}

static int cmd_w(char *args)
{
  WP *wp = new_wp();
  bool succ;
  strcpy(wp->expr, args);
  int tmp = expr(wp->expr, &succ);
  assert(succ);
  wp->oldVal = tmp;
  return 0;
}

static int cmd_d(char *args)
{
  int num;
  sscanf(args, "%d", &num);
  free_wp(get_wp(num));
  return 0;
}

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop(int is_batch_mode) {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue(void);
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
