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

size_t events_read(void *buf, size_t offset, size_t len) {
  size_t real_size;
  int keycode = read_key();
  int down = keycode >> 15;
  keycode &= 0x7fff;
  if (keycode != _KEY_NONE) {
    real_size = snprintf(buf, len, "k%c %s\n", (down) ? 'd' : 'u', keyname[keycode]) - 1;
  } else {
    real_size = snprintf(buf, len, "t %d\n", uptime()) - 1;
  }
  return real_size;
}

static char dispinfo[128] __attribute__((used)) = {};

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  size_t real_len = strlen(dispinfo);
  if (offset >= real_len) {
    return 0;
  } else {
    real_len = (real_len < offset + len) ? real_len - offset : len;
    memcpy(buf, dispinfo + offset, real_len);
  }
  return real_len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int x_co, y_co, width, height;
  size_t real_len, fb_size;

  width = screen_width();
  height = screen_height();
  fb_size = width * height * sizeof(uint32_t);

  if (offset >= fb_size) {
    return 0;
  }

  real_len = (fb_size < offset + len) ? fb_size - offset : len;
  x_co = (offset / 4) % width;
  y_co = (offset / 4) / width;
  if (real_len / 4 <= width - x_co) {
    draw_rect((uint32_t*)buf, x_co, y_co, real_len / 4, 1);
  } else {
    int x_delta = width - x_co;
    int y_delta = 1;
    draw_rect((uint32_t*)buf, x_co, y_co, x_delta, 1);
    y_co += y_delta;
    y_delta = (real_len / 4 - x_delta) / width;
    x_delta = (real_len / 4 - x_delta) % width;
    draw_rect((uint32_t*)buf, 0, y_co, width, y_delta);
    y_co += y_delta;
    draw_rect((uint32_t*)buf, 0, y_co, x_delta, 1);
  }
  return real_len;
}

size_t fbsync_write(const void *buf, size_t offset, size_t len) {
  draw_sync();
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_width(), screen_height());
}
