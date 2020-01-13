#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[60000];
static int buffSize = 0;
static int tokenSum = 0;
static inline void gen(char c)
{
  buf[buffSize++] = c;
}
static inline void gen_rand_op()
{
  switch (rand() % 4) {
  case 0:
    gen('+');
    break;
  case 1:
    gen('-');
    break;
  case 2:
    gen('*');
    break;
  case 3:
    gen('/');
    break;
  }
}
static inline void gen_num()
{
  unsigned int num = rand() % 65535;
  int low = buffSize;
  while (num > 0) {
    buf[buffSize++] = num % 10 + '0';
    num /= 10;
  }
  int high = buffSize - 1;
  char tmp;
  while (low < high) {
    tmp = buf[low];
    buf[low] = buf[high];
    buf[high] = tmp;
    low++;
    high--;
  }

}
static inline void gen_rand_expr()
{
  if (tokenSum >= 30) {
    gen_num();
    tokenSum++;
  } else {
    switch (rand() % 3) {
    case 0:
      gen_num();
      tokenSum++;
      break;
    case 1:
      gen('(');
      tokenSum += 2;
      gen_rand_expr();
      gen(')');
      break;
    default:
      tokenSum++;
      gen_rand_expr();
      gen_rand_op();
      gen_rand_expr();
      break;
    }
  }
}

static char code_buf[65536];
static char *code_format =
  "#include <stdio.h>\n"
  "int main() { "
  "  unsigned result = %s; "
  "  printf(\"%%u\", result); "
  "  return 0; "
  "}";

int main(int argc, char *argv[])
{
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    buffSize = 0;
    tokenSum = 0;
    gen_rand_expr();
    buf[buffSize] = 0;
    if (tokenSum > 32) {
      i--;
      continue;
    }
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr 2> /tmp/.gccoutput");
    if (ret != 0) continue;
    fp = fopen("/tmp/.gccoutput", "r");
    if (fp != NULL && fgetc(fp) != -1) {
      fclose(fp);
      i --;
      continue;
    }
    fclose(fp);

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);
    /* if (result == 0) { */
    /*   i --; */
    /*   continue; */
    /* } */
    printf("%u %s\n", result, buf);
  }
  return 0;
}
