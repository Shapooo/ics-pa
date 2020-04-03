#include "common.h"
#include <amdev.h>

size_t serial_write(const void *buf, size_t offset, size_t len) {
  assert(buf);
  char * ch = (char *)buf;
  for (int i = 0; i < len; ++i) {
    _putc(*ch++);
  }

  return ch - (char *)buf;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

/* FIXME: In navy-apps/tests/events test, fgetc get '\0' char
 * every time:
 * fgetc get one char only (len = 1), that makes snprintf
 * put '\0' to output. */
size_t events_read(void *buf, size_t offset, size_t len) {
  size_t real_size;

  int keycode = read_key();
  int down = keycode >> 15;
  keycode &= 0x7fff;
  if (keycode != _KEY_NONE) {
    real_size = snprintf(buf, len, "k%c %s", (down) ? 'd' : 'u', keyname[keycode]);
  } else {
    real_size = snprintf(buf, len, "t %d", uptime());
  }
  printf("%d\n", *(char *)buf);
  return real_size;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
}
