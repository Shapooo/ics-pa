#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM, TK_XNUM, TK_REG

  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {"0x[[:xdigit:]]+", TK_XNUM},
  {"[[:digit:]]+", TK_NUM},
  {"\\$[[:alpha:]]", TK_REG},
  {"\\*", '*'},
  {"\\(", '('},
  {"\\)", ')'},
  {"-", '-'},
  {"/", '/'},
  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"==", TK_EQ}         // equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e)
{
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
        case TK_NOTYPE:
          break;
        default:
          tokens[nr_token].type = rules[i].token_type;
          memcpy(tokens[nr_token].str, substr_start, substr_len);
          tokens[nr_token].str[substr_len] = '\0';
          nr_token++;
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}
static int eval(int, int);
uint32_t expr(char *e, bool *success)
{
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  /* TODO(); */
  int ret = eval(0, nr_token - 1);
  return ret;
}
static bool check_parenthesis(int p, int q);
static int getPropOp(int p, int q);
static int str2val(int p);
static int dereference(int addr);
static int eval(int p, int q)
{
  if (p > q) {
    return 0;
    assert(0);
  } else if (p == q) {
    return str2val(p);
  } else if (q - p == 1) {
    if (tokens[p].type == '-') {
      return -str2val(q);
    } else if (tokens[p].type == '*') {
      return dereference(str2val(q));
    }
  } else if (check_parenthesis(p, q) == true) {
    return eval(p + 1, q - 1);
  } else {
    int op_index = getPropOp(p, q);
    int val1 = eval(p, op_index - 1);
    int val2 = eval(op_index + 1, q);
    switch (tokens[op_index].type) {
    case '+':
      return val1 + val2;
    case '-':
      return val1 - val2;
    case '*':
      return val1 * val2;
    case '/':
      return val1 / val2;
    default:
      assert(0);
    }
  }
  return 0;
}

static int dereference(int addr)
{
  return 0;
}
int str2dec(char *str)
{
  int len = strlen(str);
  int result = 0;
  for (int i = 0; i < len; ++i) {
    result *= 10;
    result += str[i] - '0';
  }
  return result;
}

int str2hex(char *str)
{
  int len = strlen(str);
  int result = 0;
  for (int i = 2; i < len; ++i) {
    int tmp;
    if (str[i] <= '9' && str[i] >= '0') {
      tmp = str[i] - '0';
    } else if (str[i] >= 'a') {
      tmp = str[i] - 'a' + 10;
    } else {
      tmp = str[i] - 'A' + 10;
    }
    result *= 16;
    result += tmp;
  }
  return result;
}
int str2val(int p)
{
  switch (tokens[p].type) {
  case TK_NUM:
    return str2dec(tokens[p].str);
  case TK_XNUM:
    return str2hex(tokens[p].str);
  default:
    assert(0);
  }
}
#define isOp(type) ((type) == '+' || (type) == '*' || (type) == '/' || (type == '-'))

static inline int inferior(int p, int q)
{
  int firstType = 2, secondType = 2;
  if (tokens[p].type == '+' || tokens[p].type == '-') {
    firstType = 0;
  } else if (tokens[p].type == '/' || tokens[p].type == '*') {
    firstType = 1;
  }

  if (tokens[q].type == '+' || tokens[q].type == '-') {
    secondType = 0;
  } else if (tokens[q].type == '/' || tokens[q].type == '*') {
    secondType = 1;
  }

  return (secondType <= firstType) ? q : p;
}
static int getPropOp(int p, int q)
{
  int top = 0, index = p;
  int propIndex = p;
  while (index < q) {
    if (tokens[index].type == '(') {
      top++;
    } else if (tokens[index].type == ')') {
      top--;
    }
    if (top == 0 && (isOp(tokens[index].type))) {
      propIndex = inferior(propIndex, index);
    }
    index++;
  }

  if (propIndex <= p || propIndex > q) {
    Log("Unproper Index : %d\n", propIndex);
    return -1;
  } else {
    return propIndex;
  }
}

static bool check_parenthesis(int p, int q)
{
  if (tokens[p].type == '(' && tokens[q].type == ')') {
    int top = 0;
    top++;
    for (int i = p + 1; i < q; ++i) {
      if (tokens[i].type == '(') {
        top++;
      } else if (tokens[i].type == ')') {
        top--;
      }
      if (top == 0) {
        return false;
      }
    }
    return true;
  } else {
    return false;
  }
}
