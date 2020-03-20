#include "klib.h"
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

struct printf_info {
  int prec;  /* Precision.  */
  int width; /* Width.  */
  int spec;
  unsigned int space : 1;          /* Space flag.  */
  unsigned int showsign : 1;       /* + flag.  */
  unsigned int extra : 1;          /* For special use.  */
  unsigned int left : 1;           /* - flag.  */
  unsigned int alt : 1;            /* # flag.  */
  unsigned int group : 1;          /* ' flag.  */
  unsigned int i18n : 1;           /* I flag.  */
  unsigned int wide : 1;           /* Nonzero for wide character streams.  */
  unsigned int is_char : 1;        /* hh flag.  */
  unsigned int is_short : 1;       /* h flag.  */
  unsigned int is_long : 1;        /* l flag.  */
  unsigned int is_long_double : 1; /* L flag.  */
  unsigned int __padding : 20;     /* non-gnu: match _flags width of 32 bits */
  int pad;
};

enum { POINTER, DEC, HEX, STRING, CHAR, DOUBLE, OCT, UINT };

typedef union {
  char wc;
  unsigned int u;
  unsigned long ul;
  double fd;
  void *p;
} argvalue_t;

typedef struct {
  struct printf_info pf;
  const char *fmtpos;
  va_list arg;
  int argtype;
  argvalue_t argvalue;
} ppfs_t;

#define BUF_SIZE 1000
int printf(const char *fmt, ...) {
  char buf[BUF_SIZE];
  va_list args;
  va_start(args, fmt);
  int len_ret = vsprintf(buf, fmt, args);
  va_end(args);

  int i = 0;
  while (buf[i]) {
    _putc(buf[i++]);
  }
  return len_ret;
}

#define OUT_SIZE_MAX 0x7fffffff
int vsprintf(char *out, const char *fmt, va_list ap) {
  return vsnprintf(out, OUT_SIZE_MAX, fmt, ap);
}

int sprintf(char *out, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len_ret = vsprintf(out, fmt, args);
  va_end(args);
  return len_ret;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int len_ret = vsnprintf(out, n, fmt, args);
  va_end(args);
  return len_ret;
}

static char *pri_itoa(unsigned int value, char *buflim, unsigned base, int uppercase);
static int _ppfs_init(ppfs_t *ppfs, const char *fmt, size_t size);
static void _ppfs_parsespec(ppfs_t *ppfs);
int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  int count = 0;
  ppfs_t ppfs;
  _ppfs_init(&ppfs, fmt, n);
  va_copy(ppfs.arg, ap);
#define CONVBUF_SIZE 20
  char conv_buf[CONVBUF_SIZE];
  char *a, *z;

  while (*fmt != '\0') {
    a = (char *)fmt;
    while (*fmt && *fmt != '%') {
      fmt++;
    }
    z = (char *)fmt;
    while (fmt[0] == '%' && fmt[1] == '%') {
      fmt += 2;
      z++;
    }

    if (z - a > 0) {
      if (z - a + count < n) {
        memcpy(out + count, a, z - a);
        count += z - a;
        continue;
      } else {
        memcpy(out + count, a, n - count - 1);
        count = n;
        break;
      }
    }

    ppfs.fmtpos = ++fmt;
    z = conv_buf + CONVBUF_SIZE;
    _ppfs_parsespec(&ppfs);
    fmt = ppfs.fmtpos;
    int isNeg = 0;
    switch (ppfs.argtype) {
    case (STRING): {
      a = ppfs.argvalue.p ? ppfs.argvalue.p : "(NULL)";
      z = a + strlen(ppfs.argvalue.p);
      break;
    }
    case (DEC): {
      isNeg = (ppfs.argvalue.u >> ((sizeof(ppfs.argvalue.u) << 3) - 1)) ? 1 : 0;
      if (isNeg) {
      ppfs.argvalue.u = ~(ppfs.argvalue.u) + 1;
      }
      a = pri_itoa(ppfs.argvalue.u, z, 10, 0);
      if (isNeg) {
        *(--a) = '-';
      } else if (ppfs.pf.showsign) {
        *(--a) = '+';
      }
      break;
    }
    case (HEX): {
      a = pri_itoa(ppfs.argvalue.u, z, 16, 0);
      break;
    }
    case (OCT): {
      isNeg = (ppfs.argvalue.u >> ((sizeof(ppfs.argvalue.u) << 3) - 1)) ? 1 : 0;
      ppfs.argvalue.u = ~(ppfs.argvalue.u) + 1;
      a = pri_itoa(ppfs.argvalue.u, z, 8, 0);
      if (isNeg) {
        *(--a) = '-';
      } else if (ppfs.pf.showsign) {
        *(--a) = '+';
      }
      break;
    }
    case (CHAR): {
      *(a = (z - 1)) = ppfs.argvalue.wc;
      break;
    }
    case (UINT): {
      a = pri_itoa(ppfs.argvalue.u, z, 10, 0);
      break;
    }
    case (POINTER): {
      break;
    }
    }

    if (count + z - a < n) {
      memcpy(out + count, a, z - a);
      count += z - a;
    } else {
      memcpy(out + count, a, n - count - 1);
      count = n - 1;
      break;
    }
  }
  *(out + (count++)) = '\0';
  va_end(ap);

  return count;
}

static char *pri_itoa(unsigned int value, char *buflim, unsigned base, int uppercase) {
  const char _itoa_upper_digits[36] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const char _itoa_lower_digits[36] = "0123456789abcdefghijklmnopqrstuvwxyz";

  const char *digits = (uppercase ? _itoa_upper_digits : _itoa_lower_digits);

  switch (base) {
#define SPECIAL(Base)                                                          \
  case Base:                                                                   \
    do                                                                         \
      *--buflim = digits[value % Base];                                        \
    while ((value /= Base) != 0);                                              \
    break

    SPECIAL(10);
    SPECIAL(16);
    SPECIAL(8);
  default:
    do
      *--buflim = digits[value % base];
    while ((value /= base) != 0);
  }
  return buflim;
}
#undef SPECIAL

static void _ppfs_parsespec(ppfs_t *ppfs) {
  assert(*(ppfs->fmtpos - 1) == '%');

  switch(*(ppfs->fmtpos)){
  case ('-'):
    ppfs->pf.left = 1;
    if (0) {
  case ('+'):
    ppfs->pf.showsign = 1;
    } if (0) {
  case (' '):
    ppfs->pf.space = 1;
    } if (0) {
  case ('#'):
    ppfs->pf.alt = 1;
    } if (0) {
  case ('0'):
    ppfs->pf.pad = '0';
    }
    ppfs->fmtpos++;
    break;
  }

  if (*(ppfs->fmtpos) <= '9' && *(ppfs->fmtpos) >= '1') {
    /* TODO */
    ppfs->fmtpos++;
  }

  switch (*(ppfs->fmtpos)) {
  case ('h'): {
    ppfs->pf.is_short = 1;
    break;
  }
  case ('l'): {
    ppfs->pf.is_long = 1;
    break;
  }
  case ('L'): {
    ppfs->pf.is_long_double = 1;
    break;
  }
  }

  switch (*(ppfs->fmtpos)) {
  case ('c'):
    ppfs->fmtpos++;
    ppfs->argtype = CHAR;
    ppfs->argvalue.wc = va_arg(ppfs->arg, int);
    break;
  case ('s'):
    ppfs->fmtpos++;
    ppfs->argtype = STRING;
    ppfs->argvalue.p = va_arg(ppfs->arg, char *);
    break;
  case ('u'):
    ppfs->argtype = UINT;
    if (0) {
  case ('d'): case ('i'):
    ppfs->argtype = DEC;
    } if (0) {
  case ('X'): case ('x'):
    ppfs->argtype = HEX;
    } if (0) {
  case ('o'):
    ppfs->argtype = OCT;
    }
    if (0) {
  case ('p'):
    ppfs->argtype = POINTER;
    }
    ppfs->fmtpos++;
    ppfs->argvalue.u = va_arg(ppfs->arg, int);
    break;
  default:
    ppfs->fmtpos++;
    break;
  }
}

static int _ppfs_init(ppfs_t *ppfs, const char *fmt, size_t size) {
  memset(ppfs, 0, sizeof(ppfs_t));
  ppfs->fmtpos = (char *)fmt;
  return 0;
}

#endif
